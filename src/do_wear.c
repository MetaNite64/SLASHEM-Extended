/*	SCCS Id: @(#)do_wear.c	3.4	2003/11/14	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

#ifndef OVLB

STATIC_DCL long takeoff_mask, taking_off;

#else /* OVLB */

STATIC_OVL NEARDATA long takeoff_mask = 0L;
static NEARDATA long taking_off = 0L;

static NEARDATA int todelay;
static boolean cancelled_don = FALSE;

static NEARDATA const char see_yourself[] = "see yourself";
static NEARDATA const char unknown_type[] = "Unknown type of %s (%d)";
static NEARDATA const char c_armor[]  = "armor",
			   c_suit[]   = "suit",
#ifdef TOURIST
			   c_shirt[]  = "shirt",
#endif
			   c_cloak[]  = "cloak",
			   c_gloves[] = "gloves",
			   c_boots[]  = "boots",
			   c_helmet[] = "helmet",
			   c_shield[] = "shield",
			   c_weapon[] = "weapon",
			   c_sword[]  = "sword",
			   c_axe[]    = "axe",
			   c_that_[]  = "that";

static NEARDATA const long takeoff_order[] = { WORN_BLINDF, W_WEP,
	WORN_SHIELD, WORN_GLOVES, LEFT_RING, RIGHT_RING, WORN_CLOAK,
	WORN_HELMET, WORN_AMUL, WORN_ARMOR,
#ifdef TOURIST
	WORN_SHIRT,
#endif
	WORN_BOOTS, W_SWAPWEP, W_QUIVER, 0L };

STATIC_DCL void FDECL(on_msg, (struct obj *));
STATIC_DCL void FDECL(on_msgdel, (struct obj *));
STATIC_DCL void FDECL(Ring_off_or_gone, (struct obj *, BOOLEAN_P));
STATIC_PTR int FDECL(select_off, (struct obj *));
STATIC_DCL struct obj *NDECL(do_takeoff);
STATIC_PTR int NDECL(take_off);
STATIC_DCL int FDECL(menu_remarm, (int));
STATIC_DCL void FDECL(already_wearing, (const char*));
STATIC_DCL void FDECL(already_wearing2, (const char*, const char*));

void
off_msg(otmp)
register struct obj *otmp;
{
	if(flags.verbose)
	    You("were wearing %s.", doname(otmp));
}

/* for items that involve no delay */
STATIC_OVL void
on_msg(otmp)
register struct obj *otmp;
{
	if (flags.verbose) {
	    char how[BUFSZ];

	    how[0] = '\0';
	    if (otmp->otyp == TOWEL)
		Sprintf(how, " around your %s", body_part(HEAD));
	    You("are now wearing %s%s.",
		obj_is_pname(otmp) ? the(xname(otmp)) : an(xname(otmp)),
		how);
	}
}

STATIC_OVL void
on_msgdel(otmp)
register struct obj *otmp;
{
	if (flags.verbose) {
	    char how[BUFSZ];

	    how[0] = '\0';
	    if (otmp->otyp == TOWEL)
		Sprintf(how, " around your %s", body_part(HEAD));
	    You("are putting on %s%s.",
		obj_is_pname(otmp) ? the(xname(otmp)) : an(xname(otmp)),
		how);
	}
}

/*
 * The Type_on() functions should be called *after* setworn().
 * The Type_off() functions call setworn() themselves.
 */

int
Boots_on()
{
    long oldprop; 
    if (!uarmf) return 0; 
    oldprop = u.uprops[objects[uarmf->otyp].oc_oprop].extrinsic & ~WORN_BOOTS;

    switch(uarmf->otyp) {
#ifdef JEDI
	case PLASTEEL_BOOTS:
#endif
	case LOW_BOOTS:
	case IRON_SHOES:
	case HIGH_BOOTS:
	case GNOMISH_BOOTS:
	case WEDGE_SANDALS:
	case FEMININE_PUMPS:
	case DANCING_SHOES:
	case SWEET_MOCASSINS:
	case SOFT_SNEAKERS:
	case LEATHER_PEEP_TOES:
	case COMBAT_STILETTOS:
	case JUMPING_BOOTS:
	case FLYING_BOOTS:
	case KICKING_BOOTS:
	case FIRE_BOOTS:
	case BOOTS_OF_ACID_RESISTANCE:
		break;
	case BOOTS_OF_MOLASSES:
		pline(Hallucination ? "Icy legs, how cool!" : "These boots feel a little cold...");
		break;
	case ZIPPER_BOOTS:
		pline(Hallucination ? "Lovely fleecy caressing feelings stream into your legs!" : "While putting on this pair of boots, their zippers try to scratch your legs!");
		losehp(rnd(20), "foolishly putting on a zipper boot", KILLED_BY);
		break;
	case HIPPIE_HEELS:
		if (flags.female) pline("You're dressed like a '69 whore!");
		else pline("You're dressed like a frickin' hardrocker!");
	    if (!uarmf->cursed) {curse(uarmf); pline("The highly erotic boots weld themselves to your feet!");}
		break;

	case AUTODESTRUCT_DE_VICE_BOOTS:
	case SPEEDBUG_BOOTS:
	case SENTIENT_HIGH_HEELED_SHOES:
	case BOOTS_OF_FAINTING:
	case DIFFICULT_BOOTS:
	case BOOTS_OF_WEAKNESS:
	case GRIDBUG_CONDUCT_BOOTS:
	case STAIRWELL_STOMPING_BOOTS:
		if (!uarmf->cursed) curse(uarmf);
		break;

	case WATER_WALKING_BOOTS:
		if (u.uinwater) spoteffects(TRUE);
		break;
	case SPEED_BOOTS:
		/* Speed boots are still better than intrinsic speed, */
		/* though not better than potion speed */
		if (!oldprop && !(HFast & TIMEOUT)) {
			makeknown(uarmf->otyp);
			You_feel("yourself speed up%s.",
				(oldprop || HFast) ? " a bit more" : "");
		}
		break;
	case ELVEN_BOOTS:
		if (!oldprop && !HStealth && !BStealth) {
			makeknown(uarmf->otyp);
			You("walk very quietly.");
		}
		break;
	case FUMBLE_BOOTS:
		if (!oldprop && !(HFumbling & ~TIMEOUT))
			incr_itimeout(&HFumbling, rnd(20));
		break;
	case LEVITATION_BOOTS:
		if (!oldprop && !HLevitation) {
			flags.botl = 1;
			makeknown(uarmf->otyp);
			float_up();
			spoteffects(FALSE);
		}
		break;
	default: impossible(unknown_type, c_boots, uarmf->otyp);
    }
    return 0;
}

int
Boots_off()
{
    int otyp = uarmf->otyp;
    long oldprop = u.uprops[objects[otyp].oc_oprop].extrinsic & ~WORN_BOOTS;

    takeoff_mask &= ~W_ARMF;
	/* For levitation, float_down() returns if Levitation, so we
	 * must do a setworn() _before_ the levitation case.
	 */
    setworn((struct obj *)0, W_ARMF);
    switch (otyp) {
	case SPEED_BOOTS:
		if (!Very_fast && !cancelled_don) {
			makeknown(otyp);
			You_feel("yourself slow down%s.",
				Fast ? " a bit" : "");
		}
		break;
	case WATER_WALKING_BOOTS:
		if (is_pool(u.ux,u.uy) && !Levitation && !Flying &&
		    !is_clinger(youmonst.data) && !cancelled_don) {
			makeknown(otyp);
			/* make boots known in case you survive the drowning */
			spoteffects(TRUE);
		}
		break;
	case ELVEN_BOOTS:
		if (!oldprop && !HStealth && !BStealth && !cancelled_don) {
			makeknown(otyp);
			You("sure are noisy.");
		}
		break;
	case FUMBLE_BOOTS:
		if (!oldprop && !(HFumbling & ~TIMEOUT))
			HFumbling = EFumbling = 0;
		break;
	case FLYING_BOOTS:
		(void) float_down(0L, 0L);
		break;
	case LEVITATION_BOOTS:
		if (!oldprop && !HLevitation && !cancelled_don) {
			flags.botl = 1;
			(void) float_down(0L, 0L);
			makeknown(otyp);
		}
		break;
	case ZIPPER_BOOTS:
		pline(Hallucination ? "You get the feeling that something soft just slid along the full length of your legs!" : "While taking off this pair of boots, you scratch open your legs at their zippers!");
		losehp(rnd(20), "foolishly taking off a zipper boot", KILLED_BY);
		    set_wounded_legs(LEFT_SIDE, rn1(35, 41));
		    set_wounded_legs(RIGHT_SIDE, rn1(35, 41));
		break;
	case LOW_BOOTS:
	case IRON_SHOES:
	case GNOMISH_BOOTS:
	case BOOTS_OF_MOLASSES:
	case WEDGE_SANDALS:
	case FEMININE_PUMPS:
	case DANCING_SHOES:
	case SWEET_MOCASSINS:
	case SOFT_SNEAKERS:
	case LEATHER_PEEP_TOES:
	case COMBAT_STILETTOS:
	case HIGH_BOOTS:
	case JUMPING_BOOTS:
	case KICKING_BOOTS:
	case FIRE_BOOTS:
	case AUTODESTRUCT_DE_VICE_BOOTS:
	case SPEEDBUG_BOOTS:
	case SENTIENT_HIGH_HEELED_SHOES:
	case BOOTS_OF_FAINTING:
	case DIFFICULT_BOOTS:
	case BOOTS_OF_WEAKNESS:
	case GRIDBUG_CONDUCT_BOOTS:
	case STAIRWELL_STOMPING_BOOTS:
	case BOOTS_OF_ACID_RESISTANCE:
#ifdef JEDI
	case PLASTEEL_BOOTS:
#endif
		break;
	case HIPPIE_HEELS:

		pline("You decide you had enough of those hallucination-inducing boots.");
		break;

	default: impossible(unknown_type, c_boots, otyp);
    }
    cancelled_don = FALSE;
    return 0;
}

int
Cloak_on()
{
    long oldprop;
	if (!uarmc) return 0;
	oldprop = u.uprops[objects[uarmc->otyp].oc_oprop].extrinsic & ~WORN_CLOAK;

    switch(uarmc->otyp) {
	case ELVEN_CLOAK:
	case CLOAK_OF_PROTECTION:
	case CLOAK_OF_DISPLACEMENT:
		makeknown(uarmc->otyp);
		break;
	case ORCISH_CLOAK:
	case DWARVISH_CLOAK:
	case CLOAK_OF_MAGIC_RESISTANCE:
	case CLOAK_OF_REFLECTION:
	case ROBE:
	case LEATHER_CLOAK:
	case CLOAK_OF_WARMTH:
	case CLOAK_OF_GROUNDING:
	case CLOAK_OF_QUENCHING:
	case CLOAK_OF_CONFUSION:
	case MANACLOAK:
	case PLASTEEL_CLOAK:
	/* KMH, balance patch -- removed
	case CLOAK_OF_DRAIN_RESISTANCE: */               
		break;
	case CLOAK_OF_FUMBLING:
		if (!oldprop && !(HFumbling & ~TIMEOUT))
			incr_itimeout(&HFumbling, rnd(20));
		break;
	/* KMH, balance patch -- lab coat gives poison _and_ acid resistance */
	case LAB_COAT:
		EAcid_resistance |= WORN_CLOAK;
		break;
	case MUMMY_WRAPPING:
		/* Note: it's already being worn, so we have to cheat here. */
		if ((HInvis || EInvis || pm_invisible(youmonst.data)) && !Blind) {
		    newsym(u.ux,u.uy);
		    You("can %s!",
			See_invisible ? "no longer see through yourself"
			: see_yourself);
		}
		break;
	case CLOAK_OF_INVISIBILITY:
		/* since cloak of invisibility was worn, we know mummy wrapping
		   wasn't, so no need to check `oldprop' against blocked */
		if (!oldprop && !HInvis && !Blind) {
		    makeknown(uarmc->otyp);
		    newsym(u.ux,u.uy);
		    pline("Suddenly you can%s yourself.",
			See_invisible ? " see through" : "not see");
		}
		break;
	case POISONOUS_CLOAK:
		if (Poison_resistance)
			pline(Hallucination ? "Very tight, like a latex shirt!" : "This cloak feels a little itchy.");
		else {
		    makeknown(uarmc->otyp);
		    poisoned("cloak",A_STR,"poisonous cloak",3);
		}
		break;

	case CLOAK_OF_UNSPELLING:
	case ANTI_CASTER_CLOAK:
	case HEAVY_STATUS_CLOAK:
	case CLOAK_OF_LUCK_NEGATION:
	case YELLOW_SPELL_CLOAK:
	case VULNERABILITY_CLOAK:
	case CLOAK_OF_INVENTORYLESSNESS:

		if (!uarmc->cursed) curse(uarmc);
		break;

	case OILSKIN_CLOAK:
		pline("%s very tightly.", Tobjnam(uarmc, "fit"));
		break;
	/* Alchemy smock gives poison _and_ acid resistance */
#if 0
	case ALCHEMY_SMOCK:
		EAcid_resistance |= WORN_CLOAK;
  		break;
#endif
	default: impossible(unknown_type, c_cloak, uarmc->otyp);
    }
    return 0;
}

