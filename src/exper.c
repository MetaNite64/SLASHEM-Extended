/*	SCCS Id: @(#)exper.c	3.4	2002/11/20	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"

/*STATIC_DCL*/ long newuexp(int);
STATIC_DCL int enermod(int);

#define PN_POLEARMS		(-1)
#define PN_SABER		(-2)
#define PN_HAMMER		(-3)
#define PN_WHIP			(-4)
#define PN_PADDLE		(-5)
#define PN_FIREARMS		(-6)
#define PN_ATTACK_SPELL		(-7)
#define PN_HEALING_SPELL	(-8)
#define PN_DIVINATION_SPELL	(-9)
#define PN_ENCHANTMENT_SPELL	(-10)
#define PN_PROTECTION_SPELL	(-11)
#define PN_BODY_SPELL		(-12)
#define PN_OCCULT_SPELL		(-13)
#define PN_ELEMENTAL_SPELL		(-14)
#define PN_CHAOS_SPELL		(-15)
#define PN_MATTER_SPELL		(-16)
#define PN_BARE_HANDED		(-17)
#define PN_HIGH_HEELS		(-18)
#define PN_GENERAL_COMBAT		(-19)
#define PN_SHIELD		(-20)
#define PN_BODY_ARMOR		(-21)
#define PN_TWO_HANDED_WEAPON		(-22)
#define PN_POLYMORPHING		(-23)
#define PN_DEVICES		(-24)
#define PN_SEARCHING		(-25)
#define PN_SPIRITUALITY		(-26)
#define PN_PETKEEPING		(-27)
#define PN_MISSILE_WEAPONS		(-28)
#define PN_TECHNIQUES		(-29)
#define PN_IMPLANTS		(-30)
#define PN_SEXY_FLATS		(-31)
#define PN_SHII_CHO		(-32)
#define PN_MAKASHI		(-33)
#define PN_SORESU		(-34)
#define PN_ATARU		(-35)
#define PN_SHIEN		(-36)
#define PN_DJEM_SO		(-37)
#define PN_NIMAN		(-38)
#define PN_JUYO		(-39)
#define PN_VAAPAD		(-40)
#define PN_WEDI		(-41)
#define PN_MARTIAL_ARTS		(-42)
#define PN_RIDING		(-43)
#define PN_TWO_WEAPONS		(-44)
#define PN_LIGHTSABER		(-45)

#ifndef OVLB

STATIC_DCL NEARDATA const short skill_names_indices[];
STATIC_DCL NEARDATA const char *odd_skill_names[];

#else	/* OVLB */

/* KMH, balance patch -- updated */
STATIC_OVL NEARDATA const short skill_names_indices[P_NUM_SKILLS] = {
	0,                DAGGER,         KNIFE,        AXE,
	PICK_AXE,         SHORT_SWORD,    BROADSWORD,   LONG_SWORD,
	TWO_HANDED_SWORD, SCIMITAR,       PN_SABER,     CLUB,
	PN_PADDLE,        MACE,           MORNING_STAR,   FLAIL,
	PN_HAMMER,        QUARTERSTAFF,   PN_POLEARMS,  SPEAR,
	JAVELIN,          TRIDENT,        LANCE,        BOW,
	SLING,            PN_FIREARMS,    CROSSBOW,       DART,
	SHURIKEN,         BOOMERANG,      PN_WHIP,      UNICORN_HORN,
	PN_LIGHTSABER,
	PN_ATTACK_SPELL,     PN_HEALING_SPELL,
	PN_DIVINATION_SPELL, PN_ENCHANTMENT_SPELL,
	PN_PROTECTION_SPELL,            PN_BODY_SPELL,
	PN_OCCULT_SPELL,
	PN_ELEMENTAL_SPELL,
	PN_CHAOS_SPELL,
	PN_MATTER_SPELL,
	PN_BARE_HANDED,	PN_HIGH_HEELS,
	PN_GENERAL_COMBAT,	PN_SHIELD,	PN_BODY_ARMOR,
	PN_TWO_HANDED_WEAPON,	PN_POLYMORPHING,	PN_DEVICES,
	PN_SEARCHING,	PN_SPIRITUALITY,	PN_PETKEEPING,
	PN_MISSILE_WEAPONS,	PN_TECHNIQUES,	PN_IMPLANTS,	PN_SEXY_FLATS,
	PN_SHII_CHO,	PN_MAKASHI,	PN_SORESU,
	PN_ATARU,	PN_SHIEN,	PN_DJEM_SO,
	PN_NIMAN,	PN_JUYO,	PN_VAAPAD,	PN_WEDI,
	PN_MARTIAL_ARTS, 
	PN_TWO_WEAPONS,
	PN_RIDING,
};


STATIC_OVL NEARDATA const char * const odd_skill_names[] = {
    "no skill",
    "polearms",
    "saber",
    "hammer",
    "whip",
    "paddle",
    "firearms",
    "attack spells",
    "healing spells",
    "divination spells",
    "enchantment spells",
    "protection spells",
    "body spells",
    "occult spells",
    "elemental spells",
    "chaos spells",
    "matter spells",
    "bare-handed combat",
    "high heels",
    "general combat",
    "shield",
    "body armor",
    "two-handed weapons",
    "polymorphing",
    "devices",
    "searching",
    "spirituality",
    "petkeeping",
    "missile weapons",
    "techniques",
    "implants",
    "sexy flats",
    "form I (Shii-Cho)",
    "form II (Makashi)",
    "form III (Soresu)",
    "form IV (Ataru)",
    "form V (Shien)",
    "form V (Djem So)",
    "form VI (Niman)",
    "form VII (Juyo)",
    "form VII (Vaapad)",
    "form VIII (Wedi)",
    "martial arts",
    "riding",
    "two-weapon combat",
    "lightsaber"
};

#endif	/* OVLB */

#define P_NAME(type) (skill_names_indices[type] > 0 ? \
		      OBJ_NAME(objects[skill_names_indices[type]]) : \
			odd_skill_names[-skill_names_indices[type]])

/*STATIC_OVL*/ long
newuexp(lev)
int lev;
{
	/* KMH, balance patch -- changed again! */
	/*if (lev < 9) return (10L * (1L << lev));
	if (lev < 13) return (10000L * (1L << (lev - 9)));
	if (lev == 13) return (150000L);
	return (50000L * ((long)(lev - 9)));*/
	/*              Old XP routine */
	/* if (lev < 10) return (10L * (1L << lev));            */
	/* if (lev < 20) return (10000L * (1L << (lev - 10)));  */
	/* return (10000000L * ((long)(lev - 19)));             */
/*      if (lev == 1)  return (75L);
	if (lev == 2)  return (150L);
	if (lev == 3)  return (300L);
	if (lev == 4)  return (600L);
	if (lev == 5)  return (1200L); */

	/* completely overhauled by Amy */

	if (!issoviet) {

	if (lev == 1)  return (20L);     /* need 20           */
	if (lev == 2)  return (40L);    /* need 20           */
	if (lev == 3)  return (80L);    /* need 40          */
	if (lev == 4)  return (160L);    /* need 80          */
	if (lev == 5)  return (320L);   /* need 160          */
	if (lev == 6)  return (640L);   /* need 320          */
	if (lev == 7)  return (1280L);   /* need 640         */
	if (lev == 8)  return (2560L);   /* need 1280         */
	if (lev == 9)  return (5000L);   /* need 2440         */
	if (lev == 10) return (10000L);   /* need 5000         */
	if (lev == 11) return (20000L);  /* need 10000         */
	if (lev == 12) return (40000L);  /* need 20000         */
	if (lev == 13) return (80000L);  /* need 40000         */
	if (lev == 14) return (130000L);  /* need 50000         */
	if (lev == 15) return (200000L);  /* need 70000         */
	if (lev == 16) return (280000L);  /* need 80000         */
	if (lev == 17) return (380000L);  /* need 100000         */
	if (lev == 18) return (500000L);  /* need 120000        */
	if (lev == 19) return (650000L);  /* need 150000        */
	if (lev == 20) return (850000L);  /* need 200000        */
	if (lev == 21) return (1100000L); /* need 250000        */
	if (lev == 22) return (1400000L); /* need 300000        */
	if (lev == 23) return (1800000L); /* need 400000        */
	if (lev == 24) return (2300000L); /* need 500000        */
	if (lev == 25) return (3000000L); /* need 700000       */
	if (lev == 26) return (3800000L); /* need 800000       */
	if (lev == 27) return (4800000L); /* need 1000000       */
	if (lev == 28) return (6000000L); /* need 1200000       */
	if (lev == 29) return (8000000L); /* need 2000000      */
	return (15000000L);

	} else {

	if (lev == 1)  return (40L);
	if (lev == 2)  return (80L);
	if (lev == 3)  return (160L);
	if (lev == 4)  return (320L);
	if (lev == 5)  return (640L);
	if (lev == 6)  return (1280L);
	if (lev == 7)  return (2560L);
	if (lev == 8)  return (5120L);
	if (lev == 9)  return (10000L);
	if (lev == 10) return (20000L);
	if (lev == 11) return (40000L);
	if (lev == 12) return (80000L);
	if (lev == 13) return (130000L);
	if (lev == 14) return (200000L);
	if (lev == 15) return (280000L);
	if (lev == 16) return (380000L);
	if (lev == 17) return (480000L);
	if (lev == 18) return (580000L);
	if (lev == 19) return (680000L);
	if (lev == 20) return (850000L);
	if (lev == 21) return (1100000L);
	if (lev == 22) return (1400000L);
	if (lev == 23) return (1800000L);
	if (lev == 24) return (2300000L);
	if (lev == 25) return (3000000L);
	if (lev == 26) return (3800000L);
	if (lev == 27) return (4800000L);
	if (lev == 28) return (6000000L);
	if (lev == 29) return (8000000L);
	return (15000000L);

	}

}

STATIC_OVL int
enermod(en)
int en;
{
	switch (Role_switch) {
		/* WAC 'F' and 'I' get bonus similar to 'W' */
		case PM_FLAME_MAGE:
		case PM_ICE_MAGE:
		case PM_ELECTRIC_MAGE:
		case PM_POISON_MAGE:
		case PM_ACID_MAGE:
		case PM_OCCULT_MASTER:
		case PM_CHAOS_SORCEROR:
		case PM_ELEMENTALIST:
	case PM_PRIEST:
	case PM_WIZARD:
	    return(2 * en);
	case PM_HEALER:
	case PM_KNIGHT:
	    return((3 * en) / 2);
	case PM_BARBARIAN:
	case PM_VALKYRIE:
	    return((3 * en) / 4);
	default:
	    return (en);
	}
}

int
experience(mtmp, nk)	/* return # of exp points for mtmp after nk killed */
	register struct	monst *mtmp;
	register int	nk;
#if defined(MAC_MPW)
# pragma unused(nk)
#endif
{
	register struct permonst *ptr = mtmp->data;
	int	i, tmp, tmp2;

	tmp = 1 + mtmp->m_lev * mtmp->m_lev;

/*	For higher ac values, give extra experience */
	if ((i = find_mac(mtmp)) < 3) tmp += (7 - i) * ((i < 0) ? 2 : 1);

/*	For very fast monsters, give extra experience */
	if (ptr->mmove > NORMAL_SPEED)
	    tmp += (ptr->mmove > (3*NORMAL_SPEED/2)) ? 5 : 3;

/*	For each "special" attack type give extra experience */
	for(i = 0; i < NATTK; i++) {
	    tmp2 = ptr->mattk[i].aatyp;
	    if(tmp2 > AT_BUTT) {

		if(tmp2 == AT_WEAP) tmp += 5;
		else if(tmp2 == AT_MAGC) tmp += 10;
		else tmp += 3;
	    }
	}

/*	For each "special" damage type give extra experience */
	for(i = 0; i < NATTK; i++) {
	    tmp2 = ptr->mattk[i].adtyp;
	    if(tmp2 > AD_PHYS && tmp2 < AD_BLND) tmp += 2*mtmp->m_lev;
	    else if((tmp2 == AD_DRLI) || (tmp2 == AD_STON) ||
	    		(tmp2 == AD_SLIM)) tmp += 50;
	    else if(tmp != AD_PHYS) tmp += mtmp->m_lev;
		/* extra heavy damage bonus */
	    if((int)(ptr->mattk[i].damd * ptr->mattk[i].damn) > 23)
		tmp += mtmp->m_lev;
	    if (tmp2 == AD_WRAP && ptr->mlet == S_EEL && !Amphibious)
		tmp += 100;
	}

/*	For certain "extra nasty" monsters, give even more */
	if (extra_nasty(ptr)) tmp += (rnd(7) * mtmp->m_lev);

/*	For higher level monsters, an additional bonus is given */
	if(mtmp->m_lev > 8) tmp += 50;

#ifdef MAIL
	/* Mail daemons put up no fight. */
	if(mtmp->data == &mons[PM_MAIL_DAEMON]) tmp = 1;
#endif

	return(tmp);
}

void
more_experienced(exp, rexp)
	register int exp, rexp;
{
	if (u.uprops[ANTILEVELING].extrinsic || Antileveling || (uamul && uamul->oartifact == ART_NAZGUL_S_REVENGE) || have_antilevelstone() ) return;

	if ((exp > 0) && Race_if(PM_YEEK)) exp *= 2;
	if (uarmc && uarmc->oartifact == ART_ARTIFICIAL_FAKE_DIFFICULTY && (exp > 1)) exp /= 2;

	u.uexp += exp;
	u.urexp += 4*exp + rexp;
	if(exp || flags.showscore) flags.botl = 1;
	if (u.urexp >= (Role_if(PM_WIZARD) ? 1000 : 2000))
		flags.beginner = 0;
}

void
losexp(drainer,force,dresistance)	/* e.g., hit by drain life attack */
const char *drainer;	/* cause of death, if drain should be fatal */
boolean force;		/* Force the loss of an experience level */
boolean dresistance;	/* level drain resistance can protect you */
{
	register int num;
	int expdrain;

#ifdef WIZARD
	/* explicit wizard mode requests to reduce level are never fatal. */
	if (drainer && !strcmp(drainer, "#levelchange"))
		drainer = 0;
#endif

	if (dresistance && Drain_resistance && rn2(5) ) return;

	/* level drain is too strong. Let's nerf it a bit. --Amy */
	/* In Soviet Russia, level drain will always drain at least one level, because fuck you, stupid player. You're
	 * not supposed to stand a chance in this game. --Amy */
	if (!force && (u.uexp > 320) && !issoviet && u.ulevel > 1) {
		expdrain = newuexp(u.ulevel) - newuexp(u.ulevel - 1);
		expdrain /= (isfriday ? 3 : 5);
		expdrain = rnz(expdrain);
		if ((u.uexp - expdrain) > newuexp(u.ulevel - 1)) {
			/* drain some experience, but not enough to make you lose a level */
			You_feel("your life draining away!");
			if (PlayerHearsSoundEffects) pline(issoviet ? "Vy tol'ko chto poteryali odin uroven', skoro vy poteryayete vse urovni i umeret'." : "Due-l-ue-l-ue-l!");
			u.uexp -= expdrain;
			return;
		}
	}

	if (issoviet) pline("BWAR KHAR (gryaznyy smekh) govorit, chto tip bloka l'da!");

	if (u.ulevel > 1) {
		pline("%s level %d.", Goodbye(), u.ulevel--);
		if (PlayerHearsSoundEffects) pline(issoviet ? "Vy tol'ko chto poteryali odin uroven', skoro vy poteryayete vse urovni i umeret'." : "Due-l-ue-l-ue-l!");
		/* remove intrinsic abilities */
		adjabil(u.ulevel + 1, u.ulevel);
		reset_rndmonst(NON_PM);	/* new monster selection */
	} else {
		if (drainer) {
			u.youaredead = 1;
			killer_format = KILLED_BY;
			killer = drainer;
			done(DIED);
			u.youaredead = 0;
		}
		/* no drainer or lifesaved */
		u.uexp = 0;
	}
	num = newhp();
	u.uhpmax -= num;
	u.uhpmax -= rn2(3);
	u.uhpmax -= rnz(2);
	u.uhpmax -= rnz(3); /* making the drain for gain exploit much harder to perform --Amy */
	if (u.ulevel >= 19) u.uhpmax -= rnd(2);
	if (u.ulevel >= 24) u.uhpmax -= rnd(2);
	if (u.ulevel >= 27) u.uhpmax -= rnd(3);
	if (u.ulevel >= 29) u.uhpmax -= rnd(10);
	if (u.uhpmax < 1) u.uhpmax = 1;
	u.uhp -= num;
	u.uhp -= rn2(3);
	u.uhp -= rnz(3);
	u.uhp -= rnz(2);
	if (u.ulevel >= 19) u.uhp -= rnd(2);
	if (u.ulevel >= 24) u.uhp -= rnd(2);
	if (u.ulevel >= 27) u.uhp -= rnd(3);
	if (u.ulevel >= 29) u.uhp -= rnd(10);
	if (u.uhp < 1) u.uhp = 1;
	else if (u.uhp > u.uhpmax) u.uhp = u.uhpmax;

	/* screwy vanilla programmers... they were so lazy and forgot to make it so that your polymorph form gets drained! */
	if (Upolyd) {
		u.mhmax -= num;
		u.mhmax -= rn2(3);
		u.mhmax -= rnz(2);
		u.mhmax -= rnz(3); /* making the drain for gain exploit much harder to perform --Amy */
		if (u.ulevel >= 19) u.mhmax -= rnd(2);
		if (u.ulevel >= 24) u.mhmax -= rnd(2);
		if (u.ulevel >= 27) u.mhmax -= rnd(3);
		if (u.ulevel >= 29) u.mhmax -= rnd(10);
		if (u.mhmax < 1) u.mhmax = 1;
		u.mh -= num;
		u.mh -= rn2(3);
		u.mh -= rnz(3);
		u.mh -= rnz(2);
		if (u.ulevel >= 19) u.mh -= rnd(2);
		if (u.ulevel >= 24) u.mh -= rnd(2);
		if (u.ulevel >= 27) u.mh -= rnd(3);
		if (u.ulevel >= 29) u.mh -= rnd(10);
		if (u.mh < 1) u.mh = 1;
		else if (u.mh > u.mhmax) u.mh = u.mhmax;
	}

	if (u.ulevel < urole.xlev)
	    num = rn1((int)ACURR(A_WIS)/2 + urole.enadv.lornd + urace.enadv.lornd,
			urole.enadv.lofix + urace.enadv.lofix);
	else
	    num = rn1((int)ACURR(A_WIS)/2 + urole.enadv.hirnd + urace.enadv.hirnd,
			urole.enadv.hifix + urace.enadv.hifix);
	num = enermod(num);		/* M. Stephenson */
	u.uenmax -= num;
	u.uenmax -= rn2(3);
	u.uenmax -= rnz(3);
	u.uenmax -= (rn2(3) ? rnz(1) : rnz(2));
	if (u.uenmax < 0) u.uenmax = 0;
	u.uen -= num;
	u.uen -= rn2(3);
	u.uen -= rnz(3);
	if (u.uen < 0) u.uen = 0;
	else if (u.uen > u.uenmax) u.uen = u.uenmax;

	if (u.uexp > 0)
		u.uexp = newuexp(u.ulevel) - 1;
	flags.botl = 1;
}

/*
 * Make experience gaining similar to AD&D(tm), whereby you can at most go
 * up by one level at a time, extra expr possibly helping you along.
 * After all, how much real experience does one get shooting a wand of death
 * at a dragon created with a wand of polymorph??
 */
void
newexplevel()
{
	if (u.ulevel < MAXULEV && u.uexp >= newuexp(u.ulevel))
	    pluslvl(TRUE);
	else if (u.uexp >= (10000000 + (2000000 * u.xtralevelmult))) {
	    u.xtralevelmult++;
	    u.uexp = 10000000;
	    You_feel("more experienced.");
	    pluslvl(TRUE);
	}
}