int
Cloak_off()
{
    int otyp = uarmc->otyp;
    long oldprop = u.uprops[objects[otyp].oc_oprop].extrinsic & ~WORN_CLOAK;

    takeoff_mask &= ~W_ARMC;
	/* For mummy wrapping, taking it off first resets `Invisible'. */
    setworn((struct obj *)0, W_ARMC);
    switch (otyp) {
	case ELVEN_CLOAK:
	case ORCISH_CLOAK:
	case DWARVISH_CLOAK:
	case CLOAK_OF_PROTECTION:
	case CLOAK_OF_MAGIC_RESISTANCE:
	case CLOAK_OF_REFLECTION:
	case CLOAK_OF_DISPLACEMENT:
	case CLOAK_OF_CONFUSION:
	case MANACLOAK:
	case POISONOUS_CLOAK:
	case OILSKIN_CLOAK:
	case ROBE:
	case PLASTEEL_CLOAK:
	case LEATHER_CLOAK:
	case CLOAK_OF_WARMTH:
	case CLOAK_OF_GROUNDING:
	case CLOAK_OF_QUENCHING:
	case CLOAK_OF_UNSPELLING:
	case ANTI_CASTER_CLOAK:
	case HEAVY_STATUS_CLOAK:
	case CLOAK_OF_LUCK_NEGATION:
	case YELLOW_SPELL_CLOAK:
	case VULNERABILITY_CLOAK:
	case CLOAK_OF_INVENTORYLESSNESS:
	/* KMH, balance patch -- removed
	case CLOAK_OF_DRAIN_RESISTANCE: */
		break;
	case CLOAK_OF_FUMBLING:
	    if (!oldprop && !(HFumbling & ~TIMEOUT))
		HFumbling = EFumbling = 0;
	    break;
	/* KMH, balance patch -- lab coat gives poison _and_ acid resistance */
	case LAB_COAT:
		EAcid_resistance &= ~WORN_CLOAK;
		break;
	case MUMMY_WRAPPING:
		if (Invis && !Blind) {
		    newsym(u.ux,u.uy);
		    You("can %s.",
			See_invisible ? "see through yourself"
			: "no longer see yourself");
		}
		break;
	case CLOAK_OF_INVISIBILITY:
		if (!oldprop && !HInvis && !Blind) {
		    makeknown(CLOAK_OF_INVISIBILITY);
		    newsym(u.ux,u.uy);
		    pline("Suddenly you can %s.",
			See_invisible ? "no longer see through yourself"
			: see_yourself);
		}
		break;
#if 0
	/* Alchemy smock gives poison _and_ acid resistance */
	case ALCHEMY_SMOCK:
		EAcid_resistance &= ~WORN_CLOAK;
  		break;
#endif
	default: impossible(unknown_type, c_cloak, otyp);
    }
    return 0;
}

int
Helmet_on()
{
    if (!uarmh) return 0; 
    switch(uarmh->otyp) {
	case FEDORA:
		set_moreluck();
		flags.botl = 1;
		break;
#ifdef JEDI
	case PLASTEEL_HELM:
#endif
	case HELMET:
	case HELM_OF_STEEL:
	case DENTED_POT:
	case ELVEN_LEATHER_HELM:
	case GNOMISH_HELM:
	case DWARVISH_IRON_HELM:
	case ORCISH_HELM:
	/* KMH, balance patch -- removed
	case FIRE_HELMET: */
	case HELM_OF_TELEPATHY:
	case HELM_OF_DISCOVERY:
	case HELM_OF_DRAIN_RESISTANCE:
	case HELM_OF_FEAR:
	case HELM_OF_HUNGER:
		break;
	case HELM_OF_BRILLIANCE:
		adj_abon(uarmh, uarmh->spe);
		break;
	case CORNUTHAUM:
		/* people think marked wizards know what they're talking
		 * about, but it takes trained arrogance to pull it off,
		 * and the actual enchantment of the hat is irrelevant.
		 */
		ABON(A_CHA) += (Role_if(PM_WIZARD) ? 1 : -1);
		flags.botl = 1;
		makeknown(uarmh->otyp);
		break;
	case HELM_OF_OPPOSITE_ALIGNMENT:
		if (u.ualign.type == A_NEUTRAL)
		    u.ualign.type = rn2(2) ? A_CHAOTIC : A_LAWFUL;
		else u.ualign.type = -(u.ualign.type);
		u.ublessed = 0; /* lose your god's protection */
	     /* makeknown(uarmh->otyp);   -- moved below, after xname() */
		/*FALLTHRU*/
	case DUNCE_CAP:
		if (!uarmh->cursed) {
		    if (Blind)
			pline("%s for a moment.", Tobjnam(uarmh, "vibrate"));
		    else
			pline("%s %s for a moment.",
			      Tobjnam(uarmh, "glow"), hcolor(NH_BLACK));
		    curse(uarmh);
		}
		flags.botl = 1;		/* reveal new alignment or INT & WIS */
		if (Hallucination) {
		    pline("My brain hurts!"); /* Monty Python's Flying Circus */
		} else if (uarmh->otyp == DUNCE_CAP) {
		    You_feel("%s.",	/* track INT change; ignore WIS */
		  ACURR(A_INT) <= (ABASE(A_INT) + ABON(A_INT) + ATEMP(A_INT)) ?
			     "like sitting in a corner" : "giddy");
		} else {
		    Your("mind oscillates briefly.");
		    makeknown(HELM_OF_OPPOSITE_ALIGNMENT);
		}
		break;
	case HELM_OF_STORMS:
	case HELM_OF_DETECT_MONSTERS:
		if (!uarmh->cursed) {
		    if (Blind)
			pline("%s for a moment.", Tobjnam(uarmh, "vibrate"));
		    else
			pline("%s %s for a moment.",
			      Tobjnam(uarmh, "glow"), hcolor(NH_BLACK));
		    curse(uarmh);
		}
		break;

	case HELM_OF_OBSCURED_DISPLAY:
	case HELM_OF_LOSE_IDENTIFICATION:
	case HELM_OF_THIRST:
	case BLACKY_HELMET:
	case ANTI_DRINKER_HELMET:
	case WHISPERING_HELMET:
	case CYPHER_HELM:
	case HELM_OF_BAD_ALIGNMENT:
	case SOUNDPROOF_HELMET:
	case OUT_OF_MEMORY_HELMET:
		if (!uarmh->cursed) curse(uarmh);
		break;
	default: impossible(unknown_type, c_helmet, uarmh->otyp);
    }
    return 0;
}

int
Helmet_off()
{
    takeoff_mask &= ~W_ARMH;

    switch(uarmh->otyp) {
	case FEDORA:
	    setworn((struct obj *)0, W_ARMH);
	    set_moreluck();
	    flags.botl = 1;
	    return 0;
#ifdef JEDI
	case PLASTEEL_HELM:
#endif
	case HELMET:
	case HELM_OF_STEEL:
	case DENTED_POT:
	case ELVEN_LEATHER_HELM:
	case GNOMISH_HELM:
	case DWARVISH_IRON_HELM:
	case ORCISH_HELM:
	case HELM_OF_DRAIN_RESISTANCE:
	case HELM_OF_FEAR:
	case HELM_OF_HUNGER:
	case HELM_OF_STORMS:
	case HELM_OF_DETECT_MONSTERS:
	case HELM_OF_OBSCURED_DISPLAY:
	case HELM_OF_LOSE_IDENTIFICATION:
	case HELM_OF_THIRST:
	case BLACKY_HELMET:
	case ANTI_DRINKER_HELMET:
	case WHISPERING_HELMET:
	case CYPHER_HELM:
	case HELM_OF_BAD_ALIGNMENT:
	case SOUNDPROOF_HELMET:
	case OUT_OF_MEMORY_HELMET:
	/* KMH, balance patch -- removed
	case FIRE_HELMET: */
	case HELM_OF_DISCOVERY:
	    break;
	case DUNCE_CAP:
	    flags.botl = 1;
	    break;
	case CORNUTHAUM:
	    if (!cancelled_don) {
		ABON(A_CHA) += (Role_if(PM_WIZARD) ? -1 : 1);
		flags.botl = 1;
	    }
	    break;
	case HELM_OF_TELEPATHY:
	    /* need to update ability before calling see_monsters() */
	    setworn((struct obj *)0, W_ARMH);
	    see_monsters();
	    return 0;
	case HELM_OF_BRILLIANCE:
	    if (!cancelled_don) adj_abon(uarmh, -uarmh->spe);
	    break;
	case HELM_OF_OPPOSITE_ALIGNMENT:
	    u.ualign.type = u.ualignbase[A_CURRENT];
	    u.ublessed = 0; /* lose the other god's protection */
	    flags.botl = 1;
	    break;
	default: impossible(unknown_type, c_helmet, uarmh->otyp);
    }
    setworn((struct obj *)0, W_ARMH);
    cancelled_don = FALSE;
    return 0;
}

int
Gloves_on()
{
    long oldprop; 
    if (!uarmg) return 0; 
    oldprop = u.uprops[objects[uarmg->otyp].oc_oprop].extrinsic & ~WORN_GLOVES;

    switch(uarmg->otyp) {
#ifdef JEDI
	case PLASTEEL_GLOVES:
#endif
	case LEATHER_GLOVES:
	case GAUNTLETS_OF_STEEL:
	case GAUNTLETS_OF_TYPING:
	case GAUNTLETS_OF_SLOWING:
	case GAUNTLETS_OF_REFLECTION:
		break;
	case OILSKIN_GLOVES:
		if (!uarmg->cursed) {
		    if (Blind)
			pline("%s for a moment.", Tobjnam(uarmg, "vibrate"));
		    else
			pline("%s %s for a moment.",
			      Tobjnam(uarmg, "glow"), hcolor(NH_BLACK));
		    curse(uarmg);
		}
		break;
	case MENU_NOSE_GLOVES:
	case UNWIELDY_GLOVES:
	case CONFUSING_GLOVES:
	case UNDROPPABLE_GLOVES:
	case GLOVES_OF_MISSING_INFORMATION:
	case GLOVES_OF_TRAP_CREATION:
	case SADO_MASO_GLOVES:
		if (!uarmg->cursed) curse(uarmg);
		break;

	case GAUNTLETS_OF_SWIMMING:
		if (u.uinwater) {
		   pline(Hallucination ? "Suddenly, you're floating! Whee!" : "Hey! You can swim!");
		   spoteffects(TRUE);
		}
		break;
	case GAUNTLETS_OF_FUMBLING:
		if (!oldprop && !(HFumbling & ~TIMEOUT))
			incr_itimeout(&HFumbling, rnd(20));
		break;
	case GAUNTLETS_OF_POWER:
		/*makeknown(uarmg->otyp);*/
		/*flags.botl = 1;*/ /* taken care of in attrib.c */
		adj_abon(uarmg, uarmg->spe);
		break;
	case GAUNTLETS_OF_DEXTERITY:
		adj_abon(uarmg, uarmg->spe);
		break;
	default: impossible(unknown_type, c_gloves, uarmg->otyp);
    }
    return 0;
}

int
Gloves_off()
{
    long oldprop =
	u.uprops[objects[uarmg->otyp].oc_oprop].extrinsic & ~WORN_GLOVES;

    takeoff_mask &= ~W_ARMG;

    switch(uarmg->otyp) {
#ifdef JEDI
	case PLASTEEL_GLOVES:
#endif
	case LEATHER_GLOVES:
	case OILSKIN_GLOVES:
	case GAUNTLETS_OF_STEEL:
	case GAUNTLETS_OF_TYPING:
	case GAUNTLETS_OF_REFLECTION:
	case GAUNTLETS_OF_SLOWING:
	case MENU_NOSE_GLOVES:
	case UNWIELDY_GLOVES:
	case CONFUSING_GLOVES:
	case UNDROPPABLE_GLOVES:
	case GLOVES_OF_MISSING_INFORMATION:
	case GLOVES_OF_TRAP_CREATION:
	case SADO_MASO_GLOVES:
	    break;
	case GAUNTLETS_OF_SWIMMING:
	    if (u.uinwater) {
	       You("begin to thrash about!");
	       spoteffects(TRUE);
	    }
	    break;
	case GAUNTLETS_OF_FUMBLING:
	    if (!oldprop && !(HFumbling & ~TIMEOUT))
		HFumbling = EFumbling = 0;
	    break;
	case GAUNTLETS_OF_POWER:
	    /*makeknown(uarmg->otyp);*/
	    /*flags.botl = 1;*/ /* taken care of in attrib.c */
	    if (!cancelled_don) adj_abon(uarmg, -uarmg->spe);
	    break;
	case GAUNTLETS_OF_DEXTERITY:
	    if (!cancelled_don) adj_abon(uarmg, -uarmg->spe);
	    break;
	default: impossible(unknown_type, c_gloves, uarmg->otyp);
    }
    setworn((struct obj *)0, W_ARMG);
    cancelled_don = FALSE;
    (void) encumber_msg();		/* immediate feedback for GoP */

    /* Prevent wielding cockatrice when not wearing gloves */
    if (uwep && uwep->otyp == CORPSE &&
		touch_petrifies(&mons[uwep->corpsenm])) {
	char kbuf[BUFSZ];

	You("wield the %s in your bare %s.",
	    corpse_xname(uwep, TRUE), makeplural(body_part(HAND)));
	Strcpy(kbuf, an(killer_cxname(uwep, TRUE)));
	instapetrify(kbuf);
	uwepgone();  /* life-saved still doesn't allow touching cockatrice */
    }

    /* KMH -- ...or your secondary weapon when you're wielding it */
    if (u.twoweap && uswapwep && uswapwep->otyp == CORPSE &&
	touch_petrifies(&mons[uswapwep->corpsenm])) {
	char kbuf[BUFSZ];

	You("wield the %s in your bare %s.",
	    corpse_xname(uswapwep, TRUE), body_part(HAND));

	Strcpy(kbuf, an(killer_cxname(uswapwep, TRUE)));
	instapetrify(kbuf);
	uswapwepgone();	/* lifesaved still doesn't allow touching cockatrice */
    }

    return 0;
}

int
Shield_on()
{
/*
    switch (uarms->otyp) {
	case SMALL_SHIELD:
	case ELVEN_SHIELD:
	case URUK_HAI_SHIELD:
	case ORCISH_SHIELD:
	case DWARVISH_ROUNDSHIELD:
	case LARGE_SHIELD:
	case STEEL_SHIELD:
	case SHIELD_OF_REFLECTION:
	case FLAME_SHIELD:
	case ICE_SHIELD:
	case LIGHTNING_SHIELD:
	case VENOM_SHIELD:
	case SHIELD_OF_LIGHT:
	case SHIELD_OF_MOBILITY:
		break;
	default: impossible(unknown_type, c_shield, uarms->otyp);
    }
*/
    return 0;
}

int
Shield_off()
{
    takeoff_mask &= ~W_ARMS;
/*
    switch (uarms->otyp) {
	case SMALL_SHIELD:
	case ELVEN_SHIELD:
	case URUK_HAI_SHIELD:
	case ORCISH_SHIELD:
	case DWARVISH_ROUNDSHIELD:
	case LARGE_SHIELD:
	case STEEL_SHIELD:
	case SHIELD_OF_REFLECTION:
	case FLAME_SHIELD:
	case ICE_SHIELD:
	case LIGHTNING_SHIELD:
	case VENOM_SHIELD:
	case SHIELD_OF_LIGHT:
	case SHIELD_OF_MOBILITY:
		break;
	default: impossible(unknown_type, c_shield, uarms->otyp);
    }
*/
    setworn((struct obj *)0, W_ARMS);
    return 0;
}

#ifdef TOURIST
int
Shirt_on()
{
/*
    switch (uarmu->otyp) {
	case HAWAIIAN_SHIRT:
	case T_SHIRT:
		break;
	default: impossible(unknown_type, c_shirt, uarmu->otyp);
    }
*/

	/* Cursed underwear/shirt may lifesave a player. It is therefore a good idea to curse them. --Amy
	   Nobles and activistors will be able to voluntarily make them cursed. */

	if(uarmu->otyp == VICTORIAN_UNDERWEAR){

		if ((Role_if(PM_NOBLEMAN) || Role_if(PM_NOBLEWOMAN) || Role_if(PM_ACTIVISTOR)) && !uarmu->cursed ) {if (yn_function("Do you want your sexy underwear to stick?", ynchars, 'n') == 'y') { curse(uarmu);
			pline("Your fleecy underwear gently welds itself to your curved body!");
			}
		}

		pline("The %s shapes your figure, but it isn't very practical to fight in.",
				OBJ_NAME(objects[uarmu->otyp]));
	}

	if(uarmu->otyp == RUFFLED_SHIRT){

		if ((Role_if(PM_NOBLEMAN) || Role_if(PM_NOBLEWOMAN) || Role_if(PM_ACTIVISTOR)) && !uarmu->cursed ) {if (yn_function("Do you want your noble shirt to stick?", ynchars, 'n') == 'y') { curse(uarmu);
			pline("Your shirt tickles comfortably on your smooth skin!");
			}
		}

	}

    return 0;
}

int
Shirt_off()
{
    takeoff_mask &= ~W_ARMU;
/*
    switch (uarmu->otyp) {
	case HAWAIIAN_SHIRT:
	case T_SHIRT:
		break;
	default: impossible(unknown_type, c_shirt, uarmu->otyp);
    }
*/
    setworn((struct obj *)0, W_ARMU);
    return 0;
}
#endif	/*TOURIST*/

/* This must be done in worn.c, because one of the possible intrinsics conferred
 * is fire resistance, and we have to immediately set HFire_resistance in worn.c
 * since worn.c will check it before returning.
 */
int
Armor_on()
{
	/* KMH -- certain armor is obvious when worn */
	switch (uarm->otyp) {
		case ROBE_OF_PROTECTION:
		case ROBE_OF_WEAKNESS:
			makeknown(uarm->otyp);
			break;
	}
    return 0;
}

int
Armor_off()
{
    takeoff_mask &= ~W_ARM;
    setworn((struct obj *)0, W_ARM);
    cancelled_don = FALSE;
    return 0;
}

/* The gone functions differ from the off functions in that if you die from
 * taking it off and have life saving, you still die.
 */
int
Armor_gone()
{
    takeoff_mask &= ~W_ARM;
    setnotworn(uarm);
    cancelled_don = FALSE;
    return 0;
}

void
Amulet_on()
{
    long oldprop;
	if (!uamul) return;
	oldprop = u.uprops[objects[uamul->otyp].oc_oprop].extrinsic & ~WORN_AMUL;

    switch(uamul->otyp) {
	case AMULET_OF_ESP:
#if 0	/* OBSOLETE */
		if(uamul->oartifact == ART_MEDALLION_OF_SHIFTERS) rescham();
#endif
	case AMULET_OF_LIFE_SAVING:
	case AMULET_VERSUS_POISON:
	case AMULET_OF_DRAIN_RESISTANCE:
	case AMULET_OF_REFLECTION:
	case AMULET_OF_DEPETRIFY:
	case AMULET_OF_MAGICAL_BREATHING:
	/* KMH, balance patch -- removed
	case AMULET_OF_REGENERATION:
	case AMULET_OF_CONFLICT:*/
	case FAKE_AMULET_OF_YENDOR:
	case AMULET_OF_SECOND_CHANCE:
		break;
	case AMULET_OF_UNDEAD_WARNING:
		break;

	case AMULET_OF_RMB_LOSS:
	case AMULET_OF_ITEM_TELEPORTATION:

		if (uamul && !uamul->cursed) curse(uamul);

		break;

	case AMULET_OF_UNCHANGING:
		if (Slimed) {
		    Slimed = 0;
		    flags.botl = 1;
		}
		break;
	case AMULET_OF_FUMBLING:
		if (!oldprop && !(HFumbling & ~TIMEOUT))
			incr_itimeout(&HFumbling, rnd(20));
		break;
	case AMULET_OF_CHANGE:
	    {
		int orig_sex = poly_gender();

		if (Unchanging) break;
		change_sex();
		/* Don't use same message as polymorph */
		if (orig_sex != poly_gender()) {
		    makeknown(AMULET_OF_CHANGE);
		    You("are suddenly very %s!", flags.female ? "feminine"
			: "masculine");
		    flags.botl = 1;
		} else
		    /* already polymorphed into single-gender monster; only
		       changed the character's base sex */
		    You("don't feel like yourself.");
		pline_The("amulet disintegrates!");
		if (orig_sex == poly_gender() && uamul->dknown &&
			!objects[AMULET_OF_CHANGE].oc_name_known &&
			!objects[AMULET_OF_CHANGE].oc_uname)
		    docall(uamul);
		useup(uamul);
		break;
	    }
	/* KMH, balance patch -- removed
	case AMULET_OF_POLYMORPH:        
		makeknown(AMULET_OF_POLYMORPH);
		You("feel rather strange.");
		polyself();
		flags.botl = 1;
		pline("The amulet disintegrates!");
		useup(uamul);
		break;*/
	case AMULET_OF_STRANGULATION:
		makeknown(AMULET_OF_STRANGULATION);
		pline("It constricts your throat!");
		Strangled = 6;
		break;
	case AMULET_OF_RESTFUL_SLEEP:
		if Race_if(PM_KOBOLT) break;
		if(uamul->blessed) {
			char buf[BUFSZ];
			int sleeptime;
  
			makeknown(AMULET_OF_RESTFUL_SLEEP);
			do {
			getlin("How many moves do you wish to sleep for? [1-500]", buf);
				    sleeptime = (!*buf || *buf=='\033') ? 0 : atoi(buf);
				} while (sleeptime < 1 || sleeptime > 500);
			if (sleeptime > 0) {
				You("sit down and fall asleep.");
				nomul(-sleeptime, "taking a refreshing nap");
				u.usleep = 1;
				nomovemsg = "You wake up from your refreshing nap.";
		HSleeping = rnd(1000);
			}                
		} else HSleeping = rnd(1000);
		break;
	case AMULET_OF_DATA_STORAGE:
		You("feel full of knowledge.");
		break;
	case AMULET_OF_YENDOR:
		break;
	/* KMH, balance patch -- added */
	case AMULET_VERSUS_STONE:
		uunstone();
		break;
	case AMULET_OF_FLYING:
		if (!(EFlying & ~W_AMUL) && !is_flyer(youmonst.data)) {
			You_feel("like flying!");
			if (!Levitation)
				float_up();
			makeknown(AMULET_OF_FLYING);
		}
		break;
    }
}

void
Amulet_off()
{
    int otyp = uamul->otyp;
    long oldprop = u.uprops[objects[otyp].oc_oprop].extrinsic & ~WORN_AMUL;
    takeoff_mask &= ~W_AMUL;

    switch(uamul->otyp) {
	case AMULET_OF_ESP:
		/* need to update ability before calling see_monsters() */
#if 0	/* OBSOLETE */
		if(uamul->oartifact == ART_MEDALLION_OF_SHIFTERS) restartcham();
#endif
		setworn((struct obj *)0, W_AMUL);
		see_monsters();
		return;
	case AMULET_OF_LIFE_SAVING:
	case AMULET_VERSUS_POISON:
	case AMULET_OF_DRAIN_RESISTANCE:
	/* KMH, balance patch -- removed
	case AMULET_OF_REGENERATION:
	case AMULET_OF_CONFLICT:*/
	/* KMH, balance patch -- added */
	case AMULET_VERSUS_STONE:
	case AMULET_OF_REFLECTION:
	case AMULET_OF_SECOND_CHANCE:
	case AMULET_OF_DEPETRIFY:
	case AMULET_OF_CHANGE:
	case AMULET_OF_UNCHANGING:
	case FAKE_AMULET_OF_YENDOR:
		break;
	case AMULET_OF_UNDEAD_WARNING:
		break;

	case AMULET_OF_FUMBLING:
		if (!oldprop && !(HFumbling & ~TIMEOUT))
			HFumbling = EFumbling = 0;
		break;
	case AMULET_OF_MAGICAL_BREATHING:
		if (Underwater) {
		    /* HMagical_breathing must be set off
			before calling drown() */
		    setworn((struct obj *)0, W_AMUL);
		    if (!breathless(youmonst.data) && !amphibious(youmonst.data)
						&& !Swimming) {
			You("suddenly inhale an unhealthy amount of water!");
		    	(void) drown();
		    }
		    return;
		}
		break;
	case AMULET_OF_STRANGULATION:
		if (Strangled) {
			You("can breathe more easily!");
			Strangled = 0;
		}
		break;
	case AMULET_OF_RESTFUL_SLEEP:
		setworn((struct obj *)0, W_AMUL);
		if (!ESleeping && !Race_if(PM_KOBOLT))
			HSleeping = 0;
		return;
	case AMULET_OF_DATA_STORAGE:
		You("feel intellectually poor.");
		break;
	/* KMH, balance patch -- added */
	case AMULET_OF_FLYING:
		setworn((struct obj *)0, W_AMUL);
		(void) float_down(0L, 0L);
		return;
	case AMULET_OF_YENDOR:
		break;
    }
    setworn((struct obj *)0, W_AMUL);
    return;
}