#if 0 /* The old newexplevel() */
{
	register int tmp;
	struct obj *ubook;

	if(u.ulevel < MAXULEV && u.uexp >= newuexp(u.ulevel)) {

		u.ulevel++;
		if (u.ulevelmax < u.ulevel) u.ulevelmax = u.ulevel;	/* KMH */
		if (u.uexp >= newuexp(u.ulevel)) u.uexp = newuexp(u.ulevel) - 1;
		pline("Welcome to experience level %d.", u.ulevel);
		/* give new intrinsics */
		adjabil(u.ulevel - 1, u.ulevel);


		reset_rndmonst(NON_PM); /* new monster selection */
/* STEPHEN WHITE'S NEW CODE */                
		tmp = newhp();
		u.uhpmax += tmp;
		u.uhpmax += rn2(3);
		u.uhp += tmp;
		u.uhpmax += rnz(2); /*making the game a bit easier --Amy */
		if (!issoviet && (u.uhp < u.uhpmax)) u.uhp = u.uhpmax;
		if (issoviet) pline("Vy dazhe ne poluchayete polnyye linii, potomu chto sovetskiy ne ponimayet, kak rolevyye igry rabotayut!");
		switch (Role_switch) {
			case PM_ARCHEOLOGIST: u.uenbase += rnd(4) + 1; break;
			case PM_BARBARIAN: u.uenbase += rnd(2); break;
			case PM_CAVEMAN: u.uenbase += rnd(2); break;
			/*case PM_DOPPELGANGER: u.uenbase += rnd(5) + 1; break;
			case PM_ELF: case PM_DROW: u.uenbase += rnd(5) + 1; break;*/
			case PM_FLAME_MAGE: u.uenbase += rnd(6) + 2; break;
			case PM_ACID_MAGE: u.uenbase += rnd(6) + 2; break;
			case PM_GNOME: u.uenbase += rnd(3); break;
			case PM_HEALER: u.uenbase += rnd(6) + 2; break;
			case PM_ICE_MAGE: u.uenbase += rnd(6) + 2; break;
			case PM_ELECTRIC_MAGE: u.uenbase += rnd(6) + 2; break;
			case PM_YEOMAN:
			case PM_KNIGHT: u.uenbase += rnd(3); break;
			/*case PM_HUMAN_WEREWOLF: u.uenbase += rnd(5) + 1; break;*/
			case PM_MONK: u.uenbase += rnd(5) + 1; break;
			case PM_ELPH: u.uenbase += rnd(5) + 1; break;
			case PM_NECROMANCER: u.uenbase += rnd(6) + 2; break;
			case PM_PRIEST: u.uenbase += rnd(6) + 2; break;
			case PM_CHEVALIER: u.uenbase += rnd(6) + 2; break;
			case PM_ROGUE: u.uenbase += rnd(4) + 1; break;
			/*case PM_MAIA: u.uenbase += rnd(4) + 1; break;
			case PM_GASTLY: u.uenbase += rnd(3) + 1; break;*/
			case PM_SAMURAI: u.uenbase += rnd(2); break;
			case PM_TOURIST: u.uenbase += rnd(4) + 1; break;
			case PM_UNDEAD_SLAYER: u.uenbase += rnd(3); break;
			case PM_VALKYRIE: u.uenbase += rnd(2); break;
			case PM_WIZARD: u.uenbase += rnd(6) + 2; break;
			case PM_CONVICT: break;
			/*case PM_ALIEN: break;
			case PM_OGRO: break;
			case PM_KOBOLT: break;
			case PM_TROLLOR: break;
			case PM_GIGANT: break;*/
			case PM_WARRIOR: break;
			case PM_COURIER: break;
			default: u.uenbase += rnd(2) + 1; break;
		}
		if (u.uen < u.uenmax) u.uen = u.uenmax;

		flags.botl = 1;
	}
}
#endif /* old newexplevel() */