void
Ring_on(obj)
register struct obj *obj;
{
    long oldprop = u.uprops[objects[obj->otyp].oc_oprop].extrinsic;
    int old_attrib, which;

    if (obj == uwep) setuwep((struct obj *) 0, TRUE);
    if (obj == uswapwep) setuswapwep((struct obj *) 0, TRUE);
    if (obj == uquiver) setuqwep((struct obj *) 0);

    /* only mask out W_RING when we don't have both
       left and right rings of the same type */
    if ((oldprop & W_RING) != W_RING) oldprop &= ~W_RING;

    switch(obj->otyp){
	case RIN_TELEPORTATION:
	case RIN_REGENERATION:
	case RIN_SEARCHING:
	case RIN_STEALTH:
	case RIN_HUNGER:
	case RIN_AGGRAVATE_MONSTER:
	case RIN_POISON_RESISTANCE:
	case RIN_FIRE_RESISTANCE:
	case RIN_COLD_RESISTANCE:
	case RIN_FEAR_RESISTANCE:
	case RIN_SHOCK_RESISTANCE:
	case RIN_CONFLICT:
	case RIN_TELEPORT_CONTROL:
	case RIN_POLYMORPH:
	case RIN_POLYMORPH_CONTROL:
	/* KMH, balance patch -- now an amulet
	case RIN_DRAIN_RESISTANCE: */
	/* KMH -- added */
	case RIN_MOOD:
	case RIN_FREE_ACTION:                
	case RIN_SLOW_DIGESTION:
	case RIN_SUSTAIN_ABILITY:
	case MEAT_RING:
		break;
	case RIN_HALLUCINATION:
	case RIN_CURSE:

		if (!obj->cursed) {
		    if (Blind)
			pline("%s for a moment.", Tobjnam(obj, "vibrate"));
		    else
			pline("%s %s for a moment.",
			      Tobjnam(obj, "glow"), hcolor(NH_BLACK));
		    curse(obj);
		}

		break;

	case RIN_INTRINSIC_LOSS:
	case RIN_BLOOD_LOSS:
	case RIN_NASTINESS:
	case RIN_BAD_EFFECT:
	case RIN_SUPERSCROLLING:

		if (!obj->cursed) curse(obj);

		break;

	case RIN_SLEEPING:        
		if Race_if(PM_KOBOLT) break;
		HSleeping = rnd(1000);
		break;
#if 0
	case RIN_INDIGESTION:
		incr_itimeout(&HIndigestion, rnd(20));
		break;
#endif
	case RIN_WARNING:
		see_monsters();
		break;
	case RIN_SEE_INVISIBLE:
		/* can now see invisible monsters */
		set_mimic_blocking(); /* do special mimic handling */
		see_monsters();
#ifdef INVISIBLE_OBJECTS
		see_objects();
#endif

		if (Invis && !oldprop && !HSee_invisible &&
				!perceives(youmonst.data) && !Blind) {
		    newsym(u.ux,u.uy);
		    pline("Suddenly you are transparent, but there!");
		    makeknown(RIN_SEE_INVISIBLE);
		}
		break;
	case RIN_TIMELY_BACKUP:
		You("feel absolutely safe.");
		break;
	case RIN_INVISIBILITY:
		if (!oldprop && !HInvis && !BInvis && !Blind) {
		    makeknown(RIN_INVISIBILITY);
		    newsym(u.ux,u.uy);
		    self_invis_message();
		}
		break;
	case RIN_LEVITATION:
		if (!oldprop && !HLevitation) {
		    flags.botl = 1;
		    float_up();
		    makeknown(RIN_LEVITATION);
		    spoteffects(FALSE);	/* for sinks */
		}
		break;
	case RIN_GAIN_STRENGTH:
		which = A_STR;
		goto adjust_attrib;
	case RIN_GAIN_CONSTITUTION:
		which = A_CON;
		goto adjust_attrib;
	case RIN_ADORNMENT:
		which = A_CHA;
 adjust_attrib:
		old_attrib = ACURR(which);
		ABON(which) += obj->spe;
		if (ACURR(which) != old_attrib ||
			(objects[obj->otyp].oc_name_known &&
			    old_attrib != 25 && old_attrib != 3)) {
		    flags.botl = 1;
		    makeknown(obj->otyp);
		    obj->known = 1;
		    update_inventory();
		}
		break;
	case RIN_GAIN_INTELLIGENCE:
		ABON(A_INT) += obj->spe;
		flags.botl = 1;
		if (obj->spe || objects[RIN_GAIN_INTELLIGENCE].oc_name_known) {
			makeknown(RIN_GAIN_INTELLIGENCE);
			obj->known = TRUE;
		}
		break;
	case RIN_GAIN_WISDOM:
		ABON(A_WIS) += obj->spe;
		flags.botl = 1;
		if (obj->spe || objects[RIN_GAIN_WISDOM].oc_name_known) {
			makeknown(RIN_GAIN_WISDOM);
			obj->known = TRUE;
		}
		break;
	case RIN_GAIN_DEXTERITY:
		ABON(A_DEX) += obj->spe;
		flags.botl = 1;
		if (obj->spe || objects[RIN_GAIN_DEXTERITY].oc_name_known) {
			makeknown(RIN_GAIN_DEXTERITY);
			obj->known = TRUE;
		}
		break;
	case RIN_INCREASE_ACCURACY:	/* KMH */
		u.uhitinc += obj->spe;
		break;
	case RIN_INCREASE_DAMAGE:
		u.udaminc += obj->spe;
		break;
	case RIN_PROTECTION_FROM_SHAPE_CHAN:
		rescham();
		break;
	case RIN_PROTECTION:
		if (obj->spe || objects[RIN_PROTECTION].oc_name_known) {
		    flags.botl = 1;
		    makeknown(RIN_PROTECTION);
		    obj->known = 1;
		    update_inventory();
		}
		break;
    }
}

STATIC_OVL void
Ring_off_or_gone(obj,gone)
register struct obj *obj;
boolean gone;
{
    long mask = (obj->owornmask & W_RING);
    int old_attrib, which;

    takeoff_mask &= ~mask;
    if(!(u.uprops[objects[obj->otyp].oc_oprop].extrinsic & mask))
	impossible("Strange... I didn't know you had that ring.");
    if(gone) setnotworn(obj);
    else setworn((struct obj *)0, obj->owornmask);

    switch(obj->otyp) {
	case RIN_TELEPORTATION:
	case RIN_REGENERATION:
	case RIN_SEARCHING:
	case RIN_STEALTH:
	case RIN_HUNGER:
	case RIN_AGGRAVATE_MONSTER:
	case RIN_POISON_RESISTANCE:
	case RIN_FIRE_RESISTANCE:
	case RIN_COLD_RESISTANCE:
	case RIN_FEAR_RESISTANCE:
	case RIN_SHOCK_RESISTANCE:
	case RIN_CONFLICT:
	case RIN_TELEPORT_CONTROL:
	case RIN_POLYMORPH:
	case RIN_POLYMORPH_CONTROL:
	/* KMH, balance patch -- now an amulet
	case RIN_DRAIN_RESISTANCE: */
	/* KMH, balance patch -- added */
	case RIN_MOOD:
	case RIN_FREE_ACTION:                
	case RIN_SLOW_DIGESTION:
	case RIN_SUSTAIN_ABILITY:
	case MEAT_RING:
		break;
	case RIN_SLEEPING:
		if (!ESleeping && !Race_if(PM_KOBOLT))
			HSleeping = 0;
		break;
#if 0
	case RIN_INDIGESTION:
		if (!EIndigestion)
			HIndigestion = 0;
		break;
#endif
	case RIN_TIMELY_BACKUP:
		You("feel unsafe.");
		break;
	case RIN_WARNING:
		see_monsters();
		break;
	case RIN_SEE_INVISIBLE:
		/* Make invisible monsters go away */
		if (!See_invisible) {
		    set_mimic_blocking(); /* do special mimic handling */
		    see_monsters();
#ifdef INVISIBLE_OBJECTS                
		    see_objects();
#endif
		}

		if (Invisible && !Blind) {
		    newsym(u.ux,u.uy);
		    pline("Suddenly you cannot see yourself.");
		    makeknown(RIN_SEE_INVISIBLE);
		}
		break;
	case RIN_INVISIBILITY:
		if (!Invis && !BInvis && !Blind) {
		    newsym(u.ux,u.uy);
		    Your("body seems to unfade%s.",
			 See_invisible ? " completely" : "..");
		    makeknown(RIN_INVISIBILITY);
		}
		break;
	case RIN_LEVITATION:
		flags.botl = 1;
		(void) float_down(0L, 0L);
		if (!Levitation) makeknown(RIN_LEVITATION);
		break;
	case RIN_GAIN_STRENGTH:
		which = A_STR;
		goto adjust_attrib;
	case RIN_GAIN_INTELLIGENCE:
		which = A_INT;
		goto adjust_attrib;
	case RIN_GAIN_WISDOM:
		which = A_WIS;
		goto adjust_attrib;
	case RIN_GAIN_DEXTERITY:
		which = A_DEX;
		goto adjust_attrib;
	case RIN_GAIN_CONSTITUTION:
		which = A_CON;
		goto adjust_attrib;
	case RIN_ADORNMENT:
		which = A_CHA;
 adjust_attrib:
		old_attrib = ACURR(which);
		ABON(which) -= obj->spe;
		if (ACURR(which) != old_attrib) {
		    flags.botl = 1;
		    makeknown(obj->otyp);
		    obj->known = 1;
		    update_inventory();
		}
		break;
	case RIN_INCREASE_ACCURACY:	/* KMH */
		u.uhitinc -= obj->spe;
		break;
	case RIN_INCREASE_DAMAGE:
		u.udaminc -= obj->spe;
		break;
	case RIN_PROTECTION:
		/* might have forgotten it due to amnesia */
		if (obj->spe) {
		    flags.botl = 1;
		    makeknown(RIN_PROTECTION);
		    obj->known = 1;
		    update_inventory();
		}
	case RIN_PROTECTION_FROM_SHAPE_CHAN:
		/* If you're no longer protected, let the chameleons
		 * change shape again -dgk
		 */
		restartcham();
		break;
    }
}

void
Ring_off(obj)
struct obj *obj;
{
	Ring_off_or_gone(obj,FALSE);
}

void
Ring_gone(obj)
struct obj *obj;
{
	Ring_off_or_gone(obj,TRUE);
}

void
Blindf_on(otmp)
register struct obj *otmp;
{
	long already_blind = Blind, changed = FALSE;

	if (otmp == uwep)
	    setuwep((struct obj *) 0, TRUE);
	if (otmp == uswapwep)
	    setuswapwep((struct obj *) 0, TRUE);
	if (otmp == uquiver)
	    setuqwep((struct obj *) 0);
	setworn(otmp, W_TOOL);
	on_msg(otmp);

	if (Blind && !already_blind) {
	    changed = TRUE;
	    if (flags.verbose) You_cant("see any more.");
	    /* set ball&chain variables before the hero goes blind */
	    if (Punished) set_bc(0);
	} else if (already_blind && !Blind) {
	    changed = TRUE;
	    /* "You are now wearing the Eyes of the Overworld." */
	    You("can see!");
	}
	if (changed) {
	    /* blindness has just been toggled */
	    if (Blind_telepat || Infravision) see_monsters();
	    vision_full_recalc = 1;	/* recalc vision limits */
	    flags.botl = 1;
	}
}

void
Blindf_off(otmp)
register struct obj *otmp;
{
	boolean was_blind = Blind, changed = FALSE;

	takeoff_mask &= ~W_TOOL;
	setworn((struct obj *)0, otmp->owornmask);
	off_msg(otmp);

	if (Blind) {
	    if (was_blind) {
		/* "still cannot see" makes no sense when removing lenses
		   since they can't have been the cause of your blindness */
		if (otmp->otyp != LENSES)
		    You("still cannot see.");
	    } else {
		changed = TRUE;	/* !was_blind */
		/* "You were wearing the Eyes of the Overworld." */
		You_cant("see anything now!");
		/* set ball&chain variables before the hero goes blind */
		if (Punished) set_bc(0);
	    }
	} else if (was_blind) {
	    changed = TRUE;	/* !Blind */
	    You("can see again.");
	}
	if (changed) {
	    /* blindness has just been toggled */
	    if (Blind_telepat || Infravision) see_monsters();
	    vision_full_recalc = 1;	/* recalc vision limits */
	    flags.botl = 1;
	}
}