void
pluslvl(incr)
boolean incr;	/* true iff via incremental experience growth */
{		/*	(false for potion of gain level)      */
	register int num;
	struct obj *ubookz;
	register struct obj *acqo;

	if (!incr) You_feel("more experienced.");

	if (u.ulevel < MAXULEV) {

	if (!ishomicider || rn2(2)) {	/* homicider only gains hp/pw 50% of the time --Amy */
	/* a.k.a. "bullshit downside that every fun new race gets" (term coined by Khor) */

	num = newhp();
	num += rnz(2);
	if (num < 0) num = 0;
	if (Race_if(PM_SPRIGGAN) && !rn2(2)) num = 0;
	num += rn2(3);
	if (u.ulevel >= 19) num += rnd(2);
	if (u.ulevel >= 24) num += rnd(2);
	if (u.ulevel >= 27) num += rnd(3);
	if (u.ulevel >= 29) num += rnd(10);

	if (Race_if(PM_YEEK) || Race_if(PM_DUFFLEPUD)) num /= 2;
	if (Race_if(PM_PLAYER_DOLGSMAN)) {
		num *= 3;
		num /= 4;
	}

	u.uhpmax += num;
	u.uhp += num;

	if ((u.ulevel >= u.urmaxlvlUP && u.ulevel < 30) && !issoviet && (u.uhp < u.uhpmax)) u.uhp = u.uhpmax;
	if (issoviet) pline("Vy dazhe ne poluchayete polnyye linii, potomu chto sovetskiy ne ponimayet, kak rolevyye igry rabotayut!");
	/* In Soviet Russia, you don't get full health on leveling up because seriously, who needs that? --Amy */
	if (Upolyd) {
	    num = rnz(8); /* unfortunately will be lost upon unpolymorphing --Amy */
	    if (num < 0) num = 0;
	    num += rn2(3);

	    if (Race_if(PM_YEEK) || Race_if(PM_DUFFLEPUD)) num /= 2;
	    if (Race_if(PM_PLAYER_DOLGSMAN)) {
		num *= 3;
		num /= 4;
	    }

	    u.mhmax += num;
	    u.mh += num;
		if ((u.ulevel >= u.urmaxlvlUP && u.ulevel < 30) && !issoviet && (u.mh < u.mhmax)) u.mh = u.mhmax;
	}
	if (u.ulevel < urole.xlev)
	    num = rn1((int)ACURR(A_WIS)/2 + urole.enadv.lornd + urace.enadv.lornd,
			urole.enadv.lofix + urace.enadv.lofix);
	else
	    num = rn1((int)ACURR(A_WIS)/2 + urole.enadv.hirnd + urace.enadv.hirnd,
			urole.enadv.hifix + urace.enadv.hifix);

	num = enermod(num);	/* M. Stephenson */
	num += (rn2(3) ? rnz(1) : rnz(2));
	if (num < 0) num = 0;
	num += rn2(3);

	if (Race_if(PM_YEEK) || Race_if(PM_DUFFLEPUD)) num /= 2;

	u.uenmax += num;
	u.uen += num;
	} /* homicider check */

	} else { /* u.ulevel > MAXULEV */

	if (!ishomicider || rn2(2)) {	/* homicider only gains hp/pw 50% of the time --Amy */
	/* a.k.a. "bullshit downside that every fun new race gets" (term coined by Khor) */

	num = newhp();
	num += rnz(2);
	if (num < 0) num = 0;
	if (num > 1) num /= rnd(12);
	if (Race_if(PM_SPRIGGAN) && !rn2(2)) num = 0;
	num += rn2(2);

	if (Race_if(PM_YEEK) || Race_if(PM_DUFFLEPUD)) num /= 2;
	if (Race_if(PM_PLAYER_DOLGSMAN)) {
		num *= 3;
		num /= 4;
	}

	u.uhpmax += num;
	u.uhp += num;

	if (Upolyd) {
	    num = rnz(8); /* unfortunately will be lost upon unpolymorphing --Amy */
	    if (num < 0) num = 0;
	    if (num > 1) num /= rnd(12);
	    num += rn2(2);

	    if (Race_if(PM_YEEK) || Race_if(PM_DUFFLEPUD)) num /= 2;
	    if (Race_if(PM_PLAYER_DOLGSMAN)) {
		num *= 3;
		num /= 4;
	    }

	    u.mhmax += num;
	    u.mh += num;
	}
	if (u.ulevel < urole.xlev)
	    num = rn1((int)ACURR(A_WIS)/2 + urole.enadv.lornd + urace.enadv.lornd,
			urole.enadv.lofix + urace.enadv.lofix);
	else
	    num = rn1((int)ACURR(A_WIS)/2 + urole.enadv.hirnd + urace.enadv.hirnd,
			urole.enadv.hifix + urace.enadv.hifix);

	num = enermod(num);	/* M. Stephenson */
	num += (rn2(3) ? rnz(1) : rnz(2));
	if (num < 0) num = 0;
	if (num > 1) num /= rnd(12);
	num += rn2(2);

	if (Race_if(PM_YEEK) || Race_if(PM_DUFFLEPUD)) num /= 2;

	u.uenmax += num;
	u.uen += num;

	} /* homicider check */

	} /* u.ulevel > or < MAXULEV */


	if (u.ulevel >= u.urmaxlvlUP && u.ulevel < 30) {
		u.urmaxlvlUP = (u.ulevel + 1);

		if (Role_if(PM_FEMINIST) && !rn2(5)) {

			boolean havegifts = u.ugifts;

			if (!havegifts) u.ugifts++;

			acqo = mk_artifact((struct obj *)0, !rn2(3) ? A_CHAOTIC : rn2(2) ? A_NEUTRAL : A_LAWFUL, TRUE);
			if (acqo) {
			    dropy(acqo);
				if (P_MAX_SKILL(get_obj_skill(acqo, TRUE)) == P_ISRESTRICTED) {
					unrestrict_weapon_skill(get_obj_skill(acqo, TRUE));
				} else if (P_MAX_SKILL(get_obj_skill(acqo, TRUE)) == P_UNSKILLED) {
					unrestrict_weapon_skill(get_obj_skill(acqo, TRUE));
					P_MAX_SKILL(get_obj_skill(acqo, TRUE)) = P_BASIC;
				} else if (rn2(2) && P_MAX_SKILL(get_obj_skill(acqo, TRUE)) == P_BASIC) {
					P_MAX_SKILL(get_obj_skill(acqo, TRUE)) = P_SKILLED;
				} else if (!rn2(4) && P_MAX_SKILL(get_obj_skill(acqo, TRUE)) == P_SKILLED) {
					P_MAX_SKILL(get_obj_skill(acqo, TRUE)) = P_EXPERT;
				} else if (!rn2(10) && P_MAX_SKILL(get_obj_skill(acqo, TRUE)) == P_EXPERT) {
					P_MAX_SKILL(get_obj_skill(acqo, TRUE)) = P_MASTER;
				} else if (!rn2(100) && P_MAX_SKILL(get_obj_skill(acqo, TRUE)) == P_MASTER) {
					P_MAX_SKILL(get_obj_skill(acqo, TRUE)) = P_GRAND_MASTER;
				} else if (!rn2(200) && P_MAX_SKILL(get_obj_skill(acqo, TRUE)) == P_GRAND_MASTER) {
					P_MAX_SKILL(get_obj_skill(acqo, TRUE)) = P_SUPREME_MASTER;
				}

			    discover_artifact(acqo->oartifact);

				if (!havegifts) u.ugifts--;
				pline("An artifact appeared beneath you!");

			}

		}

		if (Role_if(PM_FEMINIST) && !rn2(3)) {

			boolean femintcheck = 0;
			if (rnd(70) < ACURR(A_INT)) femintcheck = 1;

			if (u.urmaxlvlUP == 3) {
				switch (u.femauspices4) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, your potions will have a chance of not working when quaffed.");
						else pline("Your auspices say: 4 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, your techniques will stop working.");
						else pline("Your auspices say: 4 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, Elbereth engravings will no longer work.");
						else pline("Your auspices say: 4 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, zapping a wand will have a chance to make it explode.");
						else pline("Your auspices say: 4 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, reading a spellbook is no longer safe.");
						else pline("Your auspices say: 4 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, your magical effects will always be very low-level.");
						else pline("Your auspices say: 4 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, you will no longer be able to uncurse your items via scrolls of remove curse, holy water or cancellation.");
						else pline("Your auspices say: 4 7");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, the 'never mind' or 'nothing happens' messages will cause bad effects.");
						else pline("Your auspices say: 4 8");
						break;
					case 9:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, you will repeatedly lose spell memory.");
						else pline("Your auspices say: 4 9");
						break;
					case 10:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, your skill training will occasionally be reduced.");
						else pline("Your auspices say: 4 10");
						break;
					case 11:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 4, your techniques will occasionally receive increased timeouts.");
						else pline("Your auspices say: 4 11");
						break;
				}

			} else if (u.urmaxlvlUP == 5) {
				switch (u.femauspices6) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 6, your right mouse button will fail.");
						else pline("Your auspices say: 6 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 6, your display will fail.");
						else pline("Your auspices say: 6 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 6, there will be low local memory.");
						else pline("Your auspices say: 6 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 6, you will get BIGscript.");
						else pline("Your auspices say: 6 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 6, you will be afflicted with weak sight.");
						else pline("Your auspices say: 6 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 6, you will see flicker strips.");
						else pline("Your auspices say: 6 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 6, menus will be fleecy-colored.");
						else pline("Your auspices say: 6 7");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 6, you start seeing mojibake glyphs.");
						else pline("Your auspices say: 6 8");
						break;
				}

			} else if (u.urmaxlvlUP == 7) {
				switch (u.femauspices8) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 8, the entire game will display in shades of grey.");
						else pline("Your auspices say: 8 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 8, a ROT13 cypher will be activated.");
						else pline("Your auspices say: 8 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 8, you will have to solve captchas.");
						else pline("Your auspices say: 8 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 8, you will have to take part in the Nethack Quiz.");
						else pline("Your auspices say: 8 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 8, there will be sound effects.");
						else pline("Your auspices say: 8 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 8, walls will become hyper blue.");
						else pline("Your auspices say: 8 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 8, the pokedex will lie to you.");
						else pline("Your auspices say: 8 7");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 8, monsters will make noises.");
						else pline("Your auspices say: 8 8");
						break;
				}

			} else if (u.urmaxlvlUP == 10) {
				switch (u.femauspices11) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become pink.");
						else pline("Your auspices say: 11 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become cyan.");
						else pline("Your auspices say: 11 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become black.");
						else pline("Your auspices say: 11 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become red.");
						else pline("Your auspices say: 11 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become green.");
						else pline("Your auspices say: 11 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become blue.");
						else pline("Your auspices say: 11 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become completely gray.");
						else pline("Your auspices say: 11 7");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become white.");
						else pline("Your auspices say: 11 8");
						break;
					case 9:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become yellow.");
						else pline("Your auspices say: 11 9");
						break;
					case 10:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become orange.");
						else pline("Your auspices say: 11 10");
						break;
					case 11:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become violet.");
						else pline("Your auspices say: 11 11");
						break;
					case 12:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become brown.");
						else pline("Your auspices say: 11 12");
						break;
					case 13:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become bright cyan.");
						else pline("Your auspices say: 11 13");
						break;
					case 14:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become silver.");
						else pline("Your auspices say: 11 14");
						break;
					case 15:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become metal.");
						else pline("Your auspices say: 11 15");
						break;
					case 16:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 11, your spells will become platinum.");
						else pline("Your auspices say: 11 16");
						break;
				}

			} else if (u.urmaxlvlUP == 12) {
				switch (u.femauspices13) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 13, you will be subjected to random intrinsic loss.");
						else pline("Your auspices say: 13 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 13, you will start to temporarily lose intrinsics.");
						else pline("Your auspices say: 13 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 13, you will start suffering from blood loss.");
						else pline("Your auspices say: 13 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 13, all newly spawned monsters will be hostile.");
						else pline("Your auspices say: 13 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 13, covetous monsters start using their AI in a smarter way.");
						else pline("Your auspices say: 13 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 13, identifying items becomes much more difficult.");
						else pline("Your auspices say: 13 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 13, Satan will cause you to lose a turn when entering a new level.");
						else pline("Your auspices say: 13 7");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 13, monsters are generated with movement energy.");
						else pline("Your auspices say: 13 8");
						break;
				}

			} else if (u.urmaxlvlUP == 14) {
				switch (u.femauspices15) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 15, you will get the speed bug.");
						else pline("Your auspices say: 15 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 15, your multi-turn actions will be interrupted every turn.");
						else pline("Your auspices say: 15 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 15, you can no longer teleport at all.");
						else pline("Your auspices say: 15 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 15, all items that you drop will automatically curse themselves.");
						else pline("Your auspices say: 15 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 15, monsters can randomly move faster than normal.");
						else pline("Your auspices say: 15 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 15, blessed items that you pick up will instantly become cursed.");
						else pline("Your auspices say: 15 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 15, using consumables will curse them first.");
						else pline("Your auspices say: 15 7");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 15, items that monsters steal from you will be degraded.");
						else pline("Your auspices say: 15 8");
						break;
				}

			} else if (u.urmaxlvlUP == 16) {
				switch (u.femauspices17) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 17, the monster difficulty will be increased.");
						else pline("Your auspices say: 17 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 17, monsters start spawning much faster.");
						else pline("Your auspices say: 17 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 17, boss monsters become more common.");
						else pline("Your auspices say: 17 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 17, rare monsters become common.");
						else pline("Your auspices say: 17 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 17, high-level monsters become more common.");
						else pline("Your auspices say: 17 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 17, the minimum level for newly spawned monsters will start scaling with the actual level difficulty.");
						else pline("Your auspices say: 17 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 17, monster attacks use exploding dice to determine their damage.");
						else pline("Your auspices say: 17 7");
						break;
				}

			} else if (u.urmaxlvlUP == 19) {
				switch (u.femauspices20) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 20, your possessions start randomly unidentifying themselves.");
						else pline("Your auspices say: 20 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 20, you will get insufficient information about items.");
						else pline("Your auspices say: 20 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 20, dungeon floors become invisible.");
						else pline("Your auspices say: 20 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 20, monsters become stronger if many of their species have been generated already.");
						else pline("Your auspices say: 20 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 20, walls become invisible.");
						else pline("Your auspices say: 20 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 20, the map will fill up with fake I markers.");
						else pline("Your auspices say: 20 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 20, I markers become invisible and will cost a turn if you bump into them.");
						else pline("Your auspices say: 20 7");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 20, you are subjected to the wing yellow changer.");
						else pline("Your auspices say: 20 8");
						break;
				}

			} else if (u.urmaxlvlUP == 21) {
				switch (u.femauspices22) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 22, you can no longer cure status effects.");
						else pline("Your auspices say: 22 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 22, the dungeon starts becoming ever more chaotic.");
						else pline("Your auspices say: 22 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 22, altars and prayer become unsafe.");
						else pline("Your auspices say: 22 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 22, all newly spawned monsters will be invisible, and see invisible won't help.");
						else pline("Your auspices say: 22 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 22, all newly spawned traps will be invisible, and there is no way to make them visible again.");
						else pline("Your auspices say: 22 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 22, empty dungeon floor tiles will quickly turn into walls.");
						else pline("Your auspices say: 22 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 22, spells with low spell memory are difficult to cast.");
						else pline("Your auspices say: 22 7");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 22, any trap you trigger will become invisible.");
						else pline("Your auspices say: 22 8");
						break;
				}

			} else if (u.urmaxlvlUP == 24) {
				switch (u.femauspices25) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 25, the display will intermittently become all gray.");
						else pline("Your auspices say: 25 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 25, your position and the eight surrounding squares will be obscured.");
						else pline("Your auspices say: 25 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 25, you will contract the checkerboard disease.");
						else pline("Your auspices say: 25 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 25, you will no longer be able to identify the type of a trap.");
						else pline("Your auspices say: 25 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 25, you will no longer be able to determine what monsters are.");
						else pline("Your auspices say: 25 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 25, leveling up no longer increases your maximum HP and Pw.");
						else pline("Your auspices say: 25 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 25, some calculations will work as if your level were 1.");
						else pline("Your auspices say: 25 7");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 25, a manler will start chasing you.");
						else pline("Your auspices say: 25 8");
						break;
				}

			} else if (u.urmaxlvlUP == 27) {
				switch (u.femauspices28) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 28, your maximum alignment record will decrease over time.");
						else pline("Your auspices say: 28 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 28, you will start suffering from recurring amnesia.");
						else pline("Your auspices say: 28 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 28, your items will start to disenchant themselves spontaneously.");
						else pline("Your auspices say: 28 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 28, all equipment you put on will autocurse.");
						else pline("Your auspices say: 28 4");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 28, monsters will be able to use their secret attack.");
						else pline("Your auspices say: 28 5");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 28, you will suffer from random item destruction.");
						else pline("Your auspices say: 28 6");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 28, monsters learn to use their ranged attacks from infinitely far away.");
						else pline("Your auspices say: 28 7");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 28, monsters will be able to use unfair attacks.");
						else pline("Your auspices say: 28 8");
						break;
					case 9:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 28, the presence of bosses causes a horrible call to ring in your head.");
						else pline("Your auspices say: 28 9");
						break;
				}

			} else if (u.urmaxlvlUP == 29) {
				switch (u.femauspices30) {
					case 1:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 30, you can no longer open your inventory.");
						else pline("Your auspices say: 30 1");
						break;
					case 2:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 30, killed monsters no longer drop corpses.");
						else pline("Your auspices say: 30 2");
						break;
					case 3:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 30, your directional keys will be inverted.");
						else pline("Your auspices say: 30 3");
						break;
					case 4:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 30, you always have to press Ctrl-R to see what happened.");
						else pline("Your auspices say: 30 8");
						break;
					case 5:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 30, you will barely be able to see the quasars.");
						else pline("Your auspices say: 30 4");
						break;
					case 6:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 30, your directional keys will be shifted 45 degrees clockwise.");
						else pline("Your auspices say: 30 5");
						break;
					case 7:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 30, your directional keys will be shifted 90 degrees counterclockwise.");
						else pline("Your auspices say: 30 6");
						break;
					case 8:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 30, bumping into monsters will cost a turn unless you use a certain prefix.");
						else pline("Your auspices say: 30 7");
						break;
					case 9:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 30, you won't see anything unless you stand on specific tiles.");
						else pline("Your auspices say: 30 9");
						break;
					case 10:
						if (femintcheck) pline("You receive an elaborate auspicious message: At experience level 30, your stats will slowly decay.");
						else pline("Your auspices say: 30 10");
						break;
				}

			}

		}

	}
	
	if(u.ulevel < MAXULEV) {
	    if (incr) {
		long tmp = newuexp(u.ulevel + 1);
		if (u.uexp >= tmp) u.uexp = tmp - 1;
	    } else {
		u.uexp = newuexp(u.ulevel);
	    }
	    ++u.ulevel;
	    if (u.ulevelmax < u.ulevel) u.ulevelmax = u.ulevel;
	    pline("Welcome to experience level %d.", u.ulevel);
	    adjabil(u.ulevel - 1, u.ulevel);	/* give new intrinsics */
	    reset_rndmonst(NON_PM);		/* new monster selection */
	}

		if (isproblematic && !rn2(3)) {
			/* no xlvl check - if you get drained repeatedly, your loss! I'm really mean :D --Amy */

			switch (rnd(229)) {

				case 1: 
				    SpeedBug |= FROMOUTSIDE; break;
				case 2: 
				    MenuBug |= FROMOUTSIDE; break;
				case 3: 
				    RMBLoss |= FROMOUTSIDE; break;
				case 4: 
				    DisplayLoss |= FROMOUTSIDE; break;
				case 5: 
				    SpellLoss |= FROMOUTSIDE; break;
				case 6: 
				    YellowSpells |= FROMOUTSIDE; break;
				case 7: 
				    AutoDestruct |= FROMOUTSIDE; break;
				case 8: 
				    MemoryLoss |= FROMOUTSIDE; break;
				case 9: 
				    InventoryLoss |= FROMOUTSIDE; break;
				case 10: 
				    BlackNgWalls |= FROMOUTSIDE; break;
				case 11: 
				    Superscroller |= FROMOUTSIDE; break;
				case 12: 
				    FreeHandLoss |= FROMOUTSIDE; break;
				case 13: 
				    Unidentify |= FROMOUTSIDE; break;
				case 14: 
				    Thirst |= FROMOUTSIDE; break;
				case 15: 
				    LuckLoss |= FROMOUTSIDE; break;
				case 16: 
				    ShadesOfGrey |= FROMOUTSIDE; break;
				case 17: 
				    FaintActive |= FROMOUTSIDE; break;
				case 18: 
				    Itemcursing |= FROMOUTSIDE; break;
				case 19: 
				    DifficultyIncreased |= FROMOUTSIDE; break;
				case 20: 
				    Deafness |= FROMOUTSIDE; break;
				case 21: 
				    CasterProblem |= FROMOUTSIDE; break;
				case 22: 
				    WeaknessProblem |= FROMOUTSIDE; break;
				case 23: 
				    RotThirteen |= FROMOUTSIDE; break;
				case 24: 
				    BishopGridbug |= FROMOUTSIDE; break;
				case 25: 
				    ConfusionProblem |= FROMOUTSIDE; break;
				case 26: 
				    NoDropProblem |= FROMOUTSIDE; break;
				case 27: 
				    DSTWProblem |= FROMOUTSIDE; break;
				case 28: 
				    StatusTrapProblem |= FROMOUTSIDE; break;
				case 29: 
				    AlignmentProblem |= FROMOUTSIDE; break;
				case 30: 
				    StairsProblem |= FROMOUTSIDE; break;
				case 31: 
				    UninformationProblem |= FROMOUTSIDE; break;
				case 32: 
				    IntrinsicLossProblem |= FROMOUTSIDE; break;
				case 33: 
				    BloodLossProblem |= FROMOUTSIDE; break;
				case 34: 
				    BadEffectProblem |= FROMOUTSIDE; break;
				case 35: 
				    TrapCreationProblem |= FROMOUTSIDE; break;
				case 36: 
				    AutomaticVulnerabilitiy |= FROMOUTSIDE; break;
				case 37: 
				    TeleportingItems |= FROMOUTSIDE; break;
				case 38: 
				    NastinessProblem |= FROMOUTSIDE; break;
				case 39: 
				    RecurringAmnesia |= FROMOUTSIDE; break;
				case 40: 
				    BigscriptEffect |= FROMOUTSIDE; break;
				case 41: 
				    BankTrapEffect |= FROMOUTSIDE; break;
				case 42: 
				    MapTrapEffect |= FROMOUTSIDE; break;
				case 43: 
				    TechTrapEffect |= FROMOUTSIDE; break;
				case 44: 
				    RecurringDisenchant |= FROMOUTSIDE; break;
				case 45: 
				    verisiertEffect |= FROMOUTSIDE; break;
				case 46: 
				    ChaosTerrain |= FROMOUTSIDE; break;
				case 47: 
				    Muteness |= FROMOUTSIDE; break;
				case 48: 
				    EngravingDoesntWork |= FROMOUTSIDE; break;
				case 49: 
				    MagicDeviceEffect |= FROMOUTSIDE; break;
				case 50: 
				    BookTrapEffect |= FROMOUTSIDE; break;
				case 51: 
				    LevelTrapEffect |= FROMOUTSIDE; break;
				case 52: 
				    QuizTrapEffect |= FROMOUTSIDE; break;
				case 53: 
				    CaptchaProblem |= FROMOUTSIDE; break;
				case 54: 
				    FarlookProblem |= FROMOUTSIDE; break;
				case 55: 
				    RespawnProblem |= FROMOUTSIDE; break;
				case 56: 
				    FastMetabolismEffect |= FROMOUTSIDE; break;
				case 57: 
				    NoReturnEffect |= FROMOUTSIDE; break;
				case 58: 
				    AlwaysEgotypeMonsters |= FROMOUTSIDE; break;
				case 59: 
				    TimeGoesByFaster |= FROMOUTSIDE; break;
				case 60: 
				    FoodIsAlwaysRotten |= FROMOUTSIDE; break;
				case 61: 
				    AllSkillsUnskilled |= FROMOUTSIDE; break;
				case 62: 
				    AllStatsAreLower |= FROMOUTSIDE; break;
				case 63: 
				    PlayerCannotTrainSkills |= FROMOUTSIDE; break;
				case 64: 
				    PlayerCannotExerciseStats |= FROMOUTSIDE; break;
				case 65: 
				    TurnLimitation |= FROMOUTSIDE; break;
				case 66: 
				    WeakSight |= FROMOUTSIDE; break;
				case 67: 
				    RandomMessages |= FROMOUTSIDE; break;
				case 68: 
				    Desecration |= FROMOUTSIDE; break;
				case 69: 
				    StarvationEffect |= FROMOUTSIDE; break;
				case 70: 
				    NoDropsEffect |= FROMOUTSIDE; break;
				case 71: 
				    LowEffects |= FROMOUTSIDE; break;
				case 72: 
				    InvisibleTrapsEffect |= FROMOUTSIDE; break;
				case 73: 
				    GhostWorld |= FROMOUTSIDE; break;
				case 74: 
				    Dehydration |= FROMOUTSIDE; break;
				case 75: 
				    HateTrapEffect |= FROMOUTSIDE; break;
				case 76: 
				    TotterTrapEffect |= FROMOUTSIDE; break;
				case 77: 
				    Nonintrinsics |= FROMOUTSIDE; break;
				case 78: 
				    Dropcurses |= FROMOUTSIDE; break;
				case 79: 
				    Nakedness |= FROMOUTSIDE; break;
				case 80: 
				    Antileveling |= FROMOUTSIDE; break;
				case 81: 
				    ItemStealingEffect |= FROMOUTSIDE; break;
				case 82: 
				    Rebellions |= FROMOUTSIDE; break;
				case 83: 
				    CrapEffect |= FROMOUTSIDE; break;
				case 84: 
				    ProjectilesMisfire |= FROMOUTSIDE; break;
				case 85: 
				    WallTrapping |= FROMOUTSIDE; break;
				case 86: 
				    DisconnectedStairs |= FROMOUTSIDE; break;
				case 87: 
				    InterfaceScrewed |= FROMOUTSIDE; break;
				case 88: 
				    Bossfights |= FROMOUTSIDE; break;
				case 89: 
				    EntireLevelMode |= FROMOUTSIDE; break;
				case 90: 
				    BonesLevelChange |= FROMOUTSIDE; break;
				case 91: 
				    AutocursingEquipment |= FROMOUTSIDE; break;
				case 92: 
				    HighlevelStatus |= FROMOUTSIDE; break;
				case 93: 
				    SpellForgetting |= FROMOUTSIDE; break;
				case 94: 
				    SoundEffectBug |= FROMOUTSIDE; break;
				case 95: 
				    TimerunBug |= FROMOUTSIDE; break;
				case 96:
				    LootcutBug |= FROMOUTSIDE; break;
				case 97:
				    MonsterSpeedBug |= FROMOUTSIDE; break;
				case 98:
				    ScalingBug |= FROMOUTSIDE; break;
				case 99:
				    EnmityBug |= FROMOUTSIDE; break;
				case 100:
				    WhiteSpells |= FROMOUTSIDE; break;
				case 101:
				    CompleteGraySpells |= FROMOUTSIDE; break;
				case 102:
				    QuasarVision |= FROMOUTSIDE; break;
				case 103:
				    MommaBugEffect |= FROMOUTSIDE; break;
				case 104:
				    HorrorBugEffect |= FROMOUTSIDE; break;
				case 105:
				    ArtificerBug |= FROMOUTSIDE; break;
				case 106:
				    WereformBug |= FROMOUTSIDE; break;
				case 107:
				    NonprayerBug |= FROMOUTSIDE; break;
				case 108:
				    EvilPatchEffect |= FROMOUTSIDE; break;
				case 109:
				    HardModeEffect |= FROMOUTSIDE; break;
				case 110:
				    SecretAttackBug |= FROMOUTSIDE; break;
				case 111:
				    EaterBugEffect |= FROMOUTSIDE; break;
				case 112:
				    CovetousnessBug |= FROMOUTSIDE; break;
				case 113:
				    NotSeenBug |= FROMOUTSIDE; break;
				case 114:
				    DarkModeBug |= FROMOUTSIDE; break;
				case 115:
				    AntisearchEffect |= FROMOUTSIDE; break;
				case 116:
				    HomicideEffect |= FROMOUTSIDE; break;
				case 117:
				    NastynationBug |= FROMOUTSIDE; break;
				case 118:
				    WakeupCallBug |= FROMOUTSIDE; break;
				case 119:
				    GrayoutBug |= FROMOUTSIDE; break;
				case 120:
				    GrayCenterBug |= FROMOUTSIDE; break;
				case 121:
				    CheckerboardBug |= FROMOUTSIDE; break;
				case 122:
				    ClockwiseSpinBug |= FROMOUTSIDE; break;
				case 123:
				    CounterclockwiseSpin |= FROMOUTSIDE; break;
				case 124:
				    LagBugEffect |= FROMOUTSIDE; break;
				case 125:
				    BlesscurseEffect |= FROMOUTSIDE; break;
				case 126:
				    DeLightBug |= FROMOUTSIDE; break;
				case 127:
				    DischargeBug |= FROMOUTSIDE; break;
				case 128:
				    TrashingBugEffect |= FROMOUTSIDE; break;
				case 129:
				    FilteringBug |= FROMOUTSIDE; break;
				case 130:
				    DeformattingBug |= FROMOUTSIDE; break;
				case 131:
				    FlickerStripBug |= FROMOUTSIDE; break;
				case 132:
				    UndressingEffect |= FROMOUTSIDE; break;
				case 133:
				    Hyperbluewalls |= FROMOUTSIDE; break;
				case 134:
				    NoliteBug |= FROMOUTSIDE; break;
				case 135:
				    ParanoiaBugEffect |= FROMOUTSIDE; break;
				case 136:
				    FleecescriptBug |= FROMOUTSIDE; break;
				case 137:
				    InterruptEffect |= FROMOUTSIDE; break;
				case 138:
				    DustbinBug |= FROMOUTSIDE; break;
				case 139:
				    ManaBatteryBug |= FROMOUTSIDE; break;
				case 140:
				    Monsterfingers |= FROMOUTSIDE; break;
				case 141:
				    MiscastBug |= FROMOUTSIDE; break;
				case 142:
				    MessageSuppression |= FROMOUTSIDE; break;
				case 143:
				    StuckAnnouncement |= FROMOUTSIDE; break;
				case 144:
				    BloodthirstyEffect |= FROMOUTSIDE; break;
				case 145:
				    MaximumDamageBug |= FROMOUTSIDE; break;
				case 146:
				    LatencyBugEffect |= FROMOUTSIDE; break;
				case 147:
				    StarlitBug |= FROMOUTSIDE; break;
				case 148:
				    KnowledgeBug |= FROMOUTSIDE; break;
				case 149:
				    HighscoreBug |= FROMOUTSIDE; break;
				case 150:
				    PinkSpells |= FROMOUTSIDE; break;
				case 151:
				    GreenSpells |= FROMOUTSIDE; break;
				case 152:
				    EvencoreEffect |= FROMOUTSIDE; break;
				case 153:
				    UnderlayerBug |= FROMOUTSIDE; break;
				case 154:
				    DamageMeterBug |= FROMOUTSIDE; break;
				case 155:
				    ArbitraryWeightBug |= FROMOUTSIDE; break;
				case 156:
				    FuckedInfoBug |= FROMOUTSIDE; break;
				case 157:
				    BlackSpells |= FROMOUTSIDE; break;
				case 158:
				    CyanSpells |= FROMOUTSIDE; break;
				case 159:
				    HeapEffectBug |= FROMOUTSIDE; break;
				case 160:
				    BlueSpells |= FROMOUTSIDE; break;
				case 161:
				    TronEffect |= FROMOUTSIDE; break;
				case 162:
				    RedSpells |= FROMOUTSIDE; break;
				case 163:
				    TooHeavyEffect |= FROMOUTSIDE; break;
				case 164:
				    ElongationBug |= FROMOUTSIDE; break;
				case 165:
				    WrapoverEffect |= FROMOUTSIDE; break;
				case 166:
				    DestructionEffect |= FROMOUTSIDE; break;
				case 167:
				    MeleePrefixBug |= FROMOUTSIDE; break;
				case 168:
				    AutomoreBug |= FROMOUTSIDE; break;
				case 169:
				    UnfairAttackBug |= FROMOUTSIDE; break;
				case 170:
				    OrangeSpells |= FROMOUTSIDE; break;
				case 171:
				    VioletSpells |= FROMOUTSIDE; break;
				case 172:
				    LongingEffect |= FROMOUTSIDE; break;
				case 173:
				    CursedParts |= FROMOUTSIDE; break;
				case 174:
				    Quaversal |= FROMOUTSIDE; break;
				case 175:
				    AppearanceShuffling |= FROMOUTSIDE; break;
				case 176:
				    BrownSpells |= FROMOUTSIDE; break;
				case 177:
				    Choicelessness |= FROMOUTSIDE; break;
				case 178:
				    Goldspells |= FROMOUTSIDE; break;
				case 179:
				    Deprovement |= FROMOUTSIDE; break;
				case 180:
				    InitializationFail |= FROMOUTSIDE; break;
				case 181:
				    GushlushEffect |= FROMOUTSIDE; break;
				case 182:
				    SoiltypeEffect |= FROMOUTSIDE; break;
				case 183:
				    DangerousTerrains |= FROMOUTSIDE; break;
				case 184:
				    FalloutEffect |= FROMOUTSIDE; break;
				case 185:
				    MojibakeEffect |= FROMOUTSIDE; break;
				case 186:
				    GravationEffect |= FROMOUTSIDE; break;
				case 187:
				    UncalledEffect |= FROMOUTSIDE; break;
				case 188:
				    ExplodingDiceEffect |= FROMOUTSIDE; break;
				case 189:
				    PermacurseEffect |= FROMOUTSIDE; break;
				case 190:
				    ShroudedIdentity |= FROMOUTSIDE; break;
				case 191:
				    FeelerGauges |= FROMOUTSIDE; break;
				case 192:
				    LongScrewup |= FROMOUTSIDE; break;
				case 193:
				    WingYellowChange |= FROMOUTSIDE; break;
				case 194:
				    LifeSavingBug |= FROMOUTSIDE; break;
				case 195:
				    CurseuseEffect |= FROMOUTSIDE; break;
				case 196:
				    CutNutritionEffect |= FROMOUTSIDE; break;
				case 197:
				    SkillLossEffect |= FROMOUTSIDE; break;
				case 198:
				    AutopilotEffect |= FROMOUTSIDE; break;
				case 199:
				    MysteriousForceActive |= FROMOUTSIDE; break;
				case 200:
				    MonsterGlyphChange |= FROMOUTSIDE; break;
				case 201:
				    ChangingDirectives |= FROMOUTSIDE; break;
				case 202:
				    ContainerKaboom |= FROMOUTSIDE; break;
				case 203:
				    StealDegrading |= FROMOUTSIDE; break;
				case 204:
				    LeftInventoryBug |= FROMOUTSIDE; break;
				case 205:
				    FluctuatingSpeed |= FROMOUTSIDE; break;
				case 206:
				    TarmuStrokingNora |= FROMOUTSIDE; break;
				case 207:
				    FailureEffects |= FROMOUTSIDE; break;
				case 208:
				    BrightCyanSpells |= FROMOUTSIDE; break;
				case 209:
				    FrequentationSpawns |= FROMOUTSIDE; break;
				case 210:
				    PetAIScrewed |= FROMOUTSIDE; break;
				case 211:
				    SatanEffect |= FROMOUTSIDE; break;
				case 212:
				    RememberanceEffect |= FROMOUTSIDE; break;
				case 213:
				    PokelieEffect |= FROMOUTSIDE; break;
				case 214:
				    AlwaysAutopickup |= FROMOUTSIDE; break;
				case 215:
				    DywypiProblem |= FROMOUTSIDE; break;
				case 216:
				    SilverSpells |= FROMOUTSIDE; break;
				case 217:
				    MetalSpells |= FROMOUTSIDE; break;
				case 218:
				    PlatinumSpells |= FROMOUTSIDE; break;
				case 219:
				    ManlerEffect |= FROMOUTSIDE; break;
				case 220:
				    DoorningEffect |= FROMOUTSIDE; break;
				case 221:
				    NownsibleEffect |= FROMOUTSIDE; break;
				case 222:
				    ElmStreetEffect |= FROMOUTSIDE; break;
				case 223:
				    MonnoiseEffect |= FROMOUTSIDE; break;
				case 224:
				    RangCallEffect |= FROMOUTSIDE; break;
				case 225:
				    RecurringSpellLoss |= FROMOUTSIDE; break;
				case 226:
				    AntitrainingEffect |= FROMOUTSIDE; break;
				case 227:
				    TechoutBug |= FROMOUTSIDE; break;
				case 228:
				    StatDecay |= FROMOUTSIDE; break;
				case 229:
				    Movemork |= FROMOUTSIDE; break;
			}

		}

		if (Race_if(PM_RODNEYAN) && u.ulevel > u.urmaxlvl) {

		u.urmaxlvl = u.ulevel;

		if (!rn2(2)) {
			ubookz = mkobj(SPBOOK_CLASS, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book appeared at your feet!"); }

		}

		if (Race_if(PM_ASGARDIAN) && u.ulevel > u.urmaxlvl) {

		u.urmaxlvl = u.ulevel;

		if (!rn2(3)) { switch (rnd(52)) {

			case 1: 
			case 2: 
			case 3: 
			    HFire_resistance |= FROMOUTSIDE; pline("Got fire resistance!"); break;
			case 4: 
			case 5: 
			case 6: 
			    HCold_resistance |= FROMOUTSIDE; pline("Got cold resistance!"); break;
			case 7: 
			case 8: 
			case 9: 
			    HSleep_resistance |= FROMOUTSIDE; pline("Got sleep resistance!"); break;
			case 10: 
			case 11: 
			    HDisint_resistance |= FROMOUTSIDE; pline("Got disintegration resistance!"); break;
			case 12: 
			case 13: 
			case 14: 
			    HShock_resistance |= FROMOUTSIDE; pline("Got shock resistance!"); break;
			case 15: 
			case 16: 
			case 17: 
			    HPoison_resistance |= FROMOUTSIDE; pline("Got poison resistance!"); break;
			case 18: 
			    HDrain_resistance |= FROMOUTSIDE; pline("Got drain resistance!"); break;
			case 19: 
			    HSick_resistance |= FROMOUTSIDE; pline("Got sickness resistance!"); break;
			case 20: 
			    HAcid_resistance |= FROMOUTSIDE; pline("Got acid resistance!"); break;
			case 21: 
			case 22: 
			    HHunger |= FROMOUTSIDE; pline("You start to hunger rapidly!"); break;
			case 23: 
			case 24: 
			    HSee_invisible |= FROMOUTSIDE; pline("Got see invisible!"); break;
			case 25: 
			    HTelepat |= FROMOUTSIDE; pline("Got telepathy!"); break;
			case 26: 
			case 27: 
			    HWarning |= FROMOUTSIDE; pline("Got warning!"); break;
			case 28: 
			case 29: 
			    HSearching |= FROMOUTSIDE; pline("Got searching!"); break;
			case 30: 
			case 31: 
			    HStealth |= FROMOUTSIDE; pline("Got stealth!"); break;
			case 32: 
			case 33: 
			    HAggravate_monster |= FROMOUTSIDE; pline("You aggravate monsters!"); break;
			case 34: 
			    HConflict |= FROMOUTSIDE; pline("You start causing conflict!"); break;
			case 35: 
			case 36: 
			    HTeleportation |= FROMOUTSIDE; pline("Got teleportitis!"); break;
			case 37: 
			    HTeleport_control |= FROMOUTSIDE; pline("Got teleport control!"); break;
			case 38: 
			    HFlying |= FROMOUTSIDE; pline("Got flying!"); break;
			case 39: 
			    HSwimming |= FROMOUTSIDE; pline("Got swimming!"); break;
			case 40: 
			    HMagical_breathing |= FROMOUTSIDE; pline("Got unbreathing!"); break;
			case 41: 
			    HSlow_digestion |= FROMOUTSIDE; pline("Got slow digestion!"); break;
			case 42: 
			case 43: 
			    HRegeneration |= FROMOUTSIDE; pline("Got regeneration!"); break;
			case 44: 
			    HPolymorph |= FROMOUTSIDE; pline("Got polymorphitis!"); break;
			case 45: 
			    HPolymorph_control |= FROMOUTSIDE; pline("Got polymorph control!"); break;
			case 46: 
			case 47: 
			    HFast |= FROMOUTSIDE; pline("Got speed!"); break;
			case 48: 
			    HInvis |= FROMOUTSIDE; pline("Got invisibility!"); break;
			case 49: 
			    HManaleech |= FROMOUTSIDE; pline("Got manaleech!"); break;
			case 50: 
			    HPeacevision |= FROMOUTSIDE; pline("Got peacevision!"); break;

			default:
				break;

		    }

		  }

		}

		if (Role_if(PM_WILD_TALENT) && u.ulevel > u.urmaxlvlF) {

		u.urmaxlvlF = u.ulevel;

		if (!rn2(4)) { switch (rnd(52)) {

			case 1: 
			case 2: 
			case 3: 
			    HFire_resistance |= FROMOUTSIDE; pline("Got fire resistance!"); break;
			case 4: 
			case 5: 
			case 6: 
			    HCold_resistance |= FROMOUTSIDE; pline("Got cold resistance!"); break;
			case 7: 
			case 8: 
			case 9: 
			    HSleep_resistance |= FROMOUTSIDE; pline("Got sleep resistance!"); break;
			case 10: 
			case 11: 
			    HDisint_resistance |= FROMOUTSIDE; pline("Got disintegration resistance!"); break;
			case 12: 
			case 13: 
			case 14: 
			    HShock_resistance |= FROMOUTSIDE; pline("Got shock resistance!"); break;
			case 15: 
			case 16: 
			case 17: 
			    HPoison_resistance |= FROMOUTSIDE; pline("Got poison resistance!"); break;
			case 18: 
			    HDrain_resistance |= FROMOUTSIDE; pline("Got drain resistance!"); break;
			case 19: 
			    HSick_resistance |= FROMOUTSIDE; pline("Got sickness resistance!"); break;
			case 20: 
			    HAcid_resistance |= FROMOUTSIDE; pline("Got acid resistance!"); break;
			case 21: 
			case 22: 
			    HHunger |= FROMOUTSIDE; pline("You start to hunger rapidly!"); break;
			case 23: 
			case 24: 
			    HSee_invisible |= FROMOUTSIDE; pline("Got see invisible!"); break;
			case 25: 
			    HTelepat |= FROMOUTSIDE; pline("Got telepathy!"); break;
			case 26: 
			case 27: 
			    HWarning |= FROMOUTSIDE; pline("Got warning!"); break;
			case 28: 
			case 29: 
			    HSearching |= FROMOUTSIDE; pline("Got searching!"); break;
			case 30: 
			case 31: 
			    HStealth |= FROMOUTSIDE; pline("Got stealth!"); break;
			case 32: 
			case 33: 
			    HAggravate_monster |= FROMOUTSIDE; pline("You aggravate monsters!"); break;
			case 34: 
			    HConflict |= FROMOUTSIDE; pline("You start causing conflict!"); break;
			case 35: 
			case 36: 
			    HTeleportation |= FROMOUTSIDE; pline("Got teleportitis!"); break;
			case 37: 
			    HTeleport_control |= FROMOUTSIDE; pline("Got teleport control!"); break;
			case 38: 
			    HFlying |= FROMOUTSIDE; pline("Got flying!"); break;
			case 39: 
			    HSwimming |= FROMOUTSIDE; pline("Got swimming!"); break;
			case 40: 
			    HMagical_breathing |= FROMOUTSIDE; pline("Got unbreathing!"); break;
			case 41: 
			    HSlow_digestion |= FROMOUTSIDE; pline("Got slow digestion!"); break;
			case 42: 
			case 43: 
			    HRegeneration |= FROMOUTSIDE; pline("Got regeneration!"); break;
			case 44: 
			    HPolymorph |= FROMOUTSIDE; pline("Got polymorphitis!"); break;
			case 45: 
			    HPolymorph_control |= FROMOUTSIDE; pline("Got polymorph control!"); break;
			case 46: 
			case 47: 
			    HFast |= FROMOUTSIDE; pline("Got speed!"); break;
			case 48: 
			    HInvis |= FROMOUTSIDE; pline("Got invisibility!"); break;
			case 49: 
			    HManaleech |= FROMOUTSIDE; pline("Got manaleech!"); break;
			case 50: 
			    HPeacevision |= FROMOUTSIDE; pline("Got peacevision!"); break;

			default:
				break;

		    }

		  }

		}

		if (Race_if(PM_MISSINGNO) && u.ulevel > u.urmaxlvl) {

		u.urmaxlvl = u.ulevel;

		if (!rn2(3)) { switch (rnd(176)) {

			case 1: 
			case 2: 
			case 3: 
				if (!tech_known(T_BERSERK)) {    	learntech(T_BERSERK, FROMOUTSIDE, 1);
			    	You("learn how to perform berserk!");
				}
				break;
			case 4: 
			case 5: 
			case 6: 
				if (!tech_known(T_KIII)) {    	learntech(T_KIII, FROMOUTSIDE, 1);
			    	You("learn how to perform kiii!");
				}
				break;
			case 7: 
			case 8: 
			case 9: 
				if (!tech_known(T_RESEARCH)) {    	learntech(T_RESEARCH, FROMOUTSIDE, 1);
			    	You("learn how to perform research!");
				}
				break;
			case 10: 
			case 11: 
			case 12: 
				if (!tech_known(T_SURGERY)) {    	learntech(T_SURGERY, FROMOUTSIDE, 1);
			    	You("learn how to perform surgery!");
				}
				break;
			case 13: 
			case 14: 
			case 15: 
				if (!tech_known(T_REINFORCE)) {    	learntech(T_REINFORCE, FROMOUTSIDE, 1);
			    	You("learn how to perform reinforce memory!");
				}
				break;
			case 16: 
			case 17:
			case 18: 
				if (!tech_known(T_FLURRY)) {    	learntech(T_FLURRY, FROMOUTSIDE, 1);
			    	You("learn how to perform missile flurry!");
				}
				break;
			case 19: 
			case 20: 
			case 21: 
				if (!tech_known(T_PRACTICE)) {    	learntech(T_PRACTICE, FROMOUTSIDE, 1);
			    	You("learn how to perform weapon practice!");
				}
				break;
			case 22: 
			case 23: 
			case 24: 
				if (!tech_known(T_EVISCERATE)) {    	learntech(T_EVISCERATE, FROMOUTSIDE, 1);
			    	You("learn how to perform eviscerate!");
				}
				break;
			case 25: 
			case 26: 
			case 27: 
				if (!tech_known(T_HEAL_HANDS)) {    	learntech(T_HEAL_HANDS, FROMOUTSIDE, 1);
			    	You("learn how to perform healing hands!");
				}
				break;
			case 28: 
			case 29: 
			case 30: 
				if (!tech_known(T_CALM_STEED)) {    	learntech(T_CALM_STEED, FROMOUTSIDE, 1);
			    	You("learn how to perform calm steed!");
				}
				break;
			case 31: 
			case 32: 
			case 33: 
				if (!tech_known(T_TURN_UNDEAD)) {    	learntech(T_TURN_UNDEAD, FROMOUTSIDE, 1);
			    	You("learn how to perform turn undead!");
				}
				break;
			case 34: 
			case 35: 
			case 36: 
				if (!tech_known(T_VANISH)) {    	learntech(T_VANISH, FROMOUTSIDE, 1);
			    	You("learn how to perform vanish!");
				}
				break;
			case 37: 
			case 38: 
			case 39: 
				if (!tech_known(T_CUTTHROAT)) {    	learntech(T_CUTTHROAT, FROMOUTSIDE, 1);
			    	You("learn how to perform cutthroat!");
				}
				break;
			case 40: 
			case 41: 
				if (!tech_known(T_BLESSING)) {    	learntech(T_BLESSING, FROMOUTSIDE, 1);
			    	You("learn how to perform blessing!");
				}
				break;
			case 42: 
			case 43: 
			case 44: 
				if (!tech_known(T_E_FIST)) {    	learntech(T_E_FIST, FROMOUTSIDE, 1);
			    	You("learn how to perform elemental fist!");
				}
				break;
			case 45: 
			case 46: 
			case 47: 
				if (!tech_known(T_PRIMAL_ROAR)) {    	learntech(T_PRIMAL_ROAR, FROMOUTSIDE, 1);
			    	You("learn how to perform primal roar!");
				}
				break;
			case 48: 
			case 49: 
				if (!tech_known(T_LIQUID_LEAP)) {    	learntech(T_LIQUID_LEAP, FROMOUTSIDE, 1);
			    	You("learn how to perform liquid leap!");
				}
				break;
			case 50: 
			case 51: 
			case 52: 
				if (!tech_known(T_CRIT_STRIKE)) {    	learntech(T_CRIT_STRIKE, FROMOUTSIDE, 1);
			    	You("learn how to perform critical strike!");
				}
				break;
			case 53: 
			case 54: 
			case 55: 
				if (!tech_known(T_SIGIL_CONTROL)) {    	learntech(T_SIGIL_CONTROL, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of control!");
				}
				break;
			case 56: 
			case 57: 
			case 58: 
				if (!tech_known(T_SIGIL_TEMPEST)) {    	learntech(T_SIGIL_TEMPEST, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of tempest!");
				}
				break;
			case 59: 
			case 60: 
			case 61: 
				if (!tech_known(T_SIGIL_DISCHARGE)) {    	learntech(T_SIGIL_DISCHARGE, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of discharge!");
				}
				break;
			case 62: 
			case 63: 
			case 64: 
				if (!tech_known(T_RAISE_ZOMBIES)) {    	learntech(T_RAISE_ZOMBIES, FROMOUTSIDE, 1);
			    	You("learn how to perform raise zombies!");
				}
				break;
			case 65: 
				if (!tech_known(T_REVIVE)) {    	learntech(T_REVIVE, FROMOUTSIDE, 1);
			    	You("learn how to perform revivification!");
				}
				break;
			case 66: 
			case 67: 
			case 68: 
				if (!tech_known(T_WARD_FIRE)) {    	learntech(T_WARD_FIRE, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against fire!");
				}
				break;
			case 69: 
			case 70: 
			case 71: 
				if (!tech_known(T_WARD_COLD)) {    	learntech(T_WARD_COLD, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against cold!");
				}
				break;
			case 72: 
			case 73: 
			case 74: 
				if (!tech_known(T_WARD_ELEC)) {    	learntech(T_WARD_ELEC, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against electricity!");
				}
				break;
			case 75: 
			case 76: 
			case 77: 
				if (!tech_known(T_TINKER)) {    	learntech(T_TINKER, FROMOUTSIDE, 1);
			    	You("learn how to perform tinker!");
				}
				break;
			case 78: 
			case 79: 
			case 80: 
				if (!tech_known(T_RAGE)) {    	learntech(T_RAGE, FROMOUTSIDE, 1);
			    	You("learn how to perform rage eruption!");
				}
				break;
			case 81: 
			case 82: 
			case 83: 
				if (!tech_known(T_BLINK)) {    	learntech(T_BLINK, FROMOUTSIDE, 1);
			    	You("learn how to perform blink!");
				}
				break;
			case 84: 
			case 85: 
			case 86: 
				if (!tech_known(T_CHI_STRIKE)) {    	learntech(T_CHI_STRIKE, FROMOUTSIDE, 1);
			    	You("learn how to perform chi strike!");
				}
				break;
			case 87: 
			case 88: 
			case 89: 
				if (!tech_known(T_DRAW_ENERGY)) {    	learntech(T_DRAW_ENERGY, FROMOUTSIDE, 1);
			    	You("learn how to perform draw energy!");
				}
				break;
			case 90: 
			case 91: 
			case 92: 
				if (!tech_known(T_CHI_HEALING)) {    	learntech(T_CHI_HEALING, FROMOUTSIDE, 1);
			    	You("learn how to perform chi healing!");
				}
				break;
			case 93: 
			case 94: 
			case 95: 
				if (!tech_known(T_DAZZLE)) {    	learntech(T_DAZZLE, FROMOUTSIDE, 1);
			    	You("learn how to perform dazzle!");
				}
				break;
			case 96: 
			case 97: 
			case 98: 
				if (!tech_known(T_BLITZ)) {    	learntech(T_BLITZ, FROMOUTSIDE, 1);
			    	You("learn how to perform chained blitz!");
				}
				break;
			case 99: 
			case 100: 
			case 101: 
				if (!tech_known(T_PUMMEL)) {    	learntech(T_PUMMEL, FROMOUTSIDE, 1);
			    	You("learn how to perform pummel!");
				}
				break;
			case 102: 
			case 103: 
			case 104: 
				if (!tech_known(T_G_SLAM)) {    	learntech(T_G_SLAM, FROMOUTSIDE, 1);
			    	You("learn how to perform ground slam!");
				}
				break;
			case 105: 
			case 106: 
			case 107: 
				if (!tech_known(T_DASH)) {    	learntech(T_DASH, FROMOUTSIDE, 1);
			    	You("learn how to perform air dash!");
				}
				break;
			case 108: 
			case 109: 
			case 110: 
				if (!tech_known(T_POWER_SURGE)) {    	learntech(T_POWER_SURGE, FROMOUTSIDE, 1);
			    	You("learn how to perform power surge!");
				}
				break;
			case 111: 
			case 112: 
			case 113: 
				if (!tech_known(T_SPIRIT_BOMB)) {    	learntech(T_SPIRIT_BOMB, FROMOUTSIDE, 1);
			    	You("learn how to perform spirit bomb!");
				}
				break;
			case 114: 
			case 115: 
			case 116: 
				if (!tech_known(T_DRAW_BLOOD)) {    	learntech(T_DRAW_BLOOD, FROMOUTSIDE, 1);
			    	You("learn how to perform draw blood!");
				}
				break;
			case 117: 
				if (!tech_known(T_WORLD_FALL)) {    	learntech(T_WORLD_FALL, FROMOUTSIDE, 1);
			    	You("learn how to perform world fall!");
				}
				break;
			case 118: 
			case 119: 
			case 120: 
				if (!tech_known(T_CREATE_AMMO)) {    	learntech(T_CREATE_AMMO, FROMOUTSIDE, 1);
			    	You("learn how to perform create ammo!");
				}
				break;
			case 121: 
			case 122: 
			case 123: 
				if (!tech_known(T_POKE_BALL)) {    	learntech(T_POKE_BALL, FROMOUTSIDE, 1);
			    	You("learn how to perform poke ball!");
				}
				break;
			case 124: 
			case 125: 
			case 126: 
				if (!tech_known(T_ATTIRE_CHARM)) {    	learntech(T_ATTIRE_CHARM, FROMOUTSIDE, 1);
			    	You("learn how to perform attire charm!");
				}
				break;
			case 127: 
			case 128: 
			case 129: 
				if (!tech_known(T_SUMMON_TEAM_ANT)) {    	learntech(T_SUMMON_TEAM_ANT, FROMOUTSIDE, 1);
			    	You("learn how to perform summon team ant!");
				}
				break;
			case 130: 
			case 131: 
			case 132: 
				if (!tech_known(T_JEDI_JUMP)) {    	learntech(T_JEDI_JUMP, FROMOUTSIDE, 1);
			    	You("learn how to perform jedi jump!");
				}
				break;
			case 133: 
			case 134: 
			case 135: 
				if (!tech_known(T_CHARGE_SABER)) {    	learntech(T_CHARGE_SABER, FROMOUTSIDE, 1);
			    	You("learn how to perform charge saber!");
				}
				break;
			case 136: 
			case 137: 
			case 138: 
				if (!tech_known(T_TELEKINESIS)) {    	learntech(T_TELEKINESIS, FROMOUTSIDE, 1);
			    	You("learn how to perform telekinesis!");
				}
				break;
			case 139: 
				if (!tech_known(T_EGG_BOMB)) {    	learntech(T_EGG_BOMB, FROMOUTSIDE, 1);
			    	You("learn how to perform egg bomb!");
				}
				break;
			case 140: 
			case 141: 
			case 142: 
				if (!tech_known(T_BOOZE)) {    	learntech(T_BOOZE, FROMOUTSIDE, 1);
			    	You("learn how to perform booze!");
				}
				break;

			case 143: 
			case 144: 
			case 145: 
				if (!tech_known(T_IRON_SKIN)) {    	learntech(T_IRON_SKIN, FROMOUTSIDE, 1);
			    	You("learn how to perform iron skin!");
				}
				break;
			case 146: 
				if (!tech_known(T_POLYFORM)) {    	learntech(T_POLYFORM, FROMOUTSIDE, 1);
			    	You("learn how to perform polyform!");
				}
				break;
			case 147: 
			case 148: 
			case 149: 
				if (!tech_known(T_CONCENTRATING)) {    	learntech(T_CONCENTRATING, FROMOUTSIDE, 1);
			    	You("learn how to perform concentrating!");
				}
				break;
			case 150: 
				if (!tech_known(T_SUMMON_PET)) {    	learntech(T_SUMMON_PET, FROMOUTSIDE, 1);
			    	You("learn how to perform summon pet!");
				}
				break;
			case 151: 
			case 152: 
				if (!tech_known(T_DOUBLE_THROWNAGE)) {    	learntech(T_DOUBLE_THROWNAGE, FROMOUTSIDE, 1);
			    	You("learn how to perform double thrownage!");
				}
				break;
			case 153: 
			case 154: 
			case 155: 
				if (!tech_known(T_SHIELD_BASH)) {    	learntech(T_SHIELD_BASH, FROMOUTSIDE, 1);
			    	You("learn how to perform shield bash!");
				}
				break;
			case 156: 
			case 157: 
				if (!tech_known(T_RECHARGE)) {    	learntech(T_RECHARGE, FROMOUTSIDE, 1);
			    	You("learn how to perform recharge!");
				}
				break;
			case 158: 
			case 159: 
			case 160: 
				if (!tech_known(T_SPIRITUALITY_CHECK)) {    	learntech(T_SPIRITUALITY_CHECK, FROMOUTSIDE, 1);
			    	You("learn how to perform spirituality check!");
				}
				break;
			case 161: 
				if (!tech_known(T_EDDY_WIND)) {    	learntech(T_EDDY_WIND, FROMOUTSIDE, 1);
			    	You("learn how to perform eddy wind!");
				}
				break;
			case 162: 
				if (!tech_known(T_BLOOD_RITUAL)) {    	learntech(T_BLOOD_RITUAL, FROMOUTSIDE, 1);
			    	You("learn how to perform blood ritual!");
				}
				break;
			case 163: 
				if (!tech_known(T_ENT_S_POTION)) {    	learntech(T_ENT_S_POTION, FROMOUTSIDE, 1);
			    	You("learn how to perform ent's potion!");
				}
				break;
			case 164: 
			case 165: 
				if (!tech_known(T_LUCKY_GAMBLE)) {    	learntech(T_LUCKY_GAMBLE, FROMOUTSIDE, 1);
			    	You("learn how to perform lucky gamble!");
				}
				break;
			case 166: 
				if (!tech_known(T_DECONTAMINATE)) {    	learntech(T_DECONTAMINATE, FROMOUTSIDE, 1);
			    	You("learn how to perform decontaminate!");
				}
				break;
			case 167: 
				if (!tech_known(T_WONDERSPELL)) {    	learntech(T_WONDERSPELL, FROMOUTSIDE, 1);
			    	You("learn how to perform wonderspell!");
				}
				break;
			case 168: 
				if (!tech_known(T_RESET_TECHNIQUE)) {    	learntech(T_RESET_TECHNIQUE, FROMOUTSIDE, 1);
			    	You("learn how to perform reset technique!");
				}
				break;
			case 169: 
				if (!tech_known(T_DIAMOND_BARRIER)) {    	learntech(T_DIAMOND_BARRIER, FROMOUTSIDE, 1);
			    	You("learn how to perform diamond barrier!");
				}
				break;

			default:
				break;

		    }

		  }
		}

		if (Role_if(PM_DQ_SLIME) && u.ulevel > u.urmaxlvlC) {

		u.urmaxlvlC = u.ulevel;

			int skillimprove = randomgoodskill();

			if (P_MAX_SKILL(skillimprove) == P_ISRESTRICTED) {
				unrestrict_weapon_skill(skillimprove);
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (P_MAX_SKILL(skillimprove) == P_UNSKILLED) {
				unrestrict_weapon_skill(skillimprove);
				P_MAX_SKILL(skillimprove) = P_BASIC;
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (rn2(2) && P_MAX_SKILL(skillimprove) == P_BASIC) {
				P_MAX_SKILL(skillimprove) = P_SKILLED;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(4) && P_MAX_SKILL(skillimprove) == P_SKILLED) {
				P_MAX_SKILL(skillimprove) = P_EXPERT;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(10) && P_MAX_SKILL(skillimprove) == P_EXPERT) {
				P_MAX_SKILL(skillimprove) = P_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(100) && P_MAX_SKILL(skillimprove) == P_MASTER) {
				P_MAX_SKILL(skillimprove) = P_GRAND_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(200) && P_MAX_SKILL(skillimprove) == P_GRAND_MASTER) {
				P_MAX_SKILL(skillimprove) = P_SUPREME_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else pline("Unfortunately, you feel no different than before.");

		if (u.urmaxlvlC >= 10) {

			int skillimprove = randomgoodskill();

			if (P_MAX_SKILL(skillimprove) == P_ISRESTRICTED) {
				unrestrict_weapon_skill(skillimprove);
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (P_MAX_SKILL(skillimprove) == P_UNSKILLED) {
				unrestrict_weapon_skill(skillimprove);
				P_MAX_SKILL(skillimprove) = P_BASIC;
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (rn2(2) && P_MAX_SKILL(skillimprove) == P_BASIC) {
				P_MAX_SKILL(skillimprove) = P_SKILLED;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(4) && P_MAX_SKILL(skillimprove) == P_SKILLED) {
				P_MAX_SKILL(skillimprove) = P_EXPERT;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(10) && P_MAX_SKILL(skillimprove) == P_EXPERT) {
				P_MAX_SKILL(skillimprove) = P_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(100) && P_MAX_SKILL(skillimprove) == P_MASTER) {
				P_MAX_SKILL(skillimprove) = P_GRAND_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(200) && P_MAX_SKILL(skillimprove) == P_GRAND_MASTER) {
				P_MAX_SKILL(skillimprove) = P_SUPREME_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else pline("Unfortunately, you feel no different than before.");

		}

		if (u.urmaxlvlC >= 20) {

			int skillimprove = randomgoodskill();

			if (P_MAX_SKILL(skillimprove) == P_ISRESTRICTED) {
				unrestrict_weapon_skill(skillimprove);
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (P_MAX_SKILL(skillimprove) == P_UNSKILLED) {
				unrestrict_weapon_skill(skillimprove);
				P_MAX_SKILL(skillimprove) = P_BASIC;
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (rn2(2) && P_MAX_SKILL(skillimprove) == P_BASIC) {
				P_MAX_SKILL(skillimprove) = P_SKILLED;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(4) && P_MAX_SKILL(skillimprove) == P_SKILLED) {
				P_MAX_SKILL(skillimprove) = P_EXPERT;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(10) && P_MAX_SKILL(skillimprove) == P_EXPERT) {
				P_MAX_SKILL(skillimprove) = P_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(100) && P_MAX_SKILL(skillimprove) == P_MASTER) {
				P_MAX_SKILL(skillimprove) = P_GRAND_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(200) && P_MAX_SKILL(skillimprove) == P_GRAND_MASTER) {
				P_MAX_SKILL(skillimprove) = P_SUPREME_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else pline("Unfortunately, you feel no different than before.");

		}

		if (u.urmaxlvlC >= 30) {

			int skillimprove = randomgoodskill();

			if (P_MAX_SKILL(skillimprove) == P_ISRESTRICTED) {
				unrestrict_weapon_skill(skillimprove);
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (P_MAX_SKILL(skillimprove) == P_UNSKILLED) {
				unrestrict_weapon_skill(skillimprove);
				P_MAX_SKILL(skillimprove) = P_BASIC;
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (rn2(2) && P_MAX_SKILL(skillimprove) == P_BASIC) {
				P_MAX_SKILL(skillimprove) = P_SKILLED;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(4) && P_MAX_SKILL(skillimprove) == P_SKILLED) {
				P_MAX_SKILL(skillimprove) = P_EXPERT;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(10) && P_MAX_SKILL(skillimprove) == P_EXPERT) {
				P_MAX_SKILL(skillimprove) = P_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(100) && P_MAX_SKILL(skillimprove) == P_MASTER) {
				P_MAX_SKILL(skillimprove) = P_GRAND_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(200) && P_MAX_SKILL(skillimprove) == P_GRAND_MASTER) {
				P_MAX_SKILL(skillimprove) = P_SUPREME_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else pline("Unfortunately, you feel no different than before.");

			skillimprove = randomgoodskill();

			if (P_MAX_SKILL(skillimprove) == P_ISRESTRICTED) {
				unrestrict_weapon_skill(skillimprove);
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (P_MAX_SKILL(skillimprove) == P_UNSKILLED) {
				unrestrict_weapon_skill(skillimprove);
				P_MAX_SKILL(skillimprove) = P_BASIC;
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (rn2(2) && P_MAX_SKILL(skillimprove) == P_BASIC) {
				P_MAX_SKILL(skillimprove) = P_SKILLED;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(4) && P_MAX_SKILL(skillimprove) == P_SKILLED) {
				P_MAX_SKILL(skillimprove) = P_EXPERT;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(10) && P_MAX_SKILL(skillimprove) == P_EXPERT) {
				P_MAX_SKILL(skillimprove) = P_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(100) && P_MAX_SKILL(skillimprove) == P_MASTER) {
				P_MAX_SKILL(skillimprove) = P_GRAND_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(200) && P_MAX_SKILL(skillimprove) == P_GRAND_MASTER) {
				P_MAX_SKILL(skillimprove) = P_SUPREME_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else pline("Unfortunately, you feel no different than before.");

			skillimprove = randomgoodskill();

			if (P_MAX_SKILL(skillimprove) == P_ISRESTRICTED) {
				unrestrict_weapon_skill(skillimprove);
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (P_MAX_SKILL(skillimprove) == P_UNSKILLED) {
				unrestrict_weapon_skill(skillimprove);
				P_MAX_SKILL(skillimprove) = P_BASIC;
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (rn2(2) && P_MAX_SKILL(skillimprove) == P_BASIC) {
				P_MAX_SKILL(skillimprove) = P_SKILLED;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(4) && P_MAX_SKILL(skillimprove) == P_SKILLED) {
				P_MAX_SKILL(skillimprove) = P_EXPERT;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(10) && P_MAX_SKILL(skillimprove) == P_EXPERT) {
				P_MAX_SKILL(skillimprove) = P_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(100) && P_MAX_SKILL(skillimprove) == P_MASTER) {
				P_MAX_SKILL(skillimprove) = P_GRAND_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(200) && P_MAX_SKILL(skillimprove) == P_GRAND_MASTER) {
				P_MAX_SKILL(skillimprove) = P_SUPREME_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else pline("Unfortunately, you feel no different than before.");

			skillimprove = randomgoodskill();

			if (P_MAX_SKILL(skillimprove) == P_ISRESTRICTED) {
				unrestrict_weapon_skill(skillimprove);
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (P_MAX_SKILL(skillimprove) == P_UNSKILLED) {
				unrestrict_weapon_skill(skillimprove);
				P_MAX_SKILL(skillimprove) = P_BASIC;
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (rn2(2) && P_MAX_SKILL(skillimprove) == P_BASIC) {
				P_MAX_SKILL(skillimprove) = P_SKILLED;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(4) && P_MAX_SKILL(skillimprove) == P_SKILLED) {
				P_MAX_SKILL(skillimprove) = P_EXPERT;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(10) && P_MAX_SKILL(skillimprove) == P_EXPERT) {
				P_MAX_SKILL(skillimprove) = P_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(100) && P_MAX_SKILL(skillimprove) == P_MASTER) {
				P_MAX_SKILL(skillimprove) = P_GRAND_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(200) && P_MAX_SKILL(skillimprove) == P_GRAND_MASTER) {
				P_MAX_SKILL(skillimprove) = P_SUPREME_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else pline("Unfortunately, you feel no different than before.");

			skillimprove = randomgoodskill();

			if (P_MAX_SKILL(skillimprove) == P_ISRESTRICTED) {
				unrestrict_weapon_skill(skillimprove);
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (P_MAX_SKILL(skillimprove) == P_UNSKILLED) {
				unrestrict_weapon_skill(skillimprove);
				P_MAX_SKILL(skillimprove) = P_BASIC;
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (rn2(2) && P_MAX_SKILL(skillimprove) == P_BASIC) {
				P_MAX_SKILL(skillimprove) = P_SKILLED;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(4) && P_MAX_SKILL(skillimprove) == P_SKILLED) {
				P_MAX_SKILL(skillimprove) = P_EXPERT;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(10) && P_MAX_SKILL(skillimprove) == P_EXPERT) {
				P_MAX_SKILL(skillimprove) = P_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(100) && P_MAX_SKILL(skillimprove) == P_MASTER) {
				P_MAX_SKILL(skillimprove) = P_GRAND_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(200) && P_MAX_SKILL(skillimprove) == P_GRAND_MASTER) {
				P_MAX_SKILL(skillimprove) = P_SUPREME_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else pline("Unfortunately, you feel no different than before.");

			skillimprove = randomgoodskill();

			if (P_MAX_SKILL(skillimprove) == P_ISRESTRICTED) {
				unrestrict_weapon_skill(skillimprove);
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (P_MAX_SKILL(skillimprove) == P_UNSKILLED) {
				unrestrict_weapon_skill(skillimprove);
				P_MAX_SKILL(skillimprove) = P_BASIC;
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (rn2(2) && P_MAX_SKILL(skillimprove) == P_BASIC) {
				P_MAX_SKILL(skillimprove) = P_SKILLED;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(4) && P_MAX_SKILL(skillimprove) == P_SKILLED) {
				P_MAX_SKILL(skillimprove) = P_EXPERT;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(10) && P_MAX_SKILL(skillimprove) == P_EXPERT) {
				P_MAX_SKILL(skillimprove) = P_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(100) && P_MAX_SKILL(skillimprove) == P_MASTER) {
				P_MAX_SKILL(skillimprove) = P_GRAND_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(200) && P_MAX_SKILL(skillimprove) == P_GRAND_MASTER) {
				P_MAX_SKILL(skillimprove) = P_SUPREME_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else pline("Unfortunately, you feel no different than before.");

			skillimprove = randomgoodskill();

			if (P_MAX_SKILL(skillimprove) == P_ISRESTRICTED) {
				unrestrict_weapon_skill(skillimprove);
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (P_MAX_SKILL(skillimprove) == P_UNSKILLED) {
				unrestrict_weapon_skill(skillimprove);
				P_MAX_SKILL(skillimprove) = P_BASIC;
				pline("You can now learn the %s skill.", P_NAME(skillimprove));
			} else if (rn2(2) && P_MAX_SKILL(skillimprove) == P_BASIC) {
				P_MAX_SKILL(skillimprove) = P_SKILLED;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(4) && P_MAX_SKILL(skillimprove) == P_SKILLED) {
				P_MAX_SKILL(skillimprove) = P_EXPERT;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(10) && P_MAX_SKILL(skillimprove) == P_EXPERT) {
				P_MAX_SKILL(skillimprove) = P_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(100) && P_MAX_SKILL(skillimprove) == P_MASTER) {
				P_MAX_SKILL(skillimprove) = P_GRAND_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else if (!rn2(200) && P_MAX_SKILL(skillimprove) == P_GRAND_MASTER) {
				P_MAX_SKILL(skillimprove) = P_SUPREME_MASTER;
				pline("Your knowledge of the %s skill increases.", P_NAME(skillimprove));
			} else pline("Unfortunately, you feel no different than before.");

		}

		}

		if (Role_if(PM_BINDER) && u.ulevel > u.urmaxlvlC) {

		u.urmaxlvlC = u.ulevel;

		if (!rn2(3)) {
			ubookz = mkobj(SPBOOK_CLASS, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book appeared at your feet!"); }

		}

		if (Role_if(PM_BARD) && u.ulevel > u.urmaxlvlD) {

		u.urmaxlvlD = u.ulevel;
		/* Yes I know, most of the names make no sense. They're from the bard patch. --Amy */

		if (u.urmaxlvlD == 3) {
			ubookz = mksobj(SPE_SLEEP, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book of lullaby appeared at your feet!");
		}
		if (u.urmaxlvlD == 4) {
			ubookz = mksobj(SPE_CURE_BLINDNESS, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book of cause blindness appeared at your feet!");
		}
		if (u.urmaxlvlD == 5) {
			ubookz = mksobj(SPE_CONFUSE_MONSTER, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book of cacophony appeared at your feet!");
		}
		if (u.urmaxlvlD == 6) {
			ubookz = mksobj(SPE_CURE_SICKNESS, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book of cause sickness appeared at your feet!");
		}
		if (u.urmaxlvlD == 7) {
			ubookz = mksobj(SPE_SLOW_MONSTER, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book of drowsiness appeared at your feet!");
		}
		if (u.urmaxlvlD == 8) {
			ubookz = mksobj(SPE_HASTE_SELF, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book of haste pets appeared at your feet!");
		}
		if (u.urmaxlvlD == 9) {
			ubookz = mksobj(RIN_PROTECTION_FROM_SHAPE_CHAN, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A ring of silence appeared at your feet!");
		}
		if (u.urmaxlvlD == 10) {
			ubookz = mksobj(SPE_CAUSE_FEAR, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book of despair appeared at your feet!");
		}
		if (u.urmaxlvlD == 12) {
			ubookz = mksobj(SPE_FORCE_BOLT, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book of shatter appeared at your feet!");
		}
		if (u.urmaxlvlD == 14) {
			ubookz = mksobj(CLOAK_OF_DISPLACEMENT, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A cloak of ventriloquism appeared at your feet!");
		}
		if (u.urmaxlvlD == 15) {
			ubookz = mksobj(SPE_CHARM_MONSTER, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book of friendship appeared at your feet!");
		}
		if (u.urmaxlvlD == 20) {
			ubookz = mksobj(SPE_POLYMORPH, TRUE, FALSE);
			if (ubookz) dropy(ubookz);
			pline("A book of change appeared at your feet!");
		}

		}

		if (Role_if(PM_ZYBORG) && u.ulevel > u.urmaxlvlB) {

		u.urmaxlvlB = u.ulevel;

		if (!rn2(3)) { switch (rnd(176)) {

			case 1: 
			case 2: 
			case 3: 
				if (!tech_known(T_BERSERK)) {    	learntech(T_BERSERK, FROMOUTSIDE, 1);
			    	You("learn how to perform berserk!");
				}
				break;
			case 4: 
			case 5: 
			case 6: 
				if (!tech_known(T_KIII)) {    	learntech(T_KIII, FROMOUTSIDE, 1);
			    	You("learn how to perform kiii!");
				}
				break;
			case 7: 
			case 8: 
			case 9: 
				if (!tech_known(T_RESEARCH)) {    	learntech(T_RESEARCH, FROMOUTSIDE, 1);
			    	You("learn how to perform research!");
				}
				break;
			case 10: 
			case 11: 
			case 12: 
				if (!tech_known(T_SURGERY)) {    	learntech(T_SURGERY, FROMOUTSIDE, 1);
			    	You("learn how to perform surgery!");
				}
				break;
			case 13: 
			case 14: 
			case 15: 
				if (!tech_known(T_REINFORCE)) {    	learntech(T_REINFORCE, FROMOUTSIDE, 1);
			    	You("learn how to perform reinforce memory!");
				}
				break;
			case 16: 
			case 17:
			case 18: 
				if (!tech_known(T_FLURRY)) {    	learntech(T_FLURRY, FROMOUTSIDE, 1);
			    	You("learn how to perform missile flurry!");
				}
				break;
			case 19: 
			case 20: 
			case 21: 
				if (!tech_known(T_PRACTICE)) {    	learntech(T_PRACTICE, FROMOUTSIDE, 1);
			    	You("learn how to perform weapon practice!");
				}
				break;
			case 22: 
			case 23: 
			case 24: 
				if (!tech_known(T_EVISCERATE)) {    	learntech(T_EVISCERATE, FROMOUTSIDE, 1);
			    	You("learn how to perform eviscerate!");
				}
				break;
			case 25: 
			case 26: 
			case 27: 
				if (!tech_known(T_HEAL_HANDS)) {    	learntech(T_HEAL_HANDS, FROMOUTSIDE, 1);
			    	You("learn how to perform healing hands!");
				}
				break;
			case 28: 
			case 29: 
			case 30: 
				if (!tech_known(T_CALM_STEED)) {    	learntech(T_CALM_STEED, FROMOUTSIDE, 1);
			    	You("learn how to perform calm steed!");
				}
				break;
			case 31: 
			case 32: 
			case 33: 
				if (!tech_known(T_TURN_UNDEAD)) {    	learntech(T_TURN_UNDEAD, FROMOUTSIDE, 1);
			    	You("learn how to perform turn undead!");
				}
				break;
			case 34: 
			case 35: 
			case 36: 
				if (!tech_known(T_VANISH)) {    	learntech(T_VANISH, FROMOUTSIDE, 1);
			    	You("learn how to perform vanish!");
				}
				break;
			case 37: 
			case 38: 
			case 39: 
				if (!tech_known(T_CUTTHROAT)) {    	learntech(T_CUTTHROAT, FROMOUTSIDE, 1);
			    	You("learn how to perform cutthroat!");
				}
				break;
			case 40: 
			case 41: 
				if (!tech_known(T_BLESSING)) {    	learntech(T_BLESSING, FROMOUTSIDE, 1);
			    	You("learn how to perform blessing!");
				}
				break;
			case 42: 
			case 43: 
			case 44: 
				if (!tech_known(T_E_FIST)) {    	learntech(T_E_FIST, FROMOUTSIDE, 1);
			    	You("learn how to perform elemental fist!");
				}
				break;
			case 45: 
			case 46: 
			case 47: 
				if (!tech_known(T_PRIMAL_ROAR)) {    	learntech(T_PRIMAL_ROAR, FROMOUTSIDE, 1);
			    	You("learn how to perform primal roar!");
				}
				break;
			case 48: 
			case 49: 
				if (!tech_known(T_LIQUID_LEAP)) {    	learntech(T_LIQUID_LEAP, FROMOUTSIDE, 1);
			    	You("learn how to perform liquid leap!");
				}
				break;
			case 50: 
			case 51: 
			case 52: 
				if (!tech_known(T_CRIT_STRIKE)) {    	learntech(T_CRIT_STRIKE, FROMOUTSIDE, 1);
			    	You("learn how to perform critical strike!");
				}
				break;
			case 53: 
			case 54: 
			case 55: 
				if (!tech_known(T_SIGIL_CONTROL)) {    	learntech(T_SIGIL_CONTROL, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of control!");
				}
				break;
			case 56: 
			case 57: 
			case 58: 
				if (!tech_known(T_SIGIL_TEMPEST)) {    	learntech(T_SIGIL_TEMPEST, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of tempest!");
				}
				break;
			case 59: 
			case 60: 
			case 61: 
				if (!tech_known(T_SIGIL_DISCHARGE)) {    	learntech(T_SIGIL_DISCHARGE, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of discharge!");
				}
				break;
			case 62: 
			case 63: 
			case 64: 
				if (!tech_known(T_RAISE_ZOMBIES)) {    	learntech(T_RAISE_ZOMBIES, FROMOUTSIDE, 1);
			    	You("learn how to perform raise zombies!");
				}
				break;
			case 65: 
				if (!tech_known(T_REVIVE)) {    	learntech(T_REVIVE, FROMOUTSIDE, 1);
			    	You("learn how to perform revivification!");
				}
				break;
			case 66: 
			case 67: 
			case 68: 
				if (!tech_known(T_WARD_FIRE)) {    	learntech(T_WARD_FIRE, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against fire!");
				}
				break;
			case 69: 
			case 70: 
			case 71: 
				if (!tech_known(T_WARD_COLD)) {    	learntech(T_WARD_COLD, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against cold!");
				}
				break;
			case 72: 
			case 73: 
			case 74: 
				if (!tech_known(T_WARD_ELEC)) {    	learntech(T_WARD_ELEC, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against electricity!");
				}
				break;
			case 75: 
			case 76: 
			case 77: 
				if (!tech_known(T_TINKER)) {    	learntech(T_TINKER, FROMOUTSIDE, 1);
			    	You("learn how to perform tinker!");
				}
				break;
			case 78: 
			case 79: 
			case 80: 
				if (!tech_known(T_RAGE)) {    	learntech(T_RAGE, FROMOUTSIDE, 1);
			    	You("learn how to perform rage eruption!");
				}
				break;
			case 81: 
			case 82: 
			case 83: 
				if (!tech_known(T_BLINK)) {    	learntech(T_BLINK, FROMOUTSIDE, 1);
			    	You("learn how to perform blink!");
				}
				break;
			case 84: 
			case 85: 
			case 86: 
				if (!tech_known(T_CHI_STRIKE)) {    	learntech(T_CHI_STRIKE, FROMOUTSIDE, 1);
			    	You("learn how to perform chi strike!");
				}
				break;
			case 87: 
			case 88: 
			case 89: 
				if (!tech_known(T_DRAW_ENERGY)) {    	learntech(T_DRAW_ENERGY, FROMOUTSIDE, 1);
			    	You("learn how to perform draw energy!");
				}
				break;
			case 90: 
			case 91: 
			case 92: 
				if (!tech_known(T_CHI_HEALING)) {    	learntech(T_CHI_HEALING, FROMOUTSIDE, 1);
			    	You("learn how to perform chi healing!");
				}
				break;
			case 93: 
			case 94: 
			case 95: 
				if (!tech_known(T_DAZZLE)) {    	learntech(T_DAZZLE, FROMOUTSIDE, 1);
			    	You("learn how to perform dazzle!");
				}
				break;
			case 96: 
			case 97: 
			case 98: 
				if (!tech_known(T_BLITZ)) {    	learntech(T_BLITZ, FROMOUTSIDE, 1);
			    	You("learn how to perform chained blitz!");
				}
				break;
			case 99: 
			case 100: 
			case 101: 
				if (!tech_known(T_PUMMEL)) {    	learntech(T_PUMMEL, FROMOUTSIDE, 1);
			    	You("learn how to perform pummel!");
				}
				break;
			case 102: 
			case 103: 
			case 104: 
				if (!tech_known(T_G_SLAM)) {    	learntech(T_G_SLAM, FROMOUTSIDE, 1);
			    	You("learn how to perform ground slam!");
				}
				break;
			case 105: 
			case 106: 
			case 107: 
				if (!tech_known(T_DASH)) {    	learntech(T_DASH, FROMOUTSIDE, 1);
			    	You("learn how to perform air dash!");
				}
				break;
			case 108: 
			case 109: 
			case 110: 
				if (!tech_known(T_POWER_SURGE)) {    	learntech(T_POWER_SURGE, FROMOUTSIDE, 1);
			    	You("learn how to perform power surge!");
				}
				break;
			case 111: 
			case 112: 
			case 113: 
				if (!tech_known(T_SPIRIT_BOMB)) {    	learntech(T_SPIRIT_BOMB, FROMOUTSIDE, 1);
			    	You("learn how to perform spirit bomb!");
				}
				break;
			case 114: 
			case 115: 
			case 116: 
				if (!tech_known(T_DRAW_BLOOD)) {    	learntech(T_DRAW_BLOOD, FROMOUTSIDE, 1);
			    	You("learn how to perform draw blood!");
				}
				break;
			case 117: 
				if (!tech_known(T_WORLD_FALL)) {    	learntech(T_WORLD_FALL, FROMOUTSIDE, 1);
			    	You("learn how to perform world fall!");
				}
				break;
			case 118: 
			case 119: 
			case 120: 
				if (!tech_known(T_CREATE_AMMO)) {    	learntech(T_CREATE_AMMO, FROMOUTSIDE, 1);
			    	You("learn how to perform create ammo!");
				}
				break;
			case 121: 
			case 122: 
			case 123: 
				if (!tech_known(T_POKE_BALL)) {    	learntech(T_POKE_BALL, FROMOUTSIDE, 1);
			    	You("learn how to perform poke ball!");
				}
				break;
			case 124: 
			case 125: 
			case 126: 
				if (!tech_known(T_ATTIRE_CHARM)) {    	learntech(T_ATTIRE_CHARM, FROMOUTSIDE, 1);
			    	You("learn how to perform attire charm!");
				}
				break;
			case 127: 
			case 128: 
			case 129: 
				if (!tech_known(T_SUMMON_TEAM_ANT)) {    	learntech(T_SUMMON_TEAM_ANT, FROMOUTSIDE, 1);
			    	You("learn how to perform summon team ant!");
				}
				break;
			case 130: 
			case 131: 
			case 132: 
				if (!tech_known(T_JEDI_JUMP)) {    	learntech(T_JEDI_JUMP, FROMOUTSIDE, 1);
			    	You("learn how to perform jedi jump!");
				}
				break;
			case 133: 
			case 134: 
			case 135: 
				if (!tech_known(T_CHARGE_SABER)) {    	learntech(T_CHARGE_SABER, FROMOUTSIDE, 1);
			    	You("learn how to perform charge saber!");
				}
				break;
			case 136: 
			case 137: 
			case 138: 
				if (!tech_known(T_TELEKINESIS)) {    	learntech(T_TELEKINESIS, FROMOUTSIDE, 1);
			    	You("learn how to perform telekinesis!");
				}
				break;
			case 139: 
				if (!tech_known(T_EGG_BOMB)) {    	learntech(T_EGG_BOMB, FROMOUTSIDE, 1);
			    	You("learn how to perform egg bomb!");
				}
				break;
			case 140: 
			case 141: 
			case 142: 
				if (!tech_known(T_BOOZE)) {    	learntech(T_BOOZE, FROMOUTSIDE, 1);
			    	You("learn how to perform booze!");
				}
				break;

			case 143: 
			case 144: 
			case 145: 
				if (!tech_known(T_IRON_SKIN)) {    	learntech(T_IRON_SKIN, FROMOUTSIDE, 1);
			    	You("learn how to perform iron skin!");
				}
				break;
			case 146: 
				if (!tech_known(T_POLYFORM)) {    	learntech(T_POLYFORM, FROMOUTSIDE, 1);
			    	You("learn how to perform polyform!");
				}
				break;
			case 147: 
			case 148: 
			case 149: 
				if (!tech_known(T_CONCENTRATING)) {    	learntech(T_CONCENTRATING, FROMOUTSIDE, 1);
			    	You("learn how to perform concentrating!");
				}
				break;
			case 150: 
				if (!tech_known(T_SUMMON_PET)) {    	learntech(T_SUMMON_PET, FROMOUTSIDE, 1);
			    	You("learn how to perform summon pet!");
				}
				break;
			case 151: 
			case 152: 
				if (!tech_known(T_DOUBLE_THROWNAGE)) {    	learntech(T_DOUBLE_THROWNAGE, FROMOUTSIDE, 1);
			    	You("learn how to perform double thrownage!");
				}
				break;
			case 153: 
			case 154: 
			case 155: 
				if (!tech_known(T_SHIELD_BASH)) {    	learntech(T_SHIELD_BASH, FROMOUTSIDE, 1);
			    	You("learn how to perform shield bash!");
				}
				break;
			case 156: 
			case 157: 
				if (!tech_known(T_RECHARGE)) {    	learntech(T_RECHARGE, FROMOUTSIDE, 1);
			    	You("learn how to perform recharge!");
				}
				break;
			case 158: 
			case 159: 
			case 160: 
				if (!tech_known(T_SPIRITUALITY_CHECK)) {    	learntech(T_SPIRITUALITY_CHECK, FROMOUTSIDE, 1);
			    	You("learn how to perform spirituality check!");
				}
				break;
			case 161: 
				if (!tech_known(T_EDDY_WIND)) {    	learntech(T_EDDY_WIND, FROMOUTSIDE, 1);
			    	You("learn how to perform eddy wind!");
				}
				break;
			case 162: 
				if (!tech_known(T_BLOOD_RITUAL)) {    	learntech(T_BLOOD_RITUAL, FROMOUTSIDE, 1);
			    	You("learn how to perform blood ritual!");
				}
				break;
			case 163: 
				if (!tech_known(T_ENT_S_POTION)) {    	learntech(T_ENT_S_POTION, FROMOUTSIDE, 1);
			    	You("learn how to perform ent's potion!");
				}
				break;
			case 164: 
			case 165: 
				if (!tech_known(T_LUCKY_GAMBLE)) {    	learntech(T_LUCKY_GAMBLE, FROMOUTSIDE, 1);
			    	You("learn how to perform lucky gamble!");
				}
				break;
			case 166: 
				if (!tech_known(T_DECONTAMINATE)) {    	learntech(T_DECONTAMINATE, FROMOUTSIDE, 1);
			    	You("learn how to perform decontaminate!");
				}
				break;
			case 167: 
				if (!tech_known(T_WONDERSPELL)) {    	learntech(T_WONDERSPELL, FROMOUTSIDE, 1);
			    	You("learn how to perform wonderspell!");
				}
				break;
			case 168: 
				if (!tech_known(T_RESET_TECHNIQUE)) {    	learntech(T_RESET_TECHNIQUE, FROMOUTSIDE, 1);
			    	You("learn how to perform reset technique!");
				}
				break;
			case 169: 
				if (!tech_known(T_DIAMOND_BARRIER)) {    	learntech(T_DIAMOND_BARRIER, FROMOUTSIDE, 1);
			    	You("learn how to perform diamond barrier!");
				}
				break;

			default:
				break;

		      }

		    }

		}

		if (Role_if(PM_ANACHRONOUNBINDER) && u.ulevel > u.urmaxlvlJ) {

			u.urmaxlvlJ = u.ulevel;

			int maxtrainingamount = 0;
			int skillnumber = 0;
			int actualskillselection = 0;
			int amountofpossibleskills = 1;
			int i;

			for (i = 0; i < P_NUM_SKILLS; i++) {
				if (P_SKILL(i) != P_ISRESTRICTED) continue;

				if (P_ADVANCE(i) > 0 && P_ADVANCE(i) >= maxtrainingamount) {
					if (P_ADVANCE(i) > maxtrainingamount) {
						amountofpossibleskills = 1;
						skillnumber = i;
						maxtrainingamount = P_ADVANCE(i);
					} else if (!rn2(amountofpossibleskills + 1)) {
						amountofpossibleskills++;
						skillnumber = i;
					} else {
						amountofpossibleskills++;
					}
				}
			}

			if (skillnumber > 0 && maxtrainingamount > 0) {
				unrestrict_weapon_skill(skillnumber);
				P_MAX_SKILL(skillnumber) = (maxtrainingamount >= 5000 ? P_SUPREME_MASTER : maxtrainingamount >= 500 ? P_GRAND_MASTER : maxtrainingamount >= 50 ? P_MASTER : P_EXPERT);
				pline("You can now learn the %s skill, with a new cap of %s.", P_NAME(skillnumber), maxtrainingamount >= 5000 ? "supreme master" : maxtrainingamount >= 500 ? "grand master" : maxtrainingamount >= 50 ? "master" : "expert");
			} else {
				pline("You've trained no unknown skills since your last level up and therefore you unfortunately don't learn anything new.");
			}

		}

		if (Role_if(PM_MYSTIC) && u.ulevel > u.urmaxlvlH) {

		u.urmaxlvlH = u.ulevel;

		if (!rn2(3)) { switch (rnd(176)) {

			case 1: 
			case 2: 
			case 3: 
				if (!tech_known(T_BERSERK)) {    	learntech(T_BERSERK, FROMOUTSIDE, 1);
			    	You("learn how to perform berserk!");
				}
				break;
			case 4: 
			case 5: 
			case 6: 
				if (!tech_known(T_KIII)) {    	learntech(T_KIII, FROMOUTSIDE, 1);
			    	You("learn how to perform kiii!");
				}
				break;
			case 7: 
			case 8: 
			case 9: 
				if (!tech_known(T_RESEARCH)) {    	learntech(T_RESEARCH, FROMOUTSIDE, 1);
			    	You("learn how to perform research!");
				}
				break;
			case 10: 
			case 11: 
			case 12: 
				if (!tech_known(T_SURGERY)) {    	learntech(T_SURGERY, FROMOUTSIDE, 1);
			    	You("learn how to perform surgery!");
				}
				break;
			case 13: 
			case 14: 
			case 15: 
				if (!tech_known(T_REINFORCE)) {    	learntech(T_REINFORCE, FROMOUTSIDE, 1);
			    	You("learn how to perform reinforce memory!");
				}
				break;
			case 16: 
			case 17:
			case 18: 
				if (!tech_known(T_FLURRY)) {    	learntech(T_FLURRY, FROMOUTSIDE, 1);
			    	You("learn how to perform missile flurry!");
				}
				break;
			case 19: 
			case 20: 
			case 21: 
				if (!tech_known(T_PRACTICE)) {    	learntech(T_PRACTICE, FROMOUTSIDE, 1);
			    	You("learn how to perform weapon practice!");
				}
				break;
			case 22: 
			case 23: 
			case 24: 
				if (!tech_known(T_EVISCERATE)) {    	learntech(T_EVISCERATE, FROMOUTSIDE, 1);
			    	You("learn how to perform eviscerate!");
				}
				break;
			case 25: 
			case 26: 
			case 27: 
				if (!tech_known(T_HEAL_HANDS)) {    	learntech(T_HEAL_HANDS, FROMOUTSIDE, 1);
			    	You("learn how to perform healing hands!");
				}
				break;
			case 28: 
			case 29: 
			case 30: 
				if (!tech_known(T_CALM_STEED)) {    	learntech(T_CALM_STEED, FROMOUTSIDE, 1);
			    	You("learn how to perform calm steed!");
				}
				break;
			case 31: 
			case 32: 
			case 33: 
				if (!tech_known(T_TURN_UNDEAD)) {    	learntech(T_TURN_UNDEAD, FROMOUTSIDE, 1);
			    	You("learn how to perform turn undead!");
				}
				break;
			case 34: 
			case 35: 
			case 36: 
				if (!tech_known(T_VANISH)) {    	learntech(T_VANISH, FROMOUTSIDE, 1);
			    	You("learn how to perform vanish!");
				}
				break;
			case 37: 
			case 38: 
			case 39: 
				if (!tech_known(T_CUTTHROAT)) {    	learntech(T_CUTTHROAT, FROMOUTSIDE, 1);
			    	You("learn how to perform cutthroat!");
				}
				break;
			case 40: 
			case 41: 
				if (!tech_known(T_BLESSING)) {    	learntech(T_BLESSING, FROMOUTSIDE, 1);
			    	You("learn how to perform blessing!");
				}
				break;
			case 42: 
			case 43: 
			case 44: 
				if (!tech_known(T_E_FIST)) {    	learntech(T_E_FIST, FROMOUTSIDE, 1);
			    	You("learn how to perform elemental fist!");
				}
				break;
			case 45: 
			case 46: 
			case 47: 
				if (!tech_known(T_PRIMAL_ROAR)) {    	learntech(T_PRIMAL_ROAR, FROMOUTSIDE, 1);
			    	You("learn how to perform primal roar!");
				}
				break;
			case 48: 
			case 49: 
				if (!tech_known(T_LIQUID_LEAP)) {    	learntech(T_LIQUID_LEAP, FROMOUTSIDE, 1);
			    	You("learn how to perform liquid leap!");
				}
				break;
			case 50: 
			case 51: 
			case 52: 
				if (!tech_known(T_CRIT_STRIKE)) {    	learntech(T_CRIT_STRIKE, FROMOUTSIDE, 1);
			    	You("learn how to perform critical strike!");
				}
				break;
			case 53: 
			case 54: 
			case 55: 
				if (!tech_known(T_SIGIL_CONTROL)) {    	learntech(T_SIGIL_CONTROL, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of control!");
				}
				break;
			case 56: 
			case 57: 
			case 58: 
				if (!tech_known(T_SIGIL_TEMPEST)) {    	learntech(T_SIGIL_TEMPEST, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of tempest!");
				}
				break;
			case 59: 
			case 60: 
			case 61: 
				if (!tech_known(T_SIGIL_DISCHARGE)) {    	learntech(T_SIGIL_DISCHARGE, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of discharge!");
				}
				break;
			case 62: 
			case 63: 
			case 64: 
				if (!tech_known(T_RAISE_ZOMBIES)) {    	learntech(T_RAISE_ZOMBIES, FROMOUTSIDE, 1);
			    	You("learn how to perform raise zombies!");
				}
				break;
			case 65: 
				if (!tech_known(T_REVIVE)) {    	learntech(T_REVIVE, FROMOUTSIDE, 1);
			    	You("learn how to perform revivification!");
				}
				break;
			case 66: 
			case 67: 
			case 68: 
				if (!tech_known(T_WARD_FIRE)) {    	learntech(T_WARD_FIRE, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against fire!");
				}
				break;
			case 69: 
			case 70: 
			case 71: 
				if (!tech_known(T_WARD_COLD)) {    	learntech(T_WARD_COLD, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against cold!");
				}
				break;
			case 72: 
			case 73: 
			case 74: 
				if (!tech_known(T_WARD_ELEC)) {    	learntech(T_WARD_ELEC, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against electricity!");
				}
				break;
			case 75: 
			case 76: 
			case 77: 
				if (!tech_known(T_TINKER)) {    	learntech(T_TINKER, FROMOUTSIDE, 1);
			    	You("learn how to perform tinker!");
				}
				break;
			case 78: 
			case 79: 
			case 80: 
				if (!tech_known(T_RAGE)) {    	learntech(T_RAGE, FROMOUTSIDE, 1);
			    	You("learn how to perform rage eruption!");
				}
				break;
			case 81: 
			case 82: 
			case 83: 
				if (!tech_known(T_BLINK)) {    	learntech(T_BLINK, FROMOUTSIDE, 1);
			    	You("learn how to perform blink!");
				}
				break;
			case 84: 
			case 85: 
			case 86: 
				if (!tech_known(T_CHI_STRIKE)) {    	learntech(T_CHI_STRIKE, FROMOUTSIDE, 1);
			    	You("learn how to perform chi strike!");
				}
				break;
			case 87: 
			case 88: 
			case 89: 
				if (!tech_known(T_DRAW_ENERGY)) {    	learntech(T_DRAW_ENERGY, FROMOUTSIDE, 1);
			    	You("learn how to perform draw energy!");
				}
				break;
			case 90: 
			case 91: 
			case 92: 
				if (!tech_known(T_CHI_HEALING)) {    	learntech(T_CHI_HEALING, FROMOUTSIDE, 1);
			    	You("learn how to perform chi healing!");
				}
				break;
			case 93: 
			case 94: 
			case 95: 
				if (!tech_known(T_DAZZLE)) {    	learntech(T_DAZZLE, FROMOUTSIDE, 1);
			    	You("learn how to perform dazzle!");
				}
				break;
			case 96: 
			case 97: 
			case 98: 
				if (!tech_known(T_BLITZ)) {    	learntech(T_BLITZ, FROMOUTSIDE, 1);
			    	You("learn how to perform chained blitz!");
				}
				break;
			case 99: 
			case 100: 
			case 101: 
				if (!tech_known(T_PUMMEL)) {    	learntech(T_PUMMEL, FROMOUTSIDE, 1);
			    	You("learn how to perform pummel!");
				}
				break;
			case 102: 
			case 103: 
			case 104: 
				if (!tech_known(T_G_SLAM)) {    	learntech(T_G_SLAM, FROMOUTSIDE, 1);
			    	You("learn how to perform ground slam!");
				}
				break;
			case 105: 
			case 106: 
			case 107: 
				if (!tech_known(T_DASH)) {    	learntech(T_DASH, FROMOUTSIDE, 1);
			    	You("learn how to perform air dash!");
				}
				break;
			case 108: 
			case 109: 
			case 110: 
				if (!tech_known(T_POWER_SURGE)) {    	learntech(T_POWER_SURGE, FROMOUTSIDE, 1);
			    	You("learn how to perform power surge!");
				}
				break;
			case 111: 
			case 112: 
			case 113: 
				if (!tech_known(T_SPIRIT_BOMB)) {    	learntech(T_SPIRIT_BOMB, FROMOUTSIDE, 1);
			    	You("learn how to perform spirit bomb!");
				}
				break;
			case 114: 
			case 115: 
			case 116: 
				if (!tech_known(T_DRAW_BLOOD)) {    	learntech(T_DRAW_BLOOD, FROMOUTSIDE, 1);
			    	You("learn how to perform draw blood!");
				}
				break;
			case 117: 
				if (!tech_known(T_WORLD_FALL)) {    	learntech(T_WORLD_FALL, FROMOUTSIDE, 1);
			    	You("learn how to perform world fall!");
				}
				break;
			case 118: 
			case 119: 
			case 120: 
				if (!tech_known(T_CREATE_AMMO)) {    	learntech(T_CREATE_AMMO, FROMOUTSIDE, 1);
			    	You("learn how to perform create ammo!");
				}
				break;
			case 121: 
			case 122: 
			case 123: 
				if (!tech_known(T_POKE_BALL)) {    	learntech(T_POKE_BALL, FROMOUTSIDE, 1);
			    	You("learn how to perform poke ball!");
				}
				break;
			case 124: 
			case 125: 
			case 126: 
				if (!tech_known(T_ATTIRE_CHARM)) {    	learntech(T_ATTIRE_CHARM, FROMOUTSIDE, 1);
			    	You("learn how to perform attire charm!");
				}
				break;
			case 127: 
			case 128: 
			case 129: 
				if (!tech_known(T_SUMMON_TEAM_ANT)) {    	learntech(T_SUMMON_TEAM_ANT, FROMOUTSIDE, 1);
			    	You("learn how to perform summon team ant!");
				}
				break;
			case 130: 
			case 131: 
			case 132: 
				if (!tech_known(T_JEDI_JUMP)) {    	learntech(T_JEDI_JUMP, FROMOUTSIDE, 1);
			    	You("learn how to perform jedi jump!");
				}
				break;
			case 133: 
			case 134: 
			case 135: 
				if (!tech_known(T_CHARGE_SABER)) {    	learntech(T_CHARGE_SABER, FROMOUTSIDE, 1);
			    	You("learn how to perform charge saber!");
				}
				break;
			case 136: 
			case 137: 
			case 138: 
				if (!tech_known(T_TELEKINESIS)) {    	learntech(T_TELEKINESIS, FROMOUTSIDE, 1);
			    	You("learn how to perform telekinesis!");
				}
				break;
			case 139: 
				if (!tech_known(T_EGG_BOMB)) {    	learntech(T_EGG_BOMB, FROMOUTSIDE, 1);
			    	You("learn how to perform egg bomb!");
				}
				break;
			case 140: 
			case 141: 
			case 142: 
				if (!tech_known(T_BOOZE)) {    	learntech(T_BOOZE, FROMOUTSIDE, 1);
			    	You("learn how to perform booze!");
				}
				break;

			case 143: 
			case 144: 
			case 145: 
				if (!tech_known(T_IRON_SKIN)) {    	learntech(T_IRON_SKIN, FROMOUTSIDE, 1);
			    	You("learn how to perform iron skin!");
				}
				break;
			case 146: 
				if (!tech_known(T_POLYFORM)) {    	learntech(T_POLYFORM, FROMOUTSIDE, 1);
			    	You("learn how to perform polyform!");
				}
				break;
			case 147: 
			case 148: 
			case 149: 
				if (!tech_known(T_CONCENTRATING)) {    	learntech(T_CONCENTRATING, FROMOUTSIDE, 1);
			    	You("learn how to perform concentrating!");
				}
				break;
			case 150: 
				if (!tech_known(T_SUMMON_PET)) {    	learntech(T_SUMMON_PET, FROMOUTSIDE, 1);
			    	You("learn how to perform summon pet!");
				}
				break;
			case 151: 
			case 152: 
				if (!tech_known(T_DOUBLE_THROWNAGE)) {    	learntech(T_DOUBLE_THROWNAGE, FROMOUTSIDE, 1);
			    	You("learn how to perform double thrownage!");
				}
				break;
			case 153: 
			case 154: 
			case 155: 
				if (!tech_known(T_SHIELD_BASH)) {    	learntech(T_SHIELD_BASH, FROMOUTSIDE, 1);
			    	You("learn how to perform shield bash!");
				}
				break;
			case 156: 
			case 157: 
				if (!tech_known(T_RECHARGE)) {    	learntech(T_RECHARGE, FROMOUTSIDE, 1);
			    	You("learn how to perform recharge!");
				}
				break;
			case 158: 
			case 159: 
			case 160: 
				if (!tech_known(T_SPIRITUALITY_CHECK)) {    	learntech(T_SPIRITUALITY_CHECK, FROMOUTSIDE, 1);
			    	You("learn how to perform spirituality check!");
				}
				break;
			case 161: 
				if (!tech_known(T_EDDY_WIND)) {    	learntech(T_EDDY_WIND, FROMOUTSIDE, 1);
			    	You("learn how to perform eddy wind!");
				}
				break;
			case 162: 
				if (!tech_known(T_BLOOD_RITUAL)) {    	learntech(T_BLOOD_RITUAL, FROMOUTSIDE, 1);
			    	You("learn how to perform blood ritual!");
				}
				break;
			case 163: 
				if (!tech_known(T_ENT_S_POTION)) {    	learntech(T_ENT_S_POTION, FROMOUTSIDE, 1);
			    	You("learn how to perform ent's potion!");
				}
				break;
			case 164: 
			case 165: 
				if (!tech_known(T_LUCKY_GAMBLE)) {    	learntech(T_LUCKY_GAMBLE, FROMOUTSIDE, 1);
			    	You("learn how to perform lucky gamble!");
				}
				break;
			case 166: 
				if (!tech_known(T_DECONTAMINATE)) {    	learntech(T_DECONTAMINATE, FROMOUTSIDE, 1);
			    	You("learn how to perform decontaminate!");
				}
				break;
			case 167: 
				if (!tech_known(T_WONDERSPELL)) {    	learntech(T_WONDERSPELL, FROMOUTSIDE, 1);
			    	You("learn how to perform wonderspell!");
				}
				break;
			case 168: 
				if (!tech_known(T_RESET_TECHNIQUE)) {    	learntech(T_RESET_TECHNIQUE, FROMOUTSIDE, 1);
			    	You("learn how to perform reset technique!");
				}
				break;
			case 169: 
				if (!tech_known(T_DIAMOND_BARRIER)) {    	learntech(T_DIAMOND_BARRIER, FROMOUTSIDE, 1);
			    	You("learn how to perform diamond barrier!");
				}
				break;

			default:
				break;

		      }

		    }

		}

		/* Lorskel wants wild talents to learn random techniques, and I agree that this is supposed to be the case. --Amy */
		if (Role_if(PM_WILD_TALENT) && u.ulevel > u.urmaxlvlG) {

		u.urmaxlvlG = u.ulevel;

		if (!rn2(5)) { switch (rnd(176)) {

			case 1: 
			case 2: 
			case 3: 
				if (!tech_known(T_BERSERK)) {    	learntech(T_BERSERK, FROMOUTSIDE, 1);
			    	You("learn how to perform berserk!");
				}
				break;
			case 4: 
			case 5: 
			case 6: 
				if (!tech_known(T_KIII)) {    	learntech(T_KIII, FROMOUTSIDE, 1);
			    	You("learn how to perform kiii!");
				}
				break;
			case 7: 
			case 8: 
			case 9: 
				if (!tech_known(T_RESEARCH)) {    	learntech(T_RESEARCH, FROMOUTSIDE, 1);
			    	You("learn how to perform research!");
				}
				break;
			case 10: 
			case 11: 
			case 12: 
				if (!tech_known(T_SURGERY)) {    	learntech(T_SURGERY, FROMOUTSIDE, 1);
			    	You("learn how to perform surgery!");
				}
				break;
			case 13: 
			case 14: 
			case 15: 
				if (!tech_known(T_REINFORCE)) {    	learntech(T_REINFORCE, FROMOUTSIDE, 1);
			    	You("learn how to perform reinforce memory!");
				}
				break;
			case 16: 
			case 17:
			case 18: 
				if (!tech_known(T_FLURRY)) {    	learntech(T_FLURRY, FROMOUTSIDE, 1);
			    	You("learn how to perform missile flurry!");
				}
				break;
			case 19: 
			case 20: 
			case 21: 
				if (!tech_known(T_PRACTICE)) {    	learntech(T_PRACTICE, FROMOUTSIDE, 1);
			    	You("learn how to perform weapon practice!");
				}
				break;
			case 22: 
			case 23: 
			case 24: 
				if (!tech_known(T_EVISCERATE)) {    	learntech(T_EVISCERATE, FROMOUTSIDE, 1);
			    	You("learn how to perform eviscerate!");
				}
				break;
			case 25: 
			case 26: 
			case 27: 
				if (!tech_known(T_HEAL_HANDS)) {    	learntech(T_HEAL_HANDS, FROMOUTSIDE, 1);
			    	You("learn how to perform healing hands!");
				}
				break;
			case 28: 
			case 29: 
			case 30: 
				if (!tech_known(T_CALM_STEED)) {    	learntech(T_CALM_STEED, FROMOUTSIDE, 1);
			    	You("learn how to perform calm steed!");
				}
				break;
			case 31: 
			case 32: 
			case 33: 
				if (!tech_known(T_TURN_UNDEAD)) {    	learntech(T_TURN_UNDEAD, FROMOUTSIDE, 1);
			    	You("learn how to perform turn undead!");
				}
				break;
			case 34: 
			case 35: 
			case 36: 
				if (!tech_known(T_VANISH)) {    	learntech(T_VANISH, FROMOUTSIDE, 1);
			    	You("learn how to perform vanish!");
				}
				break;
			case 37: 
			case 38: 
			case 39: 
				if (!tech_known(T_CUTTHROAT)) {    	learntech(T_CUTTHROAT, FROMOUTSIDE, 1);
			    	You("learn how to perform cutthroat!");
				}
				break;
			case 40: 
			case 41: 
				if (!tech_known(T_BLESSING)) {    	learntech(T_BLESSING, FROMOUTSIDE, 1);
			    	You("learn how to perform blessing!");
				}
				break;
			case 42: 
			case 43: 
			case 44: 
				if (!tech_known(T_E_FIST)) {    	learntech(T_E_FIST, FROMOUTSIDE, 1);
			    	You("learn how to perform elemental fist!");
				}
				break;
			case 45: 
			case 46: 
			case 47: 
				if (!tech_known(T_PRIMAL_ROAR)) {    	learntech(T_PRIMAL_ROAR, FROMOUTSIDE, 1);
			    	You("learn how to perform primal roar!");
				}
				break;
			case 48: 
			case 49: 
				if (!tech_known(T_LIQUID_LEAP)) {    	learntech(T_LIQUID_LEAP, FROMOUTSIDE, 1);
			    	You("learn how to perform liquid leap!");
				}
				break;
			case 50: 
			case 51: 
			case 52: 
				if (!tech_known(T_CRIT_STRIKE)) {    	learntech(T_CRIT_STRIKE, FROMOUTSIDE, 1);
			    	You("learn how to perform critical strike!");
				}
				break;
			case 53: 
			case 54: 
			case 55: 
				if (!tech_known(T_SIGIL_CONTROL)) {    	learntech(T_SIGIL_CONTROL, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of control!");
				}
				break;
			case 56: 
			case 57: 
			case 58: 
				if (!tech_known(T_SIGIL_TEMPEST)) {    	learntech(T_SIGIL_TEMPEST, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of tempest!");
				}
				break;
			case 59: 
			case 60: 
			case 61: 
				if (!tech_known(T_SIGIL_DISCHARGE)) {    	learntech(T_SIGIL_DISCHARGE, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of discharge!");
				}
				break;
			case 62: 
			case 63: 
			case 64: 
				if (!tech_known(T_RAISE_ZOMBIES)) {    	learntech(T_RAISE_ZOMBIES, FROMOUTSIDE, 1);
			    	You("learn how to perform raise zombies!");
				}
				break;
			case 65: 
				if (!tech_known(T_REVIVE)) {    	learntech(T_REVIVE, FROMOUTSIDE, 1);
			    	You("learn how to perform revivification!");
				}
				break;
			case 66: 
			case 67: 
			case 68: 
				if (!tech_known(T_WARD_FIRE)) {    	learntech(T_WARD_FIRE, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against fire!");
				}
				break;
			case 69: 
			case 70: 
			case 71: 
				if (!tech_known(T_WARD_COLD)) {    	learntech(T_WARD_COLD, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against cold!");
				}
				break;
			case 72: 
			case 73: 
			case 74: 
				if (!tech_known(T_WARD_ELEC)) {    	learntech(T_WARD_ELEC, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against electricity!");
				}
				break;
			case 75: 
			case 76: 
			case 77: 
				if (!tech_known(T_TINKER)) {    	learntech(T_TINKER, FROMOUTSIDE, 1);
			    	You("learn how to perform tinker!");
				}
				break;
			case 78: 
			case 79: 
			case 80: 
				if (!tech_known(T_RAGE)) {    	learntech(T_RAGE, FROMOUTSIDE, 1);
			    	You("learn how to perform rage eruption!");
				}
				break;
			case 81: 
			case 82: 
			case 83: 
				if (!tech_known(T_BLINK)) {    	learntech(T_BLINK, FROMOUTSIDE, 1);
			    	You("learn how to perform blink!");
				}
				break;
			case 84: 
			case 85: 
			case 86: 
				if (!tech_known(T_CHI_STRIKE)) {    	learntech(T_CHI_STRIKE, FROMOUTSIDE, 1);
			    	You("learn how to perform chi strike!");
				}
				break;
			case 87: 
			case 88: 
			case 89: 
				if (!tech_known(T_DRAW_ENERGY)) {    	learntech(T_DRAW_ENERGY, FROMOUTSIDE, 1);
			    	You("learn how to perform draw energy!");
				}
				break;
			case 90: 
			case 91: 
			case 92: 
				if (!tech_known(T_CHI_HEALING)) {    	learntech(T_CHI_HEALING, FROMOUTSIDE, 1);
			    	You("learn how to perform chi healing!");
				}
				break;
			case 93: 
			case 94: 
			case 95: 
				if (!tech_known(T_DAZZLE)) {    	learntech(T_DAZZLE, FROMOUTSIDE, 1);
			    	You("learn how to perform dazzle!");
				}
				break;
			case 96: 
			case 97: 
			case 98: 
				if (!tech_known(T_BLITZ)) {    	learntech(T_BLITZ, FROMOUTSIDE, 1);
			    	You("learn how to perform chained blitz!");
				}
				break;
			case 99: 
			case 100: 
			case 101: 
				if (!tech_known(T_PUMMEL)) {    	learntech(T_PUMMEL, FROMOUTSIDE, 1);
			    	You("learn how to perform pummel!");
				}
				break;
			case 102: 
			case 103: 
			case 104: 
				if (!tech_known(T_G_SLAM)) {    	learntech(T_G_SLAM, FROMOUTSIDE, 1);
			    	You("learn how to perform ground slam!");
				}
				break;
			case 105: 
			case 106: 
			case 107: 
				if (!tech_known(T_DASH)) {    	learntech(T_DASH, FROMOUTSIDE, 1);
			    	You("learn how to perform air dash!");
				}
				break;
			case 108: 
			case 109: 
			case 110: 
				if (!tech_known(T_POWER_SURGE)) {    	learntech(T_POWER_SURGE, FROMOUTSIDE, 1);
			    	You("learn how to perform power surge!");
				}
				break;
			case 111: 
			case 112: 
			case 113: 
				if (!tech_known(T_SPIRIT_BOMB)) {    	learntech(T_SPIRIT_BOMB, FROMOUTSIDE, 1);
			    	You("learn how to perform spirit bomb!");
				}
				break;
			case 114: 
			case 115: 
			case 116: 
				if (!tech_known(T_DRAW_BLOOD)) {    	learntech(T_DRAW_BLOOD, FROMOUTSIDE, 1);
			    	You("learn how to perform draw blood!");
				}
				break;
			case 117: 
				if (!tech_known(T_WORLD_FALL)) {    	learntech(T_WORLD_FALL, FROMOUTSIDE, 1);
			    	You("learn how to perform world fall!");
				}
				break;
			case 118: 
			case 119: 
			case 120: 
				if (!tech_known(T_CREATE_AMMO)) {    	learntech(T_CREATE_AMMO, FROMOUTSIDE, 1);
			    	You("learn how to perform create ammo!");
				}
				break;
			case 121: 
			case 122: 
			case 123: 
				if (!tech_known(T_POKE_BALL)) {    	learntech(T_POKE_BALL, FROMOUTSIDE, 1);
			    	You("learn how to perform poke ball!");
				}
				break;
			case 124: 
			case 125: 
			case 126: 
				if (!tech_known(T_ATTIRE_CHARM)) {    	learntech(T_ATTIRE_CHARM, FROMOUTSIDE, 1);
			    	You("learn how to perform attire charm!");
				}
				break;
			case 127: 
			case 128: 
			case 129: 
				if (!tech_known(T_SUMMON_TEAM_ANT)) {    	learntech(T_SUMMON_TEAM_ANT, FROMOUTSIDE, 1);
			    	You("learn how to perform summon team ant!");
				}
				break;
			case 130: 
			case 131: 
			case 132: 
				if (!tech_known(T_JEDI_JUMP)) {    	learntech(T_JEDI_JUMP, FROMOUTSIDE, 1);
			    	You("learn how to perform jedi jump!");
				}
				break;
			case 133: 
			case 134: 
			case 135: 
				if (!tech_known(T_CHARGE_SABER)) {    	learntech(T_CHARGE_SABER, FROMOUTSIDE, 1);
			    	You("learn how to perform charge saber!");
				}
				break;
			case 136: 
			case 137: 
			case 138: 
				if (!tech_known(T_TELEKINESIS)) {    	learntech(T_TELEKINESIS, FROMOUTSIDE, 1);
			    	You("learn how to perform telekinesis!");
				}
				break;
			case 139: 
				if (!tech_known(T_EGG_BOMB)) {    	learntech(T_EGG_BOMB, FROMOUTSIDE, 1);
			    	You("learn how to perform egg bomb!");
				}
				break;
			case 140: 
			case 141: 
			case 142: 
				if (!tech_known(T_BOOZE)) {    	learntech(T_BOOZE, FROMOUTSIDE, 1);
			    	You("learn how to perform booze!");
				}
				break;

			case 143: 
			case 144: 
			case 145: 
				if (!tech_known(T_IRON_SKIN)) {    	learntech(T_IRON_SKIN, FROMOUTSIDE, 1);
			    	You("learn how to perform iron skin!");
				}
				break;
			case 146: 
				if (!tech_known(T_POLYFORM)) {    	learntech(T_POLYFORM, FROMOUTSIDE, 1);
			    	You("learn how to perform polyform!");
				}
				break;
			case 147: 
			case 148: 
			case 149: 
				if (!tech_known(T_CONCENTRATING)) {    	learntech(T_CONCENTRATING, FROMOUTSIDE, 1);
			    	You("learn how to perform concentrating!");
				}
				break;
			case 150: 
				if (!tech_known(T_SUMMON_PET)) {    	learntech(T_SUMMON_PET, FROMOUTSIDE, 1);
			    	You("learn how to perform summon pet!");
				}
				break;
			case 151: 
			case 152: 
				if (!tech_known(T_DOUBLE_THROWNAGE)) {    	learntech(T_DOUBLE_THROWNAGE, FROMOUTSIDE, 1);
			    	You("learn how to perform double thrownage!");
				}
				break;
			case 153: 
			case 154: 
			case 155: 
				if (!tech_known(T_SHIELD_BASH)) {    	learntech(T_SHIELD_BASH, FROMOUTSIDE, 1);
			    	You("learn how to perform shield bash!");
				}
				break;
			case 156: 
			case 157: 
				if (!tech_known(T_RECHARGE)) {    	learntech(T_RECHARGE, FROMOUTSIDE, 1);
			    	You("learn how to perform recharge!");
				}
				break;
			case 158: 
			case 159: 
			case 160: 
				if (!tech_known(T_SPIRITUALITY_CHECK)) {    	learntech(T_SPIRITUALITY_CHECK, FROMOUTSIDE, 1);
			    	You("learn how to perform spirituality check!");
				}
				break;
			case 161: 
				if (!tech_known(T_EDDY_WIND)) {    	learntech(T_EDDY_WIND, FROMOUTSIDE, 1);
			    	You("learn how to perform eddy wind!");
				}
				break;
			case 162: 
				if (!tech_known(T_BLOOD_RITUAL)) {    	learntech(T_BLOOD_RITUAL, FROMOUTSIDE, 1);
			    	You("learn how to perform blood ritual!");
				}
				break;
			case 163: 
				if (!tech_known(T_ENT_S_POTION)) {    	learntech(T_ENT_S_POTION, FROMOUTSIDE, 1);
			    	You("learn how to perform ent's potion!");
				}
				break;
			case 164: 
			case 165: 
				if (!tech_known(T_LUCKY_GAMBLE)) {    	learntech(T_LUCKY_GAMBLE, FROMOUTSIDE, 1);
			    	You("learn how to perform lucky gamble!");
				}
				break;
			case 166: 
				if (!tech_known(T_DECONTAMINATE)) {    	learntech(T_DECONTAMINATE, FROMOUTSIDE, 1);
			    	You("learn how to perform decontaminate!");
				}
				break;
			case 167: 
				if (!tech_known(T_WONDERSPELL)) {    	learntech(T_WONDERSPELL, FROMOUTSIDE, 1);
			    	You("learn how to perform wonderspell!");
				}
				break;
			case 168: 
				if (!tech_known(T_RESET_TECHNIQUE)) {    	learntech(T_RESET_TECHNIQUE, FROMOUTSIDE, 1);
			    	You("learn how to perform reset technique!");
				}
				break;
			case 169: 
				if (!tech_known(T_DIAMOND_BARRIER)) {    	learntech(T_DIAMOND_BARRIER, FROMOUTSIDE, 1);
			    	You("learn how to perform diamond barrier!");
				}
				break;

			default:
				break;

		      }

		    }

		}

		if (Race_if(PM_PLAYER_SLIME) && Role_if(PM_DQ_SLIME) && (u.ulevel > u.urmaxlvlI) ) {

		u.urmaxlvlI = u.ulevel;

		u.uhpmax += rnd(10);
		u.uenmax += rnd(10);

		}

		if (isamerican && Role_if(PM_GLADIATOR) && (u.ulevel > u.urmaxlvlE) ) {

		u.urmaxlvlE = u.ulevel;

		if (!rn2(2)) { switch (rnd(176)) {

			case 1: 
			case 2: 
			case 3: 
				if (!tech_known(T_BERSERK)) {    	learntech(T_BERSERK, FROMOUTSIDE, 1);
			    	You("learn how to perform berserk!");
				}
				break;
			case 4: 
			case 5: 
			case 6: 
				if (!tech_known(T_KIII)) {    	learntech(T_KIII, FROMOUTSIDE, 1);
			    	You("learn how to perform kiii!");
				}
				break;
			case 7: 
			case 8: 
			case 9: 
				if (!tech_known(T_RESEARCH)) {    	learntech(T_RESEARCH, FROMOUTSIDE, 1);
			    	You("learn how to perform research!");
				}
				break;
			case 10: 
			case 11: 
			case 12: 
				if (!tech_known(T_SURGERY)) {    	learntech(T_SURGERY, FROMOUTSIDE, 1);
			    	You("learn how to perform surgery!");
				}
				break;
			case 13: 
			case 14: 
			case 15: 
				if (!tech_known(T_REINFORCE)) {    	learntech(T_REINFORCE, FROMOUTSIDE, 1);
			    	You("learn how to perform reinforce memory!");
				}
				break;
			case 16: 
			case 17:
			case 18: 
				if (!tech_known(T_FLURRY)) {    	learntech(T_FLURRY, FROMOUTSIDE, 1);
			    	You("learn how to perform missile flurry!");
				}
				break;
			case 19: 
			case 20: 
			case 21: 
				if (!tech_known(T_PRACTICE)) {    	learntech(T_PRACTICE, FROMOUTSIDE, 1);
			    	You("learn how to perform weapon practice!");
				}
				break;
			case 22: 
			case 23: 
			case 24: 
				if (!tech_known(T_EVISCERATE)) {    	learntech(T_EVISCERATE, FROMOUTSIDE, 1);
			    	You("learn how to perform eviscerate!");
				}
				break;
			case 25: 
			case 26: 
			case 27: 
				if (!tech_known(T_HEAL_HANDS)) {    	learntech(T_HEAL_HANDS, FROMOUTSIDE, 1);
			    	You("learn how to perform healing hands!");
				}
				break;
			case 28: 
			case 29: 
			case 30: 
				if (!tech_known(T_CALM_STEED)) {    	learntech(T_CALM_STEED, FROMOUTSIDE, 1);
			    	You("learn how to perform calm steed!");
				}
				break;
			case 31: 
			case 32: 
			case 33: 
				if (!tech_known(T_TURN_UNDEAD)) {    	learntech(T_TURN_UNDEAD, FROMOUTSIDE, 1);
			    	You("learn how to perform turn undead!");
				}
				break;
			case 34: 
			case 35: 
			case 36: 
				if (!tech_known(T_VANISH)) {    	learntech(T_VANISH, FROMOUTSIDE, 1);
			    	You("learn how to perform vanish!");
				}
				break;
			case 37: 
			case 38: 
			case 39: 
				if (!tech_known(T_CUTTHROAT)) {    	learntech(T_CUTTHROAT, FROMOUTSIDE, 1);
			    	You("learn how to perform cutthroat!");
				}
				break;
			case 40: 
			case 41: 
				if (!tech_known(T_BLESSING)) {    	learntech(T_BLESSING, FROMOUTSIDE, 1);
			    	You("learn how to perform blessing!");
				}
				break;
			case 42: 
			case 43: 
			case 44: 
				if (!tech_known(T_E_FIST)) {    	learntech(T_E_FIST, FROMOUTSIDE, 1);
			    	You("learn how to perform elemental fist!");
				}
				break;
			case 45: 
			case 46: 
			case 47: 
				if (!tech_known(T_PRIMAL_ROAR)) {    	learntech(T_PRIMAL_ROAR, FROMOUTSIDE, 1);
			    	You("learn how to perform primal roar!");
				}
				break;
			case 48: 
			case 49: 
				if (!tech_known(T_LIQUID_LEAP)) {    	learntech(T_LIQUID_LEAP, FROMOUTSIDE, 1);
			    	You("learn how to perform liquid leap!");
				}
				break;
			case 50: 
			case 51: 
			case 52: 
				if (!tech_known(T_CRIT_STRIKE)) {    	learntech(T_CRIT_STRIKE, FROMOUTSIDE, 1);
			    	You("learn how to perform critical strike!");
				}
				break;
			case 53: 
			case 54: 
			case 55: 
				if (!tech_known(T_SIGIL_CONTROL)) {    	learntech(T_SIGIL_CONTROL, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of control!");
				}
				break;
			case 56: 
			case 57: 
			case 58: 
				if (!tech_known(T_SIGIL_TEMPEST)) {    	learntech(T_SIGIL_TEMPEST, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of tempest!");
				}
				break;
			case 59: 
			case 60: 
			case 61: 
				if (!tech_known(T_SIGIL_DISCHARGE)) {    	learntech(T_SIGIL_DISCHARGE, FROMOUTSIDE, 1);
			    	You("learn how to perform sigil of discharge!");
				}
				break;
			case 62: 
			case 63: 
			case 64: 
				if (!tech_known(T_RAISE_ZOMBIES)) {    	learntech(T_RAISE_ZOMBIES, FROMOUTSIDE, 1);
			    	You("learn how to perform raise zombies!");
				}
				break;
			case 65: 
				if (!tech_known(T_REVIVE)) {    	learntech(T_REVIVE, FROMOUTSIDE, 1);
			    	You("learn how to perform revivification!");
				}
				break;
			case 66: 
			case 67: 
			case 68: 
				if (!tech_known(T_WARD_FIRE)) {    	learntech(T_WARD_FIRE, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against fire!");
				}
				break;
			case 69: 
			case 70: 
			case 71: 
				if (!tech_known(T_WARD_COLD)) {    	learntech(T_WARD_COLD, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against cold!");
				}
				break;
			case 72: 
			case 73: 
			case 74: 
				if (!tech_known(T_WARD_ELEC)) {    	learntech(T_WARD_ELEC, FROMOUTSIDE, 1);
			    	You("learn how to perform ward against electricity!");
				}
				break;
			case 75: 
			case 76: 
			case 77: 
				if (!tech_known(T_TINKER)) {    	learntech(T_TINKER, FROMOUTSIDE, 1);
			    	You("learn how to perform tinker!");
				}
				break;
			case 78: 
			case 79: 
			case 80: 
				if (!tech_known(T_RAGE)) {    	learntech(T_RAGE, FROMOUTSIDE, 1);
			    	You("learn how to perform rage eruption!");
				}
				break;
			case 81: 
			case 82: 
			case 83: 
				if (!tech_known(T_BLINK)) {    	learntech(T_BLINK, FROMOUTSIDE, 1);
			    	You("learn how to perform blink!");
				}
				break;
			case 84: 
			case 85: 
			case 86: 
				if (!tech_known(T_CHI_STRIKE)) {    	learntech(T_CHI_STRIKE, FROMOUTSIDE, 1);
			    	You("learn how to perform chi strike!");
				}
				break;
			case 87: 
			case 88: 
			case 89: 
				if (!tech_known(T_DRAW_ENERGY)) {    	learntech(T_DRAW_ENERGY, FROMOUTSIDE, 1);
			    	You("learn how to perform draw energy!");
				}
				break;
			case 90: 
			case 91: 
			case 92: 
				if (!tech_known(T_CHI_HEALING)) {    	learntech(T_CHI_HEALING, FROMOUTSIDE, 1);
			    	You("learn how to perform chi healing!");
				}
				break;
			case 93: 
			case 94: 
			case 95: 
				if (!tech_known(T_DAZZLE)) {    	learntech(T_DAZZLE, FROMOUTSIDE, 1);
			    	You("learn how to perform dazzle!");
				}
				break;
			case 96: 
			case 97: 
			case 98: 
				if (!tech_known(T_BLITZ)) {    	learntech(T_BLITZ, FROMOUTSIDE, 1);
			    	You("learn how to perform chained blitz!");
				}
				break;
			case 99: 
			case 100: 
			case 101: 
				if (!tech_known(T_PUMMEL)) {    	learntech(T_PUMMEL, FROMOUTSIDE, 1);
			    	You("learn how to perform pummel!");
				}
				break;
			case 102: 
			case 103: 
			case 104: 
				if (!tech_known(T_G_SLAM)) {    	learntech(T_G_SLAM, FROMOUTSIDE, 1);
			    	You("learn how to perform ground slam!");
				}
				break;
			case 105: 
			case 106: 
			case 107: 
				if (!tech_known(T_DASH)) {    	learntech(T_DASH, FROMOUTSIDE, 1);
			    	You("learn how to perform air dash!");
				}
				break;
			case 108: 
			case 109: 
			case 110: 
				if (!tech_known(T_POWER_SURGE)) {    	learntech(T_POWER_SURGE, FROMOUTSIDE, 1);
			    	You("learn how to perform power surge!");
				}
				break;
			case 111: 
			case 112: 
			case 113: 
				if (!tech_known(T_SPIRIT_BOMB)) {    	learntech(T_SPIRIT_BOMB, FROMOUTSIDE, 1);
			    	You("learn how to perform spirit bomb!");
				}
				break;
			case 114: 
			case 115: 
			case 116: 
				if (!tech_known(T_DRAW_BLOOD)) {    	learntech(T_DRAW_BLOOD, FROMOUTSIDE, 1);
			    	You("learn how to perform draw blood!");
				}
				break;
			case 117: 
				if (!tech_known(T_WORLD_FALL)) {    	learntech(T_WORLD_FALL, FROMOUTSIDE, 1);
			    	You("learn how to perform world fall!");
				}
				break;
			case 118: 
			case 119: 
			case 120: 
				if (!tech_known(T_CREATE_AMMO)) {    	learntech(T_CREATE_AMMO, FROMOUTSIDE, 1);
			    	You("learn how to perform create ammo!");
				}
				break;
			case 121: 
			case 122: 
			case 123: 
				if (!tech_known(T_POKE_BALL)) {    	learntech(T_POKE_BALL, FROMOUTSIDE, 1);
			    	You("learn how to perform poke ball!");
				}
				break;
			case 124: 
			case 125: 
			case 126: 
				if (!tech_known(T_ATTIRE_CHARM)) {    	learntech(T_ATTIRE_CHARM, FROMOUTSIDE, 1);
			    	You("learn how to perform attire charm!");
				}
				break;
			case 127: 
			case 128: 
			case 129: 
				if (!tech_known(T_SUMMON_TEAM_ANT)) {    	learntech(T_SUMMON_TEAM_ANT, FROMOUTSIDE, 1);
			    	You("learn how to perform summon team ant!");
				}
				break;
			case 130: 
			case 131: 
			case 132: 
				if (!tech_known(T_JEDI_JUMP)) {    	learntech(T_JEDI_JUMP, FROMOUTSIDE, 1);
			    	You("learn how to perform jedi jump!");
				}
				break;
			case 133: 
			case 134: 
			case 135: 
				if (!tech_known(T_CHARGE_SABER)) {    	learntech(T_CHARGE_SABER, FROMOUTSIDE, 1);
			    	You("learn how to perform charge saber!");
				}
				break;
			case 136: 
			case 137: 
			case 138: 
				if (!tech_known(T_TELEKINESIS)) {    	learntech(T_TELEKINESIS, FROMOUTSIDE, 1);
			    	You("learn how to perform telekinesis!");
				}
				break;
			case 139: 
				if (!tech_known(T_EGG_BOMB)) {    	learntech(T_EGG_BOMB, FROMOUTSIDE, 1);
			    	You("learn how to perform egg bomb!");
				}
				break;
			case 140: 
			case 141: 
			case 142: 
				if (!tech_known(T_BOOZE)) {    	learntech(T_BOOZE, FROMOUTSIDE, 1);
			    	You("learn how to perform booze!");
				}
				break;

			case 143: 
			case 144: 
			case 145: 
				if (!tech_known(T_IRON_SKIN)) {    	learntech(T_IRON_SKIN, FROMOUTSIDE, 1);
			    	You("learn how to perform iron skin!");
				}
				break;
			case 146: 
				if (!tech_known(T_POLYFORM)) {    	learntech(T_POLYFORM, FROMOUTSIDE, 1);
			    	You("learn how to perform polyform!");
				}
				break;
			case 147: 
			case 148: 
			case 149: 
				if (!tech_known(T_CONCENTRATING)) {    	learntech(T_CONCENTRATING, FROMOUTSIDE, 1);
			    	You("learn how to perform concentrating!");
				}
				break;
			case 150: 
				if (!tech_known(T_SUMMON_PET)) {    	learntech(T_SUMMON_PET, FROMOUTSIDE, 1);
			    	You("learn how to perform summon pet!");
				}
				break;
			case 151: 
			case 152: 
				if (!tech_known(T_DOUBLE_THROWNAGE)) {    	learntech(T_DOUBLE_THROWNAGE, FROMOUTSIDE, 1);
			    	You("learn how to perform double thrownage!");
				}
				break;
			case 153: 
			case 154: 
			case 155: 
				if (!tech_known(T_SHIELD_BASH)) {    	learntech(T_SHIELD_BASH, FROMOUTSIDE, 1);
			    	You("learn how to perform shield bash!");
				}
				break;
			case 156: 
			case 157: 
				if (!tech_known(T_RECHARGE)) {    	learntech(T_RECHARGE, FROMOUTSIDE, 1);
			    	You("learn how to perform recharge!");
				}
				break;
			case 158: 
			case 159: 
			case 160: 
				if (!tech_known(T_SPIRITUALITY_CHECK)) {    	learntech(T_SPIRITUALITY_CHECK, FROMOUTSIDE, 1);
			    	You("learn how to perform spirituality check!");
				}
				break;
			case 161: 
				if (!tech_known(T_EDDY_WIND)) {    	learntech(T_EDDY_WIND, FROMOUTSIDE, 1);
			    	You("learn how to perform eddy wind!");
				}
				break;
			case 162: 
				if (!tech_known(T_BLOOD_RITUAL)) {    	learntech(T_BLOOD_RITUAL, FROMOUTSIDE, 1);
			    	You("learn how to perform blood ritual!");
				}
				break;
			case 163: 
				if (!tech_known(T_ENT_S_POTION)) {    	learntech(T_ENT_S_POTION, FROMOUTSIDE, 1);
			    	You("learn how to perform ent's potion!");
				}
				break;
			case 164: 
			case 165: 
				if (!tech_known(T_LUCKY_GAMBLE)) {    	learntech(T_LUCKY_GAMBLE, FROMOUTSIDE, 1);
			    	You("learn how to perform lucky gamble!");
				}
				break;
			case 166: 
				if (!tech_known(T_DECONTAMINATE)) {    	learntech(T_DECONTAMINATE, FROMOUTSIDE, 1);
			    	You("learn how to perform decontaminate!");
				}
				break;
			case 167: 
				if (!tech_known(T_WONDERSPELL)) {    	learntech(T_WONDERSPELL, FROMOUTSIDE, 1);
			    	You("learn how to perform wonderspell!");
				}
				break;
			case 168: 
				if (!tech_known(T_RESET_TECHNIQUE)) {    	learntech(T_RESET_TECHNIQUE, FROMOUTSIDE, 1);
			    	You("learn how to perform reset technique!");
				}
				break;
			case 169: 
				if (!tech_known(T_DIAMOND_BARRIER)) {    	learntech(T_DIAMOND_BARRIER, FROMOUTSIDE, 1);
			    	You("learn how to perform diamond barrier!");
				}
				break;

			default:
				break;


			}

		  }

		}


	flags.botl = 1;
}

/* compute a random amount of experience points suitable for the hero's
   experience level:  base number of points needed to reach the current
   level plus a random portion of what it takes to get to the next level */
long
rndexp(gaining)
boolean gaining;	/* gaining XP via potion vs setting XP for polyself */
{
	long minexp, maxexp, diff, factor, result;

	minexp = (u.ulevel == 1) ? 0L : newuexp(u.ulevel - 1);
	maxexp = newuexp(u.ulevel);
	diff = maxexp - minexp,  factor = 1L;
	/* make sure that `diff' is an argument which rn2() can handle */
	while (diff >= (long)LARGEST_INT)
	    diff /= 2L,  factor *= 2L;
	result = minexp + factor * (long)rn2((int)diff);
	/* 3.4.1:  if already at level 30, add to current experience
	   points rather than to threshold needed to reach the current
	   level; otherwise blessed potions of gain level can result
	   in lowering the experience points instead of raising them */
	if (u.ulevel == MAXULEV && gaining) {
	    result += (u.uexp - minexp);
	    /* avoid wrapping (over 400 blessed potions needed for that...) */
	    if (result < u.uexp) result = u.uexp;
	}
	return result;
}

/*exper.c*/