/* called in main to set intrinsics of worn start-up items */
void
set_wear()
{
#ifdef TOURIST
	if (uarmu) (void) Shirt_on();
#endif
	if (uarm)  (void) Armor_on();
	if (uarmc) (void) Cloak_on();
	if (uarmf) (void) Boots_on();
	if (uarmg) (void) Gloves_on();
	if (uarmh) (void) Helmet_on();
	if (uarms) (void) Shield_on();
}

/* check whether the target object is currently being put on (or taken off) */
boolean
donning(otmp)		/* also checks for doffing */
register struct obj *otmp;
{
 /* long what = (occupation == take_off) ? taking_off : 0L; */
    long what = taking_off;	/* if nonzero, occupation is implied */
    boolean result = FALSE;

    if (otmp == uarm)
	result = (afternmv == Armor_on || afternmv == Armor_off ||
		  what == WORN_ARMOR);
#ifdef TOURIST
    else if (otmp == uarmu)
	result = (afternmv == Shirt_on || afternmv == Shirt_off ||
		  what == WORN_SHIRT);
#endif
    else if (otmp == uarmc)
	result = (afternmv == Cloak_on || afternmv == Cloak_off ||
		  what == WORN_CLOAK);
    else if (otmp == uarmf)
	result = (afternmv == Boots_on || afternmv == Boots_off ||
		  what == WORN_BOOTS);
    else if (otmp == uarmh)
	result = (afternmv == Helmet_on || afternmv == Helmet_off ||
		  what == WORN_HELMET);
    else if (otmp == uarmg)
	result = (afternmv == Gloves_on || afternmv == Gloves_off ||
		  what == WORN_GLOVES);
    else if (otmp == uarms)
	result = (afternmv == Shield_on || afternmv == Shield_off ||
		  what == WORN_SHIELD);

    return result;
}

void
cancel_don()
{
	/* the piece of armor we were donning/doffing has vanished, so stop
	 * wasting time on it (and don't dereference it when donning would
	 * otherwise finish)
	 */
	cancelled_don = (afternmv == Boots_on || afternmv == Helmet_on ||
			 afternmv == Gloves_on || afternmv == Armor_on);
	afternmv = 0;
	nomovemsg = (char *)0;
	multi = 0;
	todelay = 0;
	taking_off = 0L;
}

static NEARDATA const char clothes[] = {ARMOR_CLASS, 0};
static NEARDATA const char accessories[] = {RING_CLASS, AMULET_CLASS, TOOL_CLASS, FOOD_CLASS, 0};

/* the 'T' command */
int
dotakeoff()
{
	register struct obj *otmp = (struct obj *)0;
	int armorpieces = 0;

#define MOREARM(x) if (x) { armorpieces++; otmp = x; }
	MOREARM(uarmh);
	MOREARM(uarms);
	MOREARM(uarmg);
	MOREARM(uarmf);
	if (uarmc) {
		armorpieces++;
		otmp = uarmc;
	} else if (uarm) {
		armorpieces++;
		otmp = uarm;
#ifdef TOURIST
	} else if (uarmu) {
		armorpieces++;
		otmp = uarmu;
#endif
	}
	if (!armorpieces) {
	     /* assert( GRAY_DRAGON_SCALES > YELLOW_DRAGON_SCALE_MAIL ); */
		if (uskin)
		    pline_The("%s merged with your skin!",
			      uskin->otyp >= GRAY_DRAGON_SCALES ?
				"dragon scales are" : "dragon scale mail is");
		else
		    pline("Not wearing any armor.%s", (iflags.cmdassist && 
				(uleft || uright || uamul || ublindf)) ?
			  "  Use 'R' command to remove accessories." : "");
		return 0;
	}
	if (armorpieces > 0)
		otmp = getobj(clothes, "take off");
	if (otmp == 0) return(0);
	if (!(otmp->owornmask & W_ARMOR)) {
		You("are not wearing that.");
		return(0);
	}
	/* note: the `uskin' case shouldn't be able to happen here; dragons
	   can't wear any armor so will end up with `armorpieces == 0' above */
	if (otmp == uskin || ((otmp == uarm) && uarmc)
#ifdef TOURIST
			  || ((otmp == uarmu) && (uarmc || uarm))
#endif
		) {
	    You_cant("take that off.");
	    return 0;
	}

	reset_remarm();		/* clear takeoff_mask and taking_off */
	(void) select_off(otmp);
	if (!takeoff_mask) return 0;
	reset_remarm();		/* armoroff() doesn't use takeoff_mask */

	(void) armoroff(otmp);
	return(1);
}

/* the 'R' command */
int
doremring()
{
	register struct obj *otmp = 0;
	int Accessories = 0;

#define MOREACC(x) if (x) { Accessories++; otmp = x; }
	MOREACC(uleft);
	MOREACC(uright);
	MOREACC(uamul);
	MOREACC(ublindf);

	if(!Accessories) {
		pline("Not wearing any accessories.%s", (iflags.cmdassist &&
			    (uarm || uarmc ||
#ifdef TOURIST
			     uarmu ||
#endif
			     uarms || uarmh || uarmg || uarmf)) ?
		      "  Use 'T' command to take off armor." : "");
		return(0);
	}
	if (Accessories > 0) otmp = getobj(accessories, "remove");
	if(!otmp) return(0);
	if(!(otmp->owornmask & (W_RING | W_AMUL | W_TOOL))) {
		You("are not wearing that.");
		return(0);
	}

	reset_remarm();		/* clear takeoff_mask and taking_off */
	(void) select_off(otmp);
	if (!takeoff_mask) return 0;
	reset_remarm();		/* not used by Ring_/Amulet_/Blindf_off() */

	if (otmp == uright || otmp == uleft) {
		/* Sometimes we want to give the off_msg before removing and
		 * sometimes after; for instance, "you were wearing a moonstone
		 * ring (on right hand)" is desired but "you were wearing a
		 * square amulet (being worn)" is not because of the redundant
		 * "being worn".
		 */
		off_msg(otmp);
		Ring_off(otmp);
	} else if (otmp == uamul) {
		Amulet_off();
		off_msg(otmp);
	} else if (otmp == ublindf) {
		Blindf_off(otmp);	/* does its own off_msg */
	} else {
		impossible("removing strange accessory?");
	}
	return(1);
}

/* Check if something worn is cursed _and_ unremovable. */
int
cursed(otmp)
register struct obj *otmp;
{
	/* Curses, like chickens, come home to roost. */
	if((otmp == uwep) ? welded(otmp) : (int)otmp->cursed) {
		Hallucination ? pline("You can't. It's cursed.") : You("can't.  %s cursed.",
			(is_boots(otmp) || is_gloves(otmp) || otmp->quan > 1L)
			? "They are" : "It is");
		otmp->bknown = TRUE;
		return(1);
	}
	return(0);
}

int
armoroff(otmp)
register struct obj *otmp;
{
	register int delay = -objects[otmp->otyp].oc_delay;

	if(cursed(otmp)) return(0);
	if(delay) {
		nomul(delay, "disrobing");
		if (is_helmet(otmp)) {
			nomovemsg = "You finish taking off your helmet.";
			afternmv = Helmet_off;
		     }
		else if (is_gloves(otmp)) {
			nomovemsg = "You finish taking off your gloves.";
			afternmv = Gloves_off;
		     }
		else if (is_boots(otmp)) {
			nomovemsg = "You finish taking off your boots.";
			afternmv = Boots_off;
		     }
		else if (otmp->otyp == VICTORIAN_UNDERWEAR) {
			nomovemsg = "You finish taking off your impractical underwear.";
			afternmv = Shirt_off;
		     }
		else {
			nomovemsg = "You finish taking off your suit.";
			afternmv = Armor_off;
		}
	} else {
		/* Be warned!  We want off_msg after removing the item to
		 * avoid "You were wearing ____ (being worn)."  However, an
		 * item which grants fire resistance might cause some trouble
		 * if removed in Hell and lifesaving puts it back on; in this
		 * case the message will be printed at the wrong time (after
		 * the messages saying you died and were lifesaved).  Luckily,
		 * no cloak, shield, or fast-removable armor grants fire
		 * resistance, so we can safely do the off_msg afterwards.
		 * Rings do grant fire resistance, but for rings we want the
		 * off_msg before removal anyway so there's no problem.  Take
		 * care in adding armors granting fire resistance; this code
		 * might need modification.
		 * 3.2 (actually 3.1 even): this comment is obsolete since
		 * fire resistance is not needed for Gehennom.
		 */
		if(is_cloak(otmp))
			(void) Cloak_off();
		else if(is_shield(otmp))
			(void) Shield_off();
		else if (is_helmet(otmp))
			/* [Finn E. Theodorsen] For fedoras */
			(void) Helmet_off();
		else setworn((struct obj *)0, otmp->owornmask & W_ARMOR);
		off_msg(otmp);
	}
	takeoff_mask = taking_off = 0L;
	return(1);
}

STATIC_OVL void
already_wearing(cc)
const char *cc;
{
	You("are already wearing %s%c", cc, (cc == c_that_) ? '!' : '.');
}

STATIC_OVL void
already_wearing2(cc1, cc2)
const char *cc1, *cc2;
{
	You_cant("wear %s because you're wearing %s there already.", cc1, cc2);
}

/*
 * canwearobj checks to see whether the player can wear a piece of armor
 *
 * inputs: otmp (the piece of armor)
 *         noisy (if TRUE give error messages, otherwise be quiet about it)
 * output: mask (otmp's armor type)
 */
int
canwearobj(otmp,mask,noisy)
struct obj *otmp;
long *mask;
boolean noisy;
{
    int err = 0;
    const char *which;

    which = is_cloak(otmp) ? c_cloak :
#ifdef TOURIST
	    is_shirt(otmp) ? c_shirt :
#endif
	    is_suit(otmp) ? c_suit : 0;
    if (which && cantweararm(youmonst.data) && !Race_if(PM_TRANSFORMER)  &&
	    /* same exception for cloaks as used in m_dowear() */
	    (which != c_cloak || youmonst.data->msize != MZ_SMALL) &&
	    (racial_exception(&youmonst, otmp) < 1)) {
	if (noisy) { pline_The("%s will not fit on your body.", which);

		if (yn("Try to put it on anyway?") == 'y') {
			if (rn2(2)) { 	u.ublesscnt += rnz(5);
			pline("Feeling uncomfortable, you decide to stop trying.");
		    return 0;}
			}

		else {return(0);}

		}


    } else if (otmp->owornmask & W_ARMOR) {
	if (noisy) already_wearing(c_that_);
	return 0;
    }

    if (welded(uwep) && bimanual(uwep) &&
	    (is_suit(otmp)
#ifdef TOURIST
			|| is_shirt(otmp)
#endif
	    )) {
	if (noisy)
	    You("cannot do that while holding your %s.",
		is_sword(uwep) ? c_sword : c_weapon);
	return 0;
    }

    if (is_helmet(otmp)) {
	if (uarmh) {
	    if (noisy) already_wearing(an(c_helmet));
	    err++;
	} else if (Upolyd && has_horns(youmonst.data) && !Race_if(PM_TRANSFORMER) && !is_flimsy(otmp)) {
	    /* (flimsy exception matches polyself handling) */
	    if (noisy)
		pline_The("%s won't fit over your horn%s.",
			  c_helmet, plur(num_horns(youmonst.data)));
	    err++;
	} else if (Role_if(PM_COURIER)) {
	    if (noisy)
		pline("You have no head!");
	    err++;
	/* Actually, you got shot in the head by Benny, so you aren't technically without a head. But for the sake of it,
	   let's pretend the courier actually doesn't have one. By the way, Benny is the "some random guy" quest nemesis
	   in the courier quest, since all Fallout New Vegas NPCs are undistinguishable clones of each other. --Amy */

	} else if (Race_if(PM_ILLITHID)) {
	    if (noisy)
		pline("Your tentacles prevent that action!");
	    err++;

#ifdef JEDI
	} else if (Upolyd && (youmonst.data == &mons[PM_MIND_FLAYER] ||
			      youmonst.data == &mons[PM_MASTER_MIND_FLAYER]) &&
			(otmp->otyp == PLASTEEL_HELM || otmp->otyp == HELM_OF_STORMS || otmp->otyp == HELM_OF_DETECT_MONSTERS) ){
		if (noisy)
			pline_The("%s won't fit over your tentacles.", xname(otmp));
		err++;
#endif
	} else
	    *mask = W_ARMH;
    } else if (is_shield(otmp)) {
	if (uarms) {
	    if (noisy) already_wearing(an(c_shield));
	    err++;
	} else if (uwep && bimanual(uwep)) {
	    if (noisy) 
		You("cannot wear a shield while wielding a two-handed %s.",
		    is_sword(uwep) ? c_sword :
		    (uwep->otyp == BATTLE_AXE) ? c_axe : c_weapon);
	    err++;
	} else if (u.twoweap) {
	    if (noisy) {
		if (uwep && uswapwep)
		    You("cannot wear a shield while wielding two weapons.");
		else
		    You("cannot wear a shield while fighting with two %s.",
			    makeplural(body_part(HAND)));
	    }
	    err++;
	} else
	    *mask = W_ARMS;
    } else if (is_boots(otmp)) {
	if (uarmf) {
	    if (noisy) already_wearing(c_boots);
	    err++;
	} else if (Upolyd && slithy(youmonst.data) && !Race_if(PM_TRANSFORMER) ) {
	    if (noisy) You("have no feet...");	/* not body_part(FOOT) */
	    err++;
	} else if (Upolyd && youmonst.data->mlet == S_CENTAUR) {
	    /* break_armor() pushes boots off for centaurs,
	       so don't let dowear() put them back on... */
	    if (noisy) pline("You have too many hooves to wear %s.",
			     c_boots);	/* makeplural(body_part(FOOT)) yields
					   "rear hooves" which sounds odd */
	    err++;
	} else if (u.utrap && (u.utraptype == TT_BEARTRAP ||
				u.utraptype == TT_INFLOOR)) {
	    if (u.utraptype == TT_BEARTRAP) {
		if (noisy) Your("%s is trapped!", body_part(FOOT));
	    } else {
		if (noisy) Your("%s are stuck in the %s!",
				makeplural(body_part(FOOT)),
				surface(u.ux, u.uy));
	    }
	    err++;
	} else
	    *mask = W_ARMF;
    } else if (is_gloves(otmp)) {
	if (uarmg) {
	    if (noisy) already_wearing(c_gloves);
	    err++;
	} else if (welded(uwep)) {
	    if (noisy) You("cannot wear gloves over your %s.",
			   is_sword(uwep) ? c_sword : c_weapon);
	    err++;
	} else
	    *mask = W_ARMG;
#ifdef TOURIST
    } else if (is_shirt(otmp)) {
	if (uarm || uarmc || uarmu) {
	    if (uarmu) {
		if (noisy) already_wearing(an(c_shirt));
	    } else {
		if (noisy) You_cant("wear that over your %s.",
			           (uarm && !uarmc) ? c_armor : cloak_simple_name(uarmc));
	    }
	    err++;
	} else
	    *mask = W_ARMU;
#endif
    } else if (is_cloak(otmp)) {
	if (uarmc) {
	    if (noisy) already_wearing(an(cloak_simple_name(uarmc)));
	    err++;
	} else
	    *mask = W_ARMC;
    } else if (is_suit(otmp)) {
	if (uarmc) {
	    if (noisy) You("cannot wear armor over a %s.", cloak_simple_name(uarmc));
	    err++;
	} else if (uarm) {
	    if (noisy) already_wearing("some armor");
	    err++;
	} else
	    *mask = W_ARM;
    } else {
	/* getobj can't do this after setting its allow_all flag; that
	   happens if you have armor for slots that are covered up or
	   extra armor for slots that are filled */
	if (noisy) silly_thing("wear", otmp);
	err++;
    }
/* Unnecessary since now only weapons and special items like pick-axes get
 * welded to your hand, not armor
    if (welded(otmp)) {
	if (!err++) {
	    if (noisy) weldmsg(otmp);
	}
    }
 */
    return !err;
}

/* the 'W' command */
int
dowear()
{
	struct obj *otmp;
	int delay;
	long mask = 0;

	/* cantweararm checks for suits of armor */
	/* verysmall or nohands checks for shields, gloves, etc... */
	if (!Race_if(PM_TRANSFORMER) && !Race_if(PM_HUMAN_WRAITH) && (verysmall(youmonst.data) || nohands(youmonst.data))) {
		pline("Don't even bother. Your current form can't realistically wear armor!");

		if (yn("But you may try to wear something anyway. Do it?") == 'y') {
			if (rn2(3)) { 		make_stunned(HStun + rnd(40),FALSE);
			pline("Damn! You just stagger around aimlessly!");
		    return 1;}
		}
		else {return(0);}

	}

	if (Race_if(PM_HUMAN_WRAITH) && (u.uhpmax < 2 || u.uhp < 2) ) {pline("You don't have enough health to wear armor!");
		display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return 0;
	}

	if (Race_if(PM_HUMAN_WRAITH)) {

		if (yn("Putting on armor as a wraith will permanently damage your health. Do it anyway?") == 'y') {

			u.uhp -= 1;
			u.uhpmax -= 1;
			if (Upolyd) {
				u.mh -= 1;
			u.mhmax -= 1;
			}

		}
		else return 0;

	}

	otmp = getobj(clothes, "wear");
	if(!otmp) return(0);

	if (!canwearobj(otmp,&mask,TRUE)) return(0);

	if (otmp->oartifact && !touch_artifact(otmp, &youmonst))
	    return 1;	/* costs a turn even though it didn't get worn */

	if (otmp->otyp == HELM_OF_OPPOSITE_ALIGNMENT &&
			qstart_level.dnum == u.uz.dnum) {	/* in quest */
		if (u.ualignbase[A_CURRENT] == u.ualignbase[A_ORIGINAL])
			You("narrowly avoid losing all chance at your goal.");
		else	/* converted */
			You("are suddenly overcome with shame and change your mind.");
		u.ublessed = 0; /* lose your god's protection */
		makeknown(otmp->otyp);
		flags.botl = 1;
		return 1;
	}

	otmp->known = TRUE;
	if(otmp == uwep)
		setuwep((struct obj *)0, TRUE);
	if (otmp == uswapwep)
		setuswapwep((struct obj *) 0, TRUE);
	if (otmp == uquiver)
		setuqwep((struct obj *) 0);
	setworn(otmp, mask);
	delay = -objects[otmp->otyp].oc_delay;
	if(delay){
		nomul(delay, "dressing up");
		if(is_boots(otmp)) afternmv = Boots_on;
		if(is_helmet(otmp)) afternmv = Helmet_on;
		if(is_gloves(otmp)) afternmv = Gloves_on;
		if(otmp == uarm) afternmv = Armor_on;
		if(is_cloak(otmp)) afternmv = Cloak_on;
		if (is_shield(otmp)) afternmv = Shield_on;
		if (is_shirt(otmp)) afternmv = Shirt_on;
		nomovemsg = "You finish your dressing maneuver.";
		on_msgdel(otmp); /* the game is supposed to tell you what exactly you are wearing! --Amy */
	} else {
		if(is_cloak(otmp)) (void) Cloak_on();
		if(is_boots(otmp)) (void) Boots_on();
		if(is_gloves(otmp)) (void) Gloves_on();
		if(otmp == uarm) (void) Armor_on();
		if (is_shield(otmp)) (void) Shield_on();
#ifdef TOURIST
		if (is_shirt(otmp)) (void) Shirt_on();
#endif
		if (is_helmet(otmp)) (void) Helmet_on();	/* fedoras */
		on_msg(otmp);
	}
	takeoff_mask = taking_off = 0L;
	return(1);
}

int
doputon()
{
	register struct obj *otmp;
	long mask = 0L;

	if(uleft && uright && uamul && ublindf) {
		Your("%s%s are full, and you're already wearing an amulet and %s.",
			humanoid(youmonst.data) ? "ring-" : "",
			makeplural(body_part(FINGER)),
			ublindf->otyp==LENSES ? "some lenses" : "a blindfold");
		return(0);
	}
	otmp = getobj(accessories, "put on");
	if(!otmp) return(0);
	if(otmp->owornmask & (W_RING | W_AMUL | W_TOOL)) {
		already_wearing(c_that_);
		return(0);
	}
	if(welded(otmp)) {
		weldmsg(otmp);
		return(0);
	}
	if(otmp == uwep)
		setuwep((struct obj *)0, TRUE);
	if(otmp == uswapwep)
		setuswapwep((struct obj *) 0, TRUE);
	if(otmp == uquiver)
		setuqwep((struct obj *) 0);
	if(otmp->oclass == RING_CLASS || otmp->otyp == MEAT_RING) {
		if(nolimbs(youmonst.data) && !Race_if(PM_TRANSFORMER) ) {
			You("cannot make the ring stick to your body.");
			return(0);
		}
		if(uleft && uright){
			There("are no more %s%s to fill.",
				humanoid(youmonst.data) ? "ring-" : "",
				makeplural(body_part(FINGER)));
			return(0);
		}
		if(uleft) mask = RIGHT_RING;
		else if(uright) mask = LEFT_RING;
		else do {
			char qbuf[QBUFSZ];
			char answer;

			Sprintf(qbuf, "Which %s%s, Right or Left?",
				humanoid(youmonst.data) ? "ring-" : "",
				body_part(FINGER));
			if(!(answer = yn_function(qbuf, "rl", '\0')))
				return(0);
			switch(answer){
			case 'l':
			case 'L':
				mask = LEFT_RING;
				break;
			case 'r':
			case 'R':
				mask = RIGHT_RING;
				break;
			}
		} while(!mask);
		if (uarmg && uarmg->cursed) {
			uarmg->bknown = TRUE;
		    You("cannot remove your gloves to put on the ring.");
			return(0);
		}
		if (welded(uwep) && bimanual(uwep)) {
			/* welded will set bknown */
	    You("cannot free your weapon hands to put on the ring.");
			return(0);
		}
		if (welded(uwep) && mask==RIGHT_RING) {
			/* welded will set bknown */
	    You("cannot free your weapon hand to put on the ring.");
			return(0);
		}
		if (otmp->oartifact && !touch_artifact(otmp, &youmonst))
		    return 1; /* costs a turn even though it didn't get worn */
		setworn(otmp, mask);
		Ring_on(otmp);
	} else if (otmp->oclass == AMULET_CLASS) {
		if(uamul) {
			already_wearing("an amulet");
			return(0);
		}
		if (otmp->oartifact && !touch_artifact(otmp, &youmonst))
		    return 1;
		setworn(otmp, W_AMUL);
		if (otmp->otyp == AMULET_OF_CHANGE) {
			Amulet_on();
			/* Don't do a prinv() since the amulet is now gone */
			return(1);
		}
		Amulet_on();
	} else {	/* it's a blindfold, towel, or lenses */
		if (ublindf) {
			if (ublindf->otyp == TOWEL)
				Your("%s is already covered by a towel.",
					body_part(FACE));
			else if (ublindf->otyp == BLINDFOLD) {
				if (otmp->otyp == LENSES)
					already_wearing2("lenses", "a blindfold");
				else
					already_wearing("a blindfold");
			} else if (ublindf->otyp == LENSES) {
				if (otmp->otyp == BLINDFOLD)
					already_wearing2("a blindfold", "some lenses");
				else
					already_wearing("some lenses");
			} else
				already_wearing(something); /* ??? */
			return(0);
		}
		if (otmp->otyp != BLINDFOLD && otmp->otyp != TOWEL && otmp->otyp != LENSES) {
			You_cant("wear that!");
			return(0);
		}
		if (otmp->oartifact && !touch_artifact(otmp, &youmonst))
		    return 1;
#ifdef JEDI
		if (uarmh && (uarmh->otyp == PLASTEEL_HELM || uarmh->otyp == HELM_OF_STORMS || uarmh->otyp == HELM_OF_DETECT_MONSTERS) ){
			pline("The %s covers your whole face. You need to remove it first.", xname(uarmh));
			display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
			return 1;
		}
#endif
		Blindf_on(otmp);
		return(1);
	}
	if (is_worn(otmp))
	    prinv((char *)0, otmp, 0L);
	return(1);
}

#endif /* OVLB */

#ifdef OVL0

/* Limits of uac (conveniently equal to the limits of an schar ;) */
#define UAC_MIN (-128)
#define UAC_LIM 127

void
find_ac()
{
	int uac = mons[u.umonnum].ac;

	if(uarm) uac -= ARM_BONUS(uarm);
	if(uarmc) uac -= ARM_BONUS(uarmc);
	if(uarmh) uac -= ARM_BONUS(uarmh);
	if(uarmf) uac -= ARM_BONUS(uarmf);
	if(uarms) uac -= ARM_BONUS(uarms);
	if(uarmg) uac -= ARM_BONUS(uarmg);
#ifdef TOURIST
	if(uarmu) uac -= ARM_BONUS(uarmu);
#endif
	if(uleft && uleft->otyp == RIN_PROTECTION) uac -= uleft->spe;
	if(uright && uright->otyp == RIN_PROTECTION) uac -= uright->spe;
	if (HProtection & INTRINSIC) uac -= u.ublessed;
	uac -= u.uspellprot;

/* STEPHEN WHITE'S NEW CODE */
	/* Dexterity now affects AC */
	if (ACURR(A_DEX) < 4) uac += 3;
	else if (ACURR(A_DEX) < 6) uac += 2;
	else if (ACURR(A_DEX) < 8) uac += 1;
	else if (ACURR(A_DEX) < 14) uac -= 0;
	else if (ACURR(A_DEX) < 21) uac -= ACURR(A_DEX)-14;
	else if (ACURR(A_DEX) < 22) uac -= 6;
	else if (ACURR(A_DEX) < 24) uac -= 7;
	else uac -= 8;

	if (Role_if(PM_MONK) && !uwep && (!uarm ||
		uarm->otyp==ROBE ||
		uarm->otyp==ROBE_OF_POWER ||
		uarm->otyp==ROBE_OF_WEAKNESS ||
		uarm->otyp==ROBE_OF_PROTECTION) && !uarms) {
/*WAC cap off the Monk's ac bonus to -11 */
            if (u.ulevel > 18) uac -= 11;
            else uac -= (u.ulevel / 2) + 2;
	}
	if (Race_if(PM_DOPPELGANGER) && !uarm) uac -= (u.ulevel / 4) + 1;
	if (Race_if(PM_HAXOR)) uac -= 2;
	if ((Race_if(PM_HUMAN_WEREWOLF) || Race_if(PM_AK_THIEF_IS_DEAD_) || Role_if(PM_LUNATIC)) && !uarm) uac -= (u.ulevel / 4) + 1;

	if (Race_if(PM_HUMAN_WRAITH)) uac -= u.ulevel;
	if (Race_if(PM_TURTLE)) uac -= u.ulevel;

	/* Harlow - make sure it doesn't wrap around ;) */
	uac = (uac < UAC_MIN ? UAC_MIN : (uac > UAC_LIM ? UAC_LIM : uac));
	
	/*Corsets suck*/
	if(uarmu && uarmu->otyp == VICTORIAN_UNDERWEAR){
		uac += 2; //flat penalty. Something in the code "corrects" ac values >10, this is a kludge.
	}

	/* A higher-level player will have a small AC boost to compensate for monsters being more dangerous. --Amy */
	if (u.ulevel >= 5) uac -= 1;
	if (u.ulevel >= 10) uac -= 1;
	if (u.ulevel >= 15) uac -= 1;
	if (u.ulevel >= 20) uac -= 1;
	if (u.ulevel >= 25) uac -= 1;
	if (u.ulevel >= 30) uac -= 1;

	/* After all, a couatl or archon can still hit a -40 AC player without any problems... */

	/* bonus for wearing racial armor */

	if (Race_if(PM_DWARF) && uarm && uarm->otyp == DWARVISH_MITHRIL_COAT) uac -= 1;
	if (Race_if(PM_DROW) && uarm && uarm->otyp == DARK_ELVEN_MITHRIL_COAT) uac -= 1;
	if ((Race_if(PM_ELF) || Role_if(PM_ELPH)) && uarm && uarm->otyp == ELVEN_MITHRIL_COAT) uac -= 1;
	if (Race_if(PM_GNOME) && uarm && uarm->otyp == GNOMISH_SUIT) uac -= 2;
	if (Race_if(PM_ORC) && uarm && (uarm->otyp == ORCISH_CHAIN_MAIL || uarm->otyp == ORCISH_RING_MAIL) ) uac -= 1;

	if (Race_if(PM_ORC) && uarmc && uarmc->otyp == ORCISH_CLOAK ) uac -= 1;
	if (Race_if(PM_DWARF) && uarmc && uarmc->otyp == DWARVISH_CLOAK ) uac -= 1;
	if ((Race_if(PM_ELF) || Role_if(PM_ELPH)) && uarmc && uarmc->otyp == ELVEN_CLOAK) uac -= 1;

	if ((Race_if(PM_ELF) || Role_if(PM_ELPH)) && uarmh && uarmh->otyp == ELVEN_LEATHER_HELM) uac -= 1;
	if (Race_if(PM_GNOME) && uarmh && uarmh->otyp == GNOMISH_HELM) uac -= 2;
	if (Race_if(PM_ORC) && uarmh && uarmh->otyp == ORCISH_HELM) uac -= 1;
	if (Race_if(PM_DWARF) && uarmh && uarmh->otyp == DWARVISH_IRON_HELM) uac -= 1;

	if ((Race_if(PM_ELF) || Role_if(PM_ELPH)) && uarms && uarms->otyp == ELVEN_SHIELD) uac -= 1;
	if (Race_if(PM_ORC) && uarms && (uarms->otyp == ORCISH_SHIELD || uarms->otyp == URUK_HAI_SHIELD) ) uac -= 1;
	if (Race_if(PM_DWARF) && uarms && uarms->otyp == DWARVISH_ROUNDSHIELD) uac -= 1;

	if (Race_if(PM_GNOME) && uarmf && uarmf->otyp == GNOMISH_BOOTS) uac -= 2;

	if (u.artifactprotection) uac -= 2;

	if(uac != u.uac){
		u.uac = uac;
		flags.botl = 1;
	}
}

#endif /* OVL0 */
#ifdef OVLB

void
glibr()
{
	register struct obj *otmp;
	int xfl = 0;
	boolean leftfall, rightfall;
	const char *otherwep = 0;

	leftfall = (uleft && !uleft->cursed &&
		    (!uwep || !welded(uwep) || !bimanual(uwep)));
	rightfall = (uright && !uright->cursed && (!welded(uwep)));
	if (!uarmg && (leftfall || rightfall) && !nolimbs(youmonst.data)) {
		/* changed so cursed rings don't fall off, GAN 10/30/86 */
		Your("%s off your %s.",
			(leftfall && rightfall) ? "rings slip" : "ring slips",
			(leftfall && rightfall) ? makeplural(body_part(FINGER)) :
			body_part(FINGER));
		xfl++;
		if (leftfall) {
			otmp = uleft;
			Ring_off(uleft);
			dropx(otmp);
		}
		if (rightfall) {
			otmp = uright;
			Ring_off(uright);
			dropx(otmp);
		}
	}

	otmp = uswapwep;
	if (u.twoweap && otmp) {
		otherwep = is_sword(otmp) ? c_sword :
		    makesingular(oclass_names[(int)otmp->oclass]);
		Your("%s %sslips from your %s.",
			otherwep,
			xfl ? "also " : "",
			makeplural(body_part(HAND)));
		setuswapwep((struct obj *)0, FALSE);
		xfl++;
		if ( (otmp->otyp != LOADSTONE && otmp->otyp != HEALTHSTONE && otmp->otyp != LUCKSTONE && otmp->otyp != MANASTONE && otmp->otyp != SLEEPSTONE && otmp->otyp != LOADBOULDER && otmp->otyp != STONE_OF_MAGIC_RESISTANCE && !is_nastygraystone(otmp) ) || !otmp->cursed)
			dropx(otmp);
	}
	otmp = uwep;
	if (otmp && !welded(otmp)) {
		const char *thiswep;

		/* nice wording if both weapons are the same type */
		thiswep = is_sword(otmp) ? c_sword :
		    makesingular(oclass_names[(int)otmp->oclass]);
		if (otherwep && strcmp(thiswep, otherwep)) otherwep = 0;

		/* changed so cursed weapons don't fall, GAN 10/30/86 */
		Your("%s%s %sslips from your %s.",
			otherwep ? "other " : "", thiswep,
			xfl ? "also " : "",
			makeplural(body_part(HAND)));
		setuwep((struct obj *)0, FALSE);
		if ( (otmp->otyp != LOADSTONE && otmp->otyp != HEALTHSTONE && otmp->otyp != LUCKSTONE && otmp->otyp != MANASTONE && otmp->otyp != SLEEPSTONE && otmp->otyp != LOADBOULDER && otmp->otyp != STONE_OF_MAGIC_RESISTANCE && !is_nastygraystone(otmp) ) || !otmp->cursed)
			dropx(otmp);
	}
}

struct obj *
some_armor(victim)
struct monst *victim;
{
	register struct obj *otmph, *otmp;

	otmph = (victim == &youmonst) ? uarmc : which_armor(victim, W_ARMC);
	if (!otmph)
	    otmph = (victim == &youmonst) ? uarm : which_armor(victim, W_ARM);
#ifdef TOURIST
	if (!otmph)
	    otmph = (victim == &youmonst) ? uarmu : which_armor(victim, W_ARMU);
#endif
	
	otmp = (victim == &youmonst) ? uarmh : which_armor(victim, W_ARMH);
	if(otmp && (!otmph || !rn2(4))) otmph = otmp;
	otmp = (victim == &youmonst) ? uarmg : which_armor(victim, W_ARMG);
	if(otmp && (!otmph || !rn2(4))) otmph = otmp;
	otmp = (victim == &youmonst) ? uarmf : which_armor(victim, W_ARMF);
	if(otmp && (!otmph || !rn2(4))) otmph = otmp;
	otmp = (victim == &youmonst) ? uarms : which_armor(victim, W_ARMS);
	if(otmp && (!otmph || !rn2(4))) otmph = otmp;
	return(otmph);
}

/* erode some arbitrary armor worn by the victim */
void
erode_armor(victim, acid_dmg)
struct monst *victim;
boolean acid_dmg;
{
	struct obj *otmph = some_armor(victim);

	if (otmph && (otmph != uarmf)) {
	    erode_obj(otmph, acid_dmg, FALSE);
	    if (carried(otmph)) update_inventory();
	}
}

/* used for praying to check and fix levitation trouble */
struct obj *
stuck_ring(ring, otyp)
struct obj *ring;
int otyp;
{
    if (ring != uleft && ring != uright) {
	impossible("stuck_ring: neither left nor right?");
	return (struct obj *)0;
    }

    if (ring && ring->otyp == otyp) {
	/* reasons ring can't be removed match those checked by select_off();
	   limbless case has extra checks because ordinarily it's temporary */
	if (nolimbs(youmonst.data) &&
		uamul && uamul->otyp == AMULET_OF_UNCHANGING && uamul->cursed)
	    return uamul;
	if (welded(uwep) && (ring == uright || bimanual(uwep))) return uwep;
	if (uarmg && uarmg->cursed) return uarmg;
	if (ring->cursed) return ring;
    }
    /* either no ring or not right type or nothing prevents its removal */
    return (struct obj *)0;
}

/* also for praying; find worn item that confers "Unchanging" attribute */
struct obj *
unchanger()
{
    if (uamul && uamul->otyp == AMULET_OF_UNCHANGING) return uamul;
    return 0;
}

/* occupation callback for 'A' */
STATIC_PTR
int
select_off(otmp)
register struct obj *otmp;
{
	struct obj *why;
	char buf[BUFSZ];

	if (!otmp) return 0;
	*buf = '\0';			/* lint suppresion */

	/* special ring checks */
	if (otmp == uright || otmp == uleft) {
	    if (nolimbs(youmonst.data) && !Race_if(PM_TRANSFORMER) ) {
		pline_The("ring is stuck.");
		return 0;
	    }
	    why = 0;	/* the item which prevents ring removal */
	    if (welded(uwep) && (otmp == uright || bimanual(uwep))) {
		Sprintf(buf, "free a weapon %s", body_part(HAND));
		why = uwep;
	    } else if (uarmg && uarmg->cursed) {
		Sprintf(buf, "take off your %s", c_gloves);
		why = uarmg;
	    }
	    if (why) {
		You("cannot %s to remove the ring.", buf);
		why->bknown = TRUE;
		return 0;
	    }
	}
	/* special glove checks */
	if (otmp == uarmg) {
	    if (welded(uwep)) {
		You("are unable to take off your %s while wielding that %s.",
		    c_gloves, is_sword(uwep) ? c_sword : c_weapon);
		uwep->bknown = TRUE;
		return 0;
	    } else if (IsGlib) {
		You_cant("take off the slippery %s with your slippery %s.",
			 c_gloves, makeplural(body_part(FINGER)));
		return 0;
	    }
	}
	/* special boot checks */
	if (otmp == uarmf) {
	    if (u.utrap && u.utraptype == TT_BEARTRAP) {
		pline_The("bear trap prevents you from pulling your %s out.",
			  body_part(FOOT));
		return 0;
	    } else if (u.utrap && u.utraptype == TT_INFLOOR) {
		You("are stuck in the %s, and cannot pull your %s out.",
		    surface(u.ux, u.uy), makeplural(body_part(FOOT)));
		return 0;
	    }
	}
	/* special suit and shirt checks */
	if (otmp == uarm
#ifdef TOURIST
			|| otmp == uarmu
#endif
		) {
	    why = 0;	/* the item which prevents disrobing */
	    if (uarmc && uarmc->cursed) {
		Sprintf(buf, "remove your %s", cloak_simple_name(uarmc));
		why = uarmc;
#ifdef TOURIST
	    } else if (otmp == uarmu && uarm && uarm->cursed) {
		Sprintf(buf, "remove your %s", c_suit);
		why = uarm;
#endif
	    } else if (welded(uwep) && bimanual(uwep)) {
		Sprintf(buf, "release your %s",
			is_sword(uwep) ? c_sword :
			(uwep->otyp == BATTLE_AXE) ? c_axe : c_weapon);
		why = uwep;
	    }
	    if (why) {
		You("cannot %s to take off %s.", buf, the(xname(otmp)));
		why->bknown = TRUE;
		return 0;
	    }
	}
	/* basic curse check */
	if (otmp == uquiver || (otmp == uswapwep && !u.twoweap)) {
	    ;	/* some items can be removed even when cursed */
	} else {
	    /* otherwise, this is fundamental */
	    if (cursed(otmp)) return 0;
	}

	if(otmp == uarm) takeoff_mask |= WORN_ARMOR;
	else if(otmp == uarmc) takeoff_mask |= WORN_CLOAK;
	else if(otmp == uarmf) takeoff_mask |= WORN_BOOTS;
	else if(otmp == uarmg) takeoff_mask |= WORN_GLOVES;
	else if(otmp == uarmh) takeoff_mask |= WORN_HELMET;
	else if(otmp == uarms) takeoff_mask |= WORN_SHIELD;
#ifdef TOURIST
	else if(otmp == uarmu) takeoff_mask |= WORN_SHIRT;
#endif
	else if(otmp == uleft) takeoff_mask |= LEFT_RING;
	else if(otmp == uright) takeoff_mask |= RIGHT_RING;
	else if(otmp == uamul) takeoff_mask |= WORN_AMUL;
	else if(otmp == ublindf) takeoff_mask |= WORN_BLINDF;
	else if(otmp == uwep) takeoff_mask |= W_WEP;
	else if(otmp == uswapwep) takeoff_mask |= W_SWAPWEP;
	else if(otmp == uquiver) takeoff_mask |= W_QUIVER;

	else impossible("select_off: %s???", doname(otmp));

	return(0);
}

STATIC_OVL struct obj *
do_takeoff()
{
	register struct obj *otmp = (struct obj *)0;

	if (taking_off == W_WEP) {
	  if(!cursed(uwep)) {
	    setuwep((struct obj *) 0, TRUE);
	    You("are empty %s.", body_part(HANDED));
	    u.twoweap = FALSE;
	  }
	} else if (taking_off == W_SWAPWEP) {
	  setuswapwep((struct obj *) 0, TRUE);
	  You("no longer have a second weapon readied.");
	  u.twoweap = FALSE;
	} else if (taking_off == W_QUIVER) {
	  setuqwep((struct obj *) 0);
	  You("no longer have ammunition readied.");
	} else if (taking_off == WORN_ARMOR) {
	  otmp = uarm;
	  if(!cursed(otmp)) (void) Armor_off();
	} else if (taking_off == WORN_CLOAK) {
	  otmp = uarmc;
	  if(!cursed(otmp)) (void) Cloak_off();
	} else if (taking_off == WORN_BOOTS) {
	  otmp = uarmf;
	  if(!cursed(otmp)) (void) Boots_off();
	} else if (taking_off == WORN_GLOVES) {
	  otmp = uarmg;
	  if(!cursed(otmp)) (void) Gloves_off();
	} else if (taking_off == WORN_HELMET) {
	  otmp = uarmh;
	  if(!cursed(otmp)) (void) Helmet_off();
	} else if (taking_off == WORN_SHIELD) {
	  otmp = uarms;
	  if(!cursed(otmp)) (void) Shield_off();
#ifdef TOURIST
	} else if (taking_off == WORN_SHIRT) {
	  otmp = uarmu;
	  if (!cursed(otmp)) (void) Shirt_off();
#endif
	} else if (taking_off == WORN_AMUL) {
	  otmp = uamul;
	  if(!cursed(otmp)) Amulet_off();
	} else if (taking_off == LEFT_RING) {
	  otmp = uleft;
	  if(!cursed(otmp)) Ring_off(uleft);
	} else if (taking_off == RIGHT_RING) {
	  otmp = uright;
	  if(!cursed(otmp)) Ring_off(uright);
	} else if (taking_off == WORN_BLINDF) {
	  if (!cursed(ublindf)) Blindf_off(ublindf);
	} else impossible("do_takeoff: taking off %lx", taking_off);

	return(otmp);
}

static const char *disrobing = "";

STATIC_PTR
int
take_off()
{
	register int i;
	register struct obj *otmp;

	if (taking_off) {
	    if (todelay > 0) {
		todelay--;
		return(1);	/* still busy */
	    } else {
		if ((otmp = do_takeoff())) off_msg(otmp);
	    }
	    takeoff_mask &= ~taking_off;
	    taking_off = 0L;
	}

	for(i = 0; takeoff_order[i]; i++)
	    if(takeoff_mask & takeoff_order[i]) {
		taking_off = takeoff_order[i];
		break;
	    }

	otmp = (struct obj *) 0;
	todelay = 0;

	if (taking_off == 0L) {
	  You("finish %s.", disrobing);
	  return 0;
	} else if (taking_off == W_WEP) {
	  todelay = 1;
	} else if (taking_off == W_SWAPWEP) {
	  todelay = 1;
	} else if (taking_off == W_QUIVER) {
	  todelay = 1;
	} else if (taking_off == WORN_ARMOR) {
	  otmp = uarm;
	  /* If a cloak is being worn, add the time to take it off and put
	   * it back on again.  Kludge alert! since that time is 0 for all
	   * known cloaks, add 1 so that it actually matters...
	   */
	  if (uarmc) todelay += 2 * objects[uarmc->otyp].oc_delay + 1;
	} else if (taking_off == WORN_CLOAK) {
	  otmp = uarmc;
	} else if (taking_off == WORN_BOOTS) {
	  otmp = uarmf;
	} else if (taking_off == WORN_GLOVES) {
	  otmp = uarmg;
	} else if (taking_off == WORN_HELMET) {
	  otmp = uarmh;
	} else if (taking_off == WORN_SHIELD) {
	  otmp = uarms;
#ifdef TOURIST
	} else if (taking_off == WORN_SHIRT) {
	  otmp = uarmu;
	  /* add the time to take off and put back on armor and/or cloak */
	  if (uarm)  todelay += 2 * objects[uarm->otyp].oc_delay;
	  if (uarmc) todelay += 2 * objects[uarmc->otyp].oc_delay + 1;
#endif
	} else if (taking_off == WORN_AMUL) {
	  todelay = 1;
	} else if (taking_off == LEFT_RING) {
	  todelay = 1;
	} else if (taking_off == RIGHT_RING) {
	  todelay = 1;
	} else if (taking_off == WORN_BLINDF) {
	  todelay = 2;
	} else {
	  impossible("take_off: taking off %lx", taking_off);
	  return 0;	/* force done */
	}

	if (otmp) todelay += objects[otmp->otyp].oc_delay;

	/* Since setting the occupation now starts the counter next move, that
	 * would always produce a delay 1 too big per item unless we subtract
	 * 1 here to account for it.
	 */
	if (todelay > 0) todelay--;

	set_occupation(take_off, disrobing, 0);
	return(1);		/* get busy */
}

/* clear saved context to avoid inappropriate resumption of interrupted 'A' */
void
reset_remarm()
{
	taking_off = takeoff_mask = 0L;
	disrobing = nul;
}

/* the 'A' command -- remove multiple worn items */
int
doddoremarm()
{
    int result = 0;

    if (taking_off || takeoff_mask) {
	You("continue %s.", disrobing);
	set_occupation(take_off, disrobing, 0);
	return 0;
    } else if (!uwep && !uswapwep && !uquiver && !uamul && !ublindf &&
		!uleft && !uright && !wearing_armor()) {
	You("are not wearing anything.");
	return 0;
    }

    add_valid_menu_class(0); /* reset */
    if (flags.menu_style != MENU_TRADITIONAL ||
	    (result = ggetobj("take off", select_off, 0, FALSE, (unsigned *)0)) < -1)
	result = menu_remarm(result);

    if (takeoff_mask) {
	/* default activity for armor and/or accessories,
	   possibly combined with weapons */
	disrobing = "disrobing";
	/* specific activity when handling weapons only */
	if (!(takeoff_mask & ~(W_WEP|W_SWAPWEP|W_QUIVER)))
	    disrobing = "disarming";
	(void) take_off();
    }
    /* The time to perform the command is already completely accounted for
     * in take_off(); if we return 1, that would add an extra turn to each
     * disrobe.
     */
    return 0;
}

STATIC_OVL int
menu_remarm(retry)
int retry;
{
    int n, i = 0;
    menu_item *pick_list;
    boolean all_worn_categories = TRUE;

    if (retry) {
	all_worn_categories = (retry == -2);
    } else if (flags.menu_style == MENU_FULL) {
	all_worn_categories = FALSE;
	n = query_category("What type of things do you want to take off?",
			   invent, WORN_TYPES|ALL_TYPES, &pick_list, PICK_ANY);
	if (!n) return 0;
	for (i = 0; i < n; i++) {
	    if (pick_list[i].item.a_int == ALL_TYPES_SELECTED)
		all_worn_categories = TRUE;
	    else
		add_valid_menu_class(pick_list[i].item.a_int);
	}
	free((genericptr_t) pick_list);
    } else if (flags.menu_style == MENU_COMBINATION) {
	all_worn_categories = FALSE;
	if (ggetobj("take off", select_off, 0, TRUE, (unsigned *)0) == -2)
	    all_worn_categories = TRUE;
    }

    n = query_objlist("What do you want to take off?", invent,
			SIGNAL_NOMENU|USE_INVLET|INVORDER_SORT,
			&pick_list, PICK_ANY,
			all_worn_categories ? is_worn : is_worn_by_type);
    if (n > 0) {
	for (i = 0; i < n; i++)
	    (void) select_off(pick_list[i].item.a_obj);
	free((genericptr_t) pick_list);
    } else if (n < 0 && flags.menu_style != MENU_COMBINATION) {
	There("is nothing else you can remove or unwield.");
    }
    return 0;
}

/* hit by destroy armor scroll/black dragon breath/monster spell */
int
destroy_arm(atmp)
register struct obj *atmp;
{
	register struct obj *otmp;
#define DESTROY_ARM(o) ((otmp = (o)) != 0 && \
			(!atmp || atmp == otmp) && \
			(!obj_resists(otmp, 0, 90)))

	if (DESTROY_ARM(uarmc)) {
		if (donning(otmp)) cancel_don();
		Your("%s crumbles and turns to dust!",
		     cloak_simple_name(uarmc));
		(void) Cloak_off();
		useup(otmp);
	} else if (DESTROY_ARM(uarm)) {
		if (donning(otmp)) cancel_don();
		Your("armor turns to dust and falls to the %s!",
			surface(u.ux,u.uy));
		(void) Armor_gone();
		useup(otmp);
#ifdef TOURIST
	} else if (DESTROY_ARM(uarmu)) {
		if (donning(otmp)) cancel_don();
		Your("shirt crumbles into tiny threads and falls apart!");
		(void) Shirt_off();
		useup(otmp);
#endif
	} else if (DESTROY_ARM(uarmh)) {
		if (donning(otmp)) cancel_don();
		Your("helmet turns to dust and is blown away!");
		(void) Helmet_off();
		useup(otmp);
	} else if (DESTROY_ARM(uarmg)) {
		if (donning(otmp)) cancel_don();
		Your("gloves vanish!");
		(void) Gloves_off();
		useup(otmp);
		selftouch("You");
	} else if (DESTROY_ARM(uarmf)) {
		if (donning(otmp)) cancel_don();
		Your("boots disintegrate!");
		(void) Boots_off();
		useup(otmp);
	} else if (DESTROY_ARM(uarms)) {
		if (donning(otmp)) cancel_don();
		Your("shield crumbles away!");
		(void) Shield_off();
		useup(otmp);
	} else {
		return 0;		/* could not destroy anything */
	}

#undef DESTROY_ARM
	stop_occupation();
	return(1);
}

void
adj_abon(otmp, delta)
register struct obj *otmp;
register schar delta;
{
	if (uarmg && uarmg == otmp && otmp->otyp == GAUNTLETS_OF_DEXTERITY) {
		if (delta) {
			makeknown(uarmg->otyp);
			ABON(A_DEX) += (delta);
		}
		flags.botl = 1;
	}
	if (uarmg && uarmg == otmp && otmp->otyp == GAUNTLETS_OF_POWER) {
		if (delta) {
			makeknown(uarmg->otyp);
			ABON(A_STR) += (delta);
		}
		flags.botl = 1;
	}
	if (uarmh && uarmh == otmp && otmp->otyp == HELM_OF_BRILLIANCE) {
		if (delta) {
			makeknown(uarmh->otyp);
			ABON(A_INT) += (delta);
			ABON(A_WIS) += (delta);
		}
		flags.botl = 1;
	}
}

#endif /* OVLB */

/*do_wear.c*/
