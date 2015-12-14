/*	SCCS Id: @(#)obj.h	3.4	2002/01/07	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef OBJ_H
#define OBJ_H

/* #define obj obj_nh */ /* uncomment for SCO UNIX, which has a conflicting
			  * typedef for "obj" in <sys/types.h> */

union vptrs {
	    struct obj *v_nexthere;	/* floor location lists */
	    struct obj *v_ocontainer;	/* point back to container */
	    struct monst *v_ocarry;	/* point back to carrying monst */
};

struct obj {
	struct obj *nobj;
	union vptrs v;
#define nexthere	v.v_nexthere
#define ocontainer	v.v_ocontainer
#define ocarry		v.v_ocarry

	struct obj *cobj;	/* contents list for containers */
	unsigned o_id;
	xchar ox,oy;
	/*short*/long otyp;		/* object class number */
#ifdef UNPOLYPILE
	/*short*/long oldtyp;	/* WAC for unpolymorph */
#endif
	unsigned owt;
	long quan;		/* number of items */

	schar spe;		/* quality of weapon, armor or ring (+ or -)
				   number of charges for wand ( >= -1 )
				   marks your eggs, spinach tins
				   royal coffers for a court ( == 2)
				   tells which fruit a fruit is
				   special for uball and amulet
				   historic and gender for statues */
#define STATUE_HISTORIC 0x01
#define STATUE_MALE     0x02
#define STATUE_FEMALE   0x04
	char	oclass;		/* object class */
	char	invlet;		/* designation in inventory */
	/*char*/int	oartifact;	/* artifact array index */
	schar 	altmode; 	/* alternate modes - eg. SMG, double Lightsaber */
				/* WP_MODEs are in decreasing speed */
#define WP_MODE_AUTO	0	/* Max firing speed */
#define WP_MODE_BURST	1	/* 1/3 of max rate */
#define WP_MODE_SINGLE 	2	/* Single shot */

	xchar where;		/* where the object thinks it is */
#define OBJ_FREE	0		/* object not attached to anything */
#define OBJ_FLOOR	1		/* object on floor */
#define OBJ_CONTAINED	2		/* object in a container */
#define OBJ_INVENT	3		/* object in the hero's inventory */
#define OBJ_MINVENT	4		/* object in a monster inventory */
#define OBJ_MIGRATING	5		/* object sent off to another level */
#define OBJ_BURIED	6		/* object buried */
#define OBJ_ONBILL	7		/* object on shk bill */
#define NOBJ_STATES	8
	xchar timed;		/* # of fuses (timers) attached to this obj */

	Bitfield(cursed,1);
	Bitfield(blessed,1);
	Bitfield(hvycurse,1);	/* harder to uncurse than "regular" cursed items --Amy */
	Bitfield(prmcurse,1);
	Bitfield(unpaid,1);	/* on some bill */
	Bitfield(no_charge,1);	/* if shk shouldn't charge for this */
	Bitfield(known,1);	/* exact nature known */
	Bitfield(dknown,1);	/* color or text known */
	Bitfield(bknown,1);	/* blessing or curse known */
	Bitfield(rknown,1);	/* rustproof or not known */

	Bitfield(oeroded,2);	/* rusted/burnt weapon/armor */
	Bitfield(oeroded2,2);	/* corroded/rotted weapon/armor */
#define greatest_erosion(otmp) (int)((otmp)->oeroded > (otmp)->oeroded2 ? (otmp)->oeroded : (otmp)->oeroded2)
#define MAX_ERODE 3
#define orotten oeroded		/* rotten food */
#define odiluted oeroded	/* diluted potions */
#define norevive oeroded2
	Bitfield(oerodeproof,1); /* erodeproof weapon/armor */
	Bitfield(olocked,1);	/* object is locked */
#define oarmed olocked
#define odrained olocked	/* drained corpse */
	Bitfield(obroken,1);	/* lock has been broken */
	Bitfield(otrapped,1);	/* container is trapped */
				/* or accidental tripped rolling boulder trap */
#define opoisoned otrapped	/* object (weapon) is coated with poison */

	Bitfield(recharged,3);	/* number of times it's been recharged */
	Bitfield(lamplit,1);	/* a light-source -- can be lit */
#ifdef INVISIBLE_OBJECTS
	Bitfield(oinvis,1);	/* invisible */
	Bitfield(oinvisreal,1);	/* really invisible - won't be visible even with see invis --Amy */
#endif
	Bitfield(greased,1);	/* covered with grease */
	Bitfield(oattached,2);	/* obj struct has special attachment */
#define OATTACHED_NOTHING 0
#define OATTACHED_MONST   1	/* monst struct in oextra */
#define OATTACHED_M_ID    2	/* monst id in oextra */
#define OATTACHED_UNUSED3 3
	Bitfield(in_use,1);	/* for magic items before useup items */
	Bitfield(bypass,1);	/* mark this as an object to be skipped by bhito() */

	Bitfield(yours,1);	/* obj is yours (eg. thrown by you) */
	Bitfield(was_thrown,1); /* thrown by the hero since last picked up */
	Bitfield(mstartinvent,1); /* item is part of a monster's starting inventory --Amy */
	/* ? free bits */

	int	corpsenm;	/* type of corpse is mons[corpsenm] */
#define leashmon  corpsenm	/* gets m_id of attached pet */
#define spestudied corpsenm	/* # of times a spellbook has been studied */
#define fromsink  corpsenm	/* a potion from a sink */

#ifdef RECORD_ACHIEVE
#define record_achieve_special corpsenm
#endif
	uchar enchantment;	/* in case of armors, special property --Amy */

	unsigned oeaten;	/* nutrition left in food, if partly eaten */
	long age;		/* creation date */

	uchar onamelth;		/* length of name (following oxlth) */
	Bitfield(selfmade,1);	/* made with chemistry set or magic marker */
	/*short*/long oxlth;		/* length of following data */
	/* in order to prevent alignment problems oextra should
	   be (or follow) a long int */
	long owornmask;
	long oextra[1];		/* used for name of ordinary objects - length
				   is flexible; amount for tmp gold objects */
};

#define newobj(xl)	(struct obj *)alloc((unsigned)(xl) + sizeof(struct obj))
#define ONAME(otmp)	(((char *)(otmp)->oextra) + (otmp)->oxlth)

/* All objects */
#ifdef UNPOLYPILE
#define is_hazy(otmp)	((otmp)->oldtyp != STRANGE_OBJECT)
#endif
/* [ALI] None of the objects listed here can be picked up by normal monsters.
 * If any such objects need to be marked as indestructible then consideration
 * will need to be given to what happens when such a monster disappears
 * carrying the object.
 */
#define evades_destruction(otmp) ( \
			(otmp)->otyp == AMULET_OF_YENDOR || \
			(otmp)->otyp == CANDELABRUM_OF_INVOCATION || \
			(otmp)->otyp == BELL_OF_OPENING || \
			(otmp)->otyp == SPE_BOOK_OF_THE_DEAD || \
			(otmp)->oartifact == ART_KEY_OF_LAW || \
			(otmp)->oartifact == ART_KEY_OF_NEUTRALITY || \
			(otmp)->oartifact == ART_KEY_OF_CHAOS)
#ifdef INVISIBLE_OBJECTS
#define always_visible(otmp) ( \
			(otmp)->otyp == MUMMY_WRAPPING || \
			(otmp)->oclass == COIN_CLASS)
#endif

/* Weapons and weapon-tools */
/* KMH -- now based on skill categories.  Formerly:
 *	#define is_sword(otmp)	(otmp->oclass == WEAPON_CLASS && \
 *			 objects[otmp->otyp].oc_wepcat == WEP_SWORD)
 *	#define is_blade(otmp)	(otmp->oclass == WEAPON_CLASS && \
 *			 (objects[otmp->otyp].oc_wepcat == WEP_BLADE || \
 *			  objects[otmp->otyp].oc_wepcat == WEP_SWORD))
 *	#define is_weptool(o)	((o)->oclass == TOOL_CLASS && \
 *			 objects[(o)->otyp].oc_weptool)
 *	#define is_multigen(otyp) (otyp <= SHURIKEN)
 *	#define is_poisonable(otyp) (otyp <= BEC_DE_CORBIN)
 */

#define is_sword(otmp)	((otmp)->oclass == WEAPON_CLASS && \
			 objects[(otmp)->otyp].oc_skill >= P_SHORT_SWORD && \
			 objects[(otmp)->otyp].oc_skill <= P_SABER)
#define is_blade(otmp)	((otmp)->oclass == WEAPON_CLASS && \
			 objects[(otmp)->otyp].oc_skill >= P_DAGGER && \
			 objects[(otmp)->otyp].oc_skill <= P_SABER)
#define is_pole(otmp)	(((otmp)->oclass == WEAPON_CLASS || \
			(otmp)->oclass == TOOL_CLASS) && \
			 (objects[(otmp)->otyp].oc_skill == P_POLEARMS || \
			 objects[(otmp)->otyp].oc_skill == P_LANCE))
#define is_spear(otmp)	((otmp)->oclass == WEAPON_CLASS && \
			 objects[(otmp)->otyp].oc_skill >= P_SPEAR && \
			 objects[(otmp)->otyp].oc_skill <= P_JAVELIN)
#define is_axe(otmp)	((otmp)->oclass == WEAPON_CLASS && \
			 objects[(otmp)->otyp].oc_skill == P_AXE)
#define is_launcher(otmp)	((otmp)->oclass == WEAPON_CLASS && \
			 objects[(otmp)->otyp].oc_skill >= P_BOW && \
			 objects[(otmp)->otyp].oc_skill <= P_CROSSBOW)
#define is_ammo(otmp)	(((otmp)->oclass == WEAPON_CLASS || \
			 (otmp)->oclass == GEM_CLASS) && \
			 objects[(otmp)->otyp].oc_skill >= -P_CROSSBOW && \
			 objects[(otmp)->otyp].oc_skill <= -P_BOW)
#define is_missile(otmp)	(((otmp)->oclass == WEAPON_CLASS || \
			 (otmp)->oclass == TOOL_CLASS) && \
			 objects[(otmp)->otyp].oc_skill >= -P_BOOMERANG && \
			 objects[(otmp)->otyp].oc_skill <= -P_DART)
#define is_grenade(otmp)	(is_ammo(otmp) && \
			 	 objects[(otmp)->otyp].w_ammotyp == WP_GRENADE)
#define is_multigen(otmp)	((otmp)->oclass == WEAPON_CLASS && \
			 objects[(otmp)->otyp].oc_skill >= -P_SHURIKEN && \
			 objects[(otmp)->otyp].oc_skill <= -P_BOW)
#ifdef FIREARMS
#define is_unpoisonable_firearm_ammo(otmp)	\
			 (is_bullet(otmp) || (otmp)->otyp == STICK_OF_DYNAMITE)
#else
#define is_unpoisonable_firearm_ammo(otmp)	0
#endif
#define is_poisonable(otmp)	((otmp)->oclass == WEAPON_CLASS && \
			 (objects[(otmp)->otyp].oc_skill <= P_SABER || \
			 (objects[(otmp)->otyp].oc_skill >= P_POLEARMS && \
			 objects[(otmp)->otyp].oc_skill <= P_LANCE)) && \
			 !is_unpoisonable_firearm_ammo(otmp))
#define uslinging()	(uwep && objects[uwep->otyp].oc_skill == P_SLING)
#define is_weptool(o)	((o)->oclass == TOOL_CLASS && \
			 objects[(o)->otyp].oc_skill != P_NONE)
#define is_pick(otmp)	(((otmp)->oclass == WEAPON_CLASS || \
			 (otmp)->oclass == TOOL_CLASS) && \
			 objects[(otmp)->otyp].oc_skill == P_PICK_AXE)
#define is_antibar(otmp)	((otmp)->otyp == ELECTRIC_SWORD || (otmp)->otyp == GOLDEN_SABER  || (otmp)->otyp == METAL_CLUB  || (otmp)->otyp == DEVIL_STAR  || (otmp)->otyp == OBSID  || (otmp)->otyp == MALLET  || (otmp)->otyp == WEDGED_LITTLE_GIRL_SANDAL  || (otmp)->otyp == HUGGING_BOOT || (otmp)->otyp == BLOCK_HEELED_COMBAT_BOOT  || (otmp)->otyp == HIGH_HEELED_SANDAL  || (otmp)->otyp == SEXY_LEATHER_PUMP  || (otmp)->otyp == TORPEDO  || (otmp)->otyp == STEEL_WHIP  )

#define ammo_and_launcher(otmp,ltmp) \
			(is_ammo(otmp) && (ltmp) && \
			objects[(otmp)->otyp].oc_skill == -objects[(ltmp)->otyp].oc_skill && \
			  objects[(otmp)->otyp].w_ammotyp == objects[(ltmp)->otyp].w_ammotyp)
#define bimanual(otmp)	(((otmp)->oclass == WEAPON_CLASS || \
			  (otmp)->oclass == TOOL_CLASS) && \
			 objects[(otmp)->otyp].oc_bimanual)

#ifdef LIGHTSABERS
#define is_lightsaber(otmp) (objects[(otmp)->otyp].oc_skill == P_LIGHTSABER || (otmp)->otyp == LASER_SWATTER)
#endif

#ifdef FIREARMS
#define is_firearm(otmp) \
			((otmp)->oclass == WEAPON_CLASS && \
			 objects[(otmp)->otyp].oc_skill == P_FIREARM)
#define is_bullet(otmp)	((otmp)->oclass == WEAPON_CLASS && \
			 objects[(otmp)->otyp].oc_skill == -P_FIREARM)
#endif

/* Armor */
#define is_shield(otmp) ((otmp)->oclass == ARMOR_CLASS && \
			 objects[(otmp)->otyp].oc_armcat == ARM_SHIELD)
#define is_helmet(otmp) ((otmp)->oclass == ARMOR_CLASS && \
			 objects[(otmp)->otyp].oc_armcat == ARM_HELM)
#define is_boots(otmp)	((otmp)->oclass == ARMOR_CLASS && \
			 objects[(otmp)->otyp].oc_armcat == ARM_BOOTS)
#define is_gloves(otmp) ((otmp)->oclass == ARMOR_CLASS && \
			 objects[(otmp)->otyp].oc_armcat == ARM_GLOVES)
#define is_cloak(otmp)	((otmp)->oclass == ARMOR_CLASS && \
			 objects[(otmp)->otyp].oc_armcat == ARM_CLOAK)
#define is_shirt(otmp)	((otmp)->oclass == ARMOR_CLASS && \
			 objects[(otmp)->otyp].oc_armcat == ARM_SHIRT)
#define is_suit(otmp)	((otmp)->oclass == ARMOR_CLASS && \
			 objects[(otmp)->otyp].oc_armcat == ARM_SUIT)
#define is_droven_armor(otmp)	((otmp)->otyp == DROVEN_HELM\
				|| (otmp)->otyp == DROVEN_PLATE_MAIL\
				|| (otmp)->otyp == DROVEN_CHAIN_MAIL\
				|| (otmp)->otyp == DROVEN_CLOAK)
#define is_elven_armor(otmp)	((otmp)->otyp == ELVEN_LEATHER_HELM\
				|| (otmp)->otyp == ELVEN_MITHRIL_COAT\
				|| (otmp)->otyp == ELVEN_CLOAK\
				|| (otmp)->otyp == ELVEN_TOGA\
				|| (otmp)->otyp == ELVEN_SHIELD\
				|| (otmp)->otyp == ELVEN_BOOTS)
#define is_orcish_armor(otmp)	((otmp)->otyp == ORCISH_HELM\
				|| (otmp)->otyp == ORCISH_CHAIN_MAIL\
				|| (otmp)->otyp == ORCISH_RING_MAIL\
				|| (otmp)->otyp == ORCISH_CLOAK\
				|| (otmp)->otyp == URUK_HAI_SHIELD\
				|| (otmp)->otyp == ORCISH_GUARD_SHIELD\
				|| (otmp)->otyp == ORCISH_SHIELD)
#define is_dwarvish_armor(otmp)	((otmp)->otyp == DWARVISH_IRON_HELM\
				|| (otmp)->otyp == DWARVISH_MITHRIL_COAT\
				|| (otmp)->otyp == DWARVISH_CLOAK\
				|| (otmp)->otyp == DWARVISH_ROUNDSHIELD)
#define is_gnomish_armor(otmp)	(FALSE)

#define is_musable(otmp)	((otmp)->otyp == SCR_TELEPORTATION\
|| (otmp)->otyp == POT_HEALING\
|| (otmp)->otyp == POT_CURE_WOUNDS\
|| (otmp)->otyp == POT_CURE_SERIOUS_WOUNDS\
|| (otmp)->otyp == POT_CURE_CRITICAL_WOUNDS\
|| (otmp)->otyp == POT_EXTRA_HEALING\
|| (otmp)->otyp == WAN_DIGGING\
|| (otmp)->otyp == WAN_CREATE_MONSTER\
|| (otmp)->otyp == SCR_CREATE_MONSTER\
|| (otmp)->otyp == SCR_CREATE_VICTIM\
|| (otmp)->otyp == WAN_TELEPORTATION\
|| (otmp)->otyp == POT_FULL_HEALING\
|| (otmp)->otyp == WAN_HEALING\
|| (otmp)->otyp == WAN_EXTRA_HEALING\
|| (otmp)->otyp == WAN_CREATE_HORDE\
|| (otmp)->otyp == POT_VAMPIRE_BLOOD\
|| (otmp)->otyp == WAN_FULL_HEALING\
|| (otmp)->otyp == SCR_TELE_LEVEL\
|| (otmp)->otyp == WAN_TELE_LEVEL\
|| (otmp)->otyp == SCR_ROOT_PASSWORD_DETECTION\
|| (otmp)->otyp == RIN_TIMELY_BACKUP\
|| (otmp)->otyp == SCR_SUMMON_UNDEAD\
|| (otmp)->otyp == WAN_SUMMON_UNDEAD\
|| (otmp)->otyp == SCR_HEALING\
|| (otmp)->otyp == SCR_POWER_HEALING\
|| (otmp)->otyp == SCR_WARPING\
|| (otmp)->otyp == WAN_DEATH\
|| (otmp)->otyp == WAN_SLEEP\
|| (otmp)->otyp == WAN_FIREBALL\
|| (otmp)->otyp == WAN_FIRE\
|| (otmp)->otyp == WAN_COLD\
|| (otmp)->otyp == WAN_LIGHTNING\
|| (otmp)->otyp == WAN_MAGIC_MISSILE\
|| (otmp)->otyp == WAN_STRIKING\
|| (otmp)->otyp == SCR_FIRE\
|| (otmp)->otyp == POT_PARALYSIS\
|| (otmp)->otyp == POT_BLINDNESS\
|| (otmp)->otyp == POT_CONFUSION\
|| (otmp)->otyp == POT_SLEEPING\
|| (otmp)->otyp == POT_ACID\
|| (otmp)->otyp == FROST_HORN\
|| (otmp)->otyp == FIRE_HORN\
|| (otmp)->otyp == TEMPEST_HORN\
|| (otmp)->otyp == WAN_DRAINING\
|| (otmp)->otyp == WAN_INCREASE_MAX_HITPOINTS\
|| (otmp)->otyp == WAN_REDUCE_MAX_HITPOINTS\
|| (otmp)->otyp == SCR_EARTH\
|| (otmp)->otyp == POT_AMNESIA\
|| (otmp)->otyp == WAN_CANCELLATION\
|| (otmp)->otyp == POT_CYANIDE\
|| (otmp)->otyp == POT_RADIUM\
|| (otmp)->otyp == WAN_ACID\
|| (otmp)->otyp == SCR_TRAP_CREATION\
|| (otmp)->otyp == SCR_CREATE_TRAP\
|| (otmp)->otyp == WAN_TRAP_CREATION\
|| (otmp)->otyp == SCR_FLOOD\
|| (otmp)->otyp == SCR_LAVA\
|| (otmp)->otyp == SCR_GROWTH\
|| (otmp)->otyp == SCR_ICE\
|| (otmp)->otyp == SCR_CLOUDS\
|| (otmp)->otyp == SCR_BARRHING\
|| (otmp)->otyp == WAN_SOLAR_BEAM\
|| (otmp)->otyp == WAN_POISON\
|| (otmp)->otyp == SCR_LOCKOUT\
|| (otmp)->otyp == WAN_BANISHMENT\
|| (otmp)->otyp == POT_HALLUCINATION\
|| (otmp)->otyp == POT_ICE\
|| (otmp)->otyp == POT_STUNNING\
|| (otmp)->otyp == POT_NUMBNESS\
|| (otmp)->otyp == POT_URINE\
|| (otmp)->otyp == POT_CANCELLATION\
|| (otmp)->otyp == POT_SLIME\
|| (otmp)->otyp == SCR_BAD_EFFECT\
|| (otmp)->otyp == WAN_BAD_EFFECT\
|| (otmp)->otyp == POT_FIRE\
|| (otmp)->otyp == WAN_SLOW_MONSTER\
|| (otmp)->otyp == WAN_FEAR\
|| (otmp)->otyp == POT_FEAR\
|| (otmp)->otyp == POT_GAIN_LEVEL\
|| (otmp)->otyp == WAN_GAIN_LEVEL\
|| (otmp)->otyp == WAN_MAKE_INVISIBLE\
|| (otmp)->otyp == POT_INVISIBILITY\
|| (otmp)->otyp == WAN_POLYMORPH\
|| (otmp)->otyp == WAN_MUTATION\
|| (otmp)->otyp == POT_SPEED\
|| (otmp)->otyp == WAN_SPEED_MONSTER\
|| (otmp)->otyp == POT_POLYMORPH\
|| (otmp)->otyp == POT_MUTATION\
|| (otmp)->otyp == WAN_CLONE_MONSTER\
|| (otmp)->otyp == SCR_DESTROY_ARMOR\
|| (otmp)->otyp == SCR_DESTROY_WEAPON\
|| (otmp)->otyp == SCR_STONING\
|| (otmp)->otyp == SCR_AMNESIA\
|| (otmp)->otyp == BAG_OF_TRICKS\
|| (otmp)->otyp == WAN_STONING\
|| (otmp)->otyp == WAN_DISINTEGRATION\
|| (otmp)->otyp == WAN_PARALYSIS\
|| (otmp)->otyp == WAN_CURSE_ITEMS\
|| (otmp)->otyp == WAN_AMNESIA\
|| (otmp)->otyp == WAN_BAD_LUCK\
|| (otmp)->otyp == WAN_REMOVE_RESISTANCE\
|| (otmp)->otyp == WAN_CORROSION\
|| (otmp)->otyp == WAN_STARVATION\
|| (otmp)->otyp == WAN_CONFUSION\
|| (otmp)->otyp == WAN_SLIMING\
|| (otmp)->otyp == WAN_LYCANTHROPY\
|| (otmp)->otyp == WAN_FUMBLING\
|| (otmp)->otyp == WAN_PUNISHMENT\
|| (otmp)->otyp == SCR_PUNISHMENT\
|| (otmp)->otyp == WAN_MAKE_VISIBLE\
|| (otmp)->otyp == SCR_SUMMON_BOSS\
|| (otmp)->otyp == SCR_WOUNDS\
|| (otmp)->otyp == SCR_BULLSHIT\
|| (otmp)->otyp == SCR_CHAOS_TERRAIN\
|| (otmp)->otyp == SCR_NASTINESS\
|| (otmp)->otyp == SCR_DEMONOLOGY\
|| (otmp)->otyp == SCR_ELEMENTALISM\
|| (otmp)->otyp == SCR_GIRLINESS\
|| (otmp)->otyp == WAN_SUMMON_SEXY_GIRL\
|| (otmp)->otyp == WAN_HASTE_MONSTER)

				
/* Eggs and other food */
#define MAX_EGG_HATCH_TIME 200	/* longest an egg can remain unhatched */
#define stale_egg(egg)	((monstermoves - (egg)->age) > (2*MAX_EGG_HATCH_TIME))
#define ofood(o) ((o)->otyp == CORPSE || (o)->otyp == EGG || (o)->otyp == TIN)
#define polyfodder(obj)	(ofood(obj) && (obj)->corpsenm == PM_CHAMELEON)
#define mlevelgain(obj) (ofood(obj) && (obj)->corpsenm == PM_WRAITH)
#define mhealup(obj)	(ofood(obj) && (obj)->corpsenm == PM_NURSE)
#define drainlevel(corpse) (mons[(corpse)->corpsenm].cnutrit*4/5)

/* Containers */
#define carried(o)	((o)->where == OBJ_INVENT)
#define mcarried(o)	((o)->where == OBJ_MINVENT)
#define Has_contents(o) (/* (Is_container(o) || (o)->otyp == STATUE) && */ \
			 (o)->cobj != (struct obj *)0)
#define Is_container(o) ((o)->otyp == MEDICAL_KIT || \
			 (o)->otyp >= LARGE_BOX && (o)->otyp <= BAG_OF_TRICKS)
#define Is_box(otmp)	((otmp)->otyp == LARGE_BOX || (otmp)->otyp == CHEST)
#ifdef WALLET_O_P
#define Is_mbag(otmp)	((otmp)->otyp == BAG_OF_HOLDING || \
                         ((otmp)->oartifact && \
                          (otmp)->oartifact == ART_WALLET_OF_PERSEUS) || \
  			             (otmp)->otyp == BAG_OF_TRICKS)
#else
#define Is_mbag(otmp)	((otmp)->otyp == BAG_OF_HOLDING || \
  			 (otmp)->otyp == BAG_OF_TRICKS)
#endif

/* dragon gear */
#define Is_dragon_scales(obj)	((obj)->otyp >= GRAY_DRAGON_SCALES && \
				 (obj)->otyp <= YELLOW_DRAGON_SCALES)
#define Is_dragon_mail(obj)	((obj)->otyp >= GRAY_DRAGON_SCALE_MAIL && \
				 (obj)->otyp <= YELLOW_DRAGON_SCALE_MAIL)
#define Is_dragon_armor(obj)	(Is_dragon_scales(obj) || Is_dragon_mail(obj))
#define Dragon_scales_to_pm(obj) &mons[PM_GRAY_DRAGON + (obj)->otyp \
				       - GRAY_DRAGON_SCALES]
#define Dragon_mail_to_pm(obj)	&mons[PM_GRAY_DRAGON + (obj)->otyp \
				      - GRAY_DRAGON_SCALE_MAIL]
#define Dragon_to_scales(pm)	(GRAY_DRAGON_SCALES + (pm - mons))

/* Elven gear */
#define is_elven_weapon(otmp)	((otmp)->otyp == ELVEN_ARROW\
				|| (otmp)->otyp == ELVEN_SPEAR\
				|| (otmp)->otyp == ELVEN_MACE\
				|| (otmp)->otyp == ELVEN_LANCE\
				|| (otmp)->otyp == ELVEN_DAGGER\
				|| (otmp)->otyp == ELVEN_SHORT_SWORD\
				|| (otmp)->otyp == ELVEN_BROADSWORD\
				|| (otmp)->otyp == ELVEN_BOW)
#define is_elven_obj(otmp)	(is_elven_armor(otmp) || is_elven_weapon(otmp))

#define is_droven_weapon(otmp)	((otmp)->otyp == DROVEN_ARROW\
				|| (otmp)->otyp == DROVEN_SPEAR\
				|| (otmp)->otyp == DROVEN_LANCE\
				|| (otmp)->otyp == DROVEN_DAGGER\
				|| (otmp)->otyp == DROVEN_SHORT_SWORD\
				|| (otmp)->otyp == DROVEN_GREATSWORD\
				|| (otmp)->otyp == DROVEN_BOW\
				|| (otmp)->otyp == DROVEN_CROSSBOW\
				|| (otmp)->otyp == DROVEN_BOLT)

/* Orcish gear */
#define is_orcish_obj(otmp)	(is_orcish_armor(otmp)\
				|| (otmp)->otyp == ORCISH_ARROW\
				|| (otmp)->otyp == ORCISH_SPEAR\
				|| (otmp)->otyp == ORCISH_DAGGER\
				|| (otmp)->otyp == ORCISH_SHORT_SWORD\
				|| (otmp)->otyp == ORCISH_BOW)

/* Dwarvish gear */
#define is_dwarvish_obj(otmp)	(is_dwarvish_armor(otmp)\
				|| (otmp)->otyp == DWARVISH_SPEAR\
				|| (otmp)->otyp == DWARVISH_BATTLE_AXE\
				|| (otmp)->otyp == DWARVISH_SHORT_SWORD\
				|| (otmp)->otyp == DWARVISH_MATTOCK)

/* Gnomish gear */
#define is_gnomish_obj(otmp)	(is_gnomish_armor(otmp))

/* Light sources */
#define Is_candle(otmp)	((otmp)->otyp == TALLOW_CANDLE || \
			 (otmp)->otyp == WAX_CANDLE || \
			 (otmp)->otyp == JAPAN_WAX_CANDLE || \
			 (otmp)->otyp == OIL_CANDLE || \
			 (otmp)->otyp == MAGIC_CANDLE)
/* maximum amount of oil in a potion of oil */
#define MAX_OIL_IN_FLASK 400

/* MAGIC_LAMP intentionally excluded below */
/* age field of this is relative age rather than absolute */
#define age_is_relative(otmp)	((otmp)->otyp == BRASS_LANTERN\
				|| (otmp)->otyp == OIL_LAMP\
				|| (otmp)->otyp == TORCH\
				|| (otmp)->otyp == CANDELABRUM_OF_INVOCATION\
				|| (otmp)->otyp == TALLOW_CANDLE\
				|| (otmp)->otyp == WAX_CANDLE\
				|| (otmp)->otyp == JAPAN_WAX_CANDLE\
				|| (otmp)->otyp == OIL_CANDLE\
				|| (otmp)->otyp == POT_OIL)
/* object can be ignited */
#define ignitable(otmp)	((otmp)->otyp == BRASS_LANTERN\
				|| (otmp)->otyp == OIL_LAMP\
				|| (otmp)->otyp == TORCH\
				|| (otmp)->otyp == CANDELABRUM_OF_INVOCATION\
				|| (otmp)->otyp == TALLOW_CANDLE\
				|| (otmp)->otyp == WAX_CANDLE\
				|| (otmp)->otyp == JAPAN_WAX_CANDLE\
				|| (otmp)->otyp == OIL_CANDLE\
				|| (otmp)->otyp == MAGIC_CANDLE\
				|| (otmp)->otyp == POT_OIL)

/* special stones */
#define is_nastygraystone(obj)	((obj)->otyp == RIGHT_MOUSE_BUTTON_STONE || \
				 (obj)->otyp == DISPLAY_LOSS_STONE || \
				 (obj)->otyp == SPELL_LOSS_STONE || \
				 (obj)->otyp == YELLOW_SPELL_STONE || \
				 (obj)->otyp == AUTO_DESTRUCT_STONE || \
				 (obj)->otyp == MEMORY_LOSS_STONE || \
				 (obj)->otyp == INVENTORY_LOSS_STONE || \
				 (obj)->otyp == BLACKY_STONE || \
				 (obj)->otyp == MENU_BUG_STONE || \
				 (obj)->otyp == SPEEDBUG_STONE || \
				 (obj)->otyp == SUPERSCROLLER_STONE || \
				 (obj)->otyp == FREE_HAND_BUG_STONE || \
				 (obj)->otyp == UNIDENTIFY_STONE || \
				 (obj)->otyp == STONE_OF_THIRST || \
				 (obj)->otyp == UNLUCKY_STONE || \
				 (obj)->otyp == SHADES_OF_GREY_STONE || \
				 (obj)->otyp == STONE_OF_FAINTING || \
				 (obj)->otyp == STONE_OF_CURSING || \
				 (obj)->otyp == STONE_OF_DIFFICULTY || \
				 (obj)->otyp == DEAFNESS_STONE || \
				 (obj)->otyp == ANTIMAGIC_STONE || \
				 (obj)->otyp == WEAKNESS_STONE || \
				 (obj)->otyp == ROT_THIRTEEN_STONE || \
				 (obj)->otyp == BISHOP_STONE || \
				 (obj)->otyp == CONFUSION_STONE || \
				 (obj)->otyp == DROPBUG_STONE || \
				 (obj)->otyp == DSTW_STONE || \
				 (obj)->otyp == STATUS_STONE || \
				 (obj)->otyp == ALIGNMENT_STONE || \
				 (obj)->otyp == STAIRSTRAP_STONE || \
				 (obj)->otyp == STONE_OF_INTRINSIC_LOSS || \
				 (obj)->otyp == BLOOD_LOSS_STONE || \
				 (obj)->otyp == BAD_EFFECT_STONE || \
				 (obj)->otyp == TRAP_CREATION_STONE || \
				 (obj)->otyp == STONE_OF_VULNERABILITY || \
				 (obj)->otyp == ITEM_TELEPORTING_STONE || \
				 (obj)->otyp == NASTY_STONE || \
				 (obj)->otyp == FARLOOK_STONE || \
				 (obj)->otyp == CAPTCHA_STONE || \
				 (obj)->otyp == RESPAWN_STONE || \
				 (obj)->otyp == AMNESIA_STONE || \
				 (obj)->otyp == BIGSCRIPT_STONE || \
				 (obj)->otyp == BANK_STONE || \
				 (obj)->otyp == MAP_STONE || \
				 (obj)->otyp == TECHNIQUE_STONE || \
				 (obj)->otyp == DISENCHANTMENT_STONE || \
				 (obj)->otyp == VERISIERT_STONE || \
				 (obj)->otyp == CHAOS_TERRAIN_STONE || \
				 (obj)->otyp == MUTENESS_STONE || \
				 (obj)->otyp == ENGRAVING_STONE || \
				 (obj)->otyp == MAGIC_DEVICE_STONE || \
				 (obj)->otyp == BOOK_STONE || \
				 (obj)->otyp == LEVEL_STONE || \
				 (obj)->otyp == QUIZ_STONE || \
				 (obj)->otyp == METABOLIC_STONE || \
				 (obj)->otyp == STONE_OF_NO_RETURN || \
				 (obj)->otyp == EGOSTONE || \
				 (obj)->otyp == FAST_FORWARD_STONE || \
				 (obj)->otyp == ROTTEN_STONE || \
				 (obj)->otyp == UNSKILLED_STONE || \
				 (obj)->otyp == LOW_STAT_STONE || \
				 (obj)->otyp == TRAINING_STONE || \
				 (obj)->otyp == EXERCISE_STONE || \
				 (obj)->otyp == UNINFORMATION_STONE )

#define is_graystone(obj)	((obj)->otyp == LUCKSTONE || \
				 (obj)->otyp == LOADSTONE || \
				 (obj)->otyp == FLINT     || \
				 (obj)->otyp == TALC     || \
				 (obj)->otyp == GRAPHITE     || \
				 (obj)->otyp == VOLCANIC_GLASS_FRAGMENT     || \
				 (obj)->otyp == TOUCHSTONE || \
				 (obj)->otyp == HEALTHSTONE || \
				 (obj)->otyp == SALT_CHUNK || \
				 (obj)->otyp == MANASTONE || \
				 (obj)->otyp == SMALL_PIECE_OF_UNREFINED_MITHR || \
				 (obj)->otyp == SILVER_SLINGSTONE || \
				 (obj)->otyp == STONE_OF_MAGIC_RESISTANCE || \
				 (obj)->otyp == LOADBOULDER || \
				 (obj)->otyp == STARLIGHTSTONE || \
				 (obj)->otyp == SLEEPSTONE || \
				 (obj)->otyp == WHETSTONE || is_nastygraystone(obj) )


/* misc */
#ifdef KOPS
#define is_flimsy(otmp)		(objects[(otmp)->otyp].oc_material <= LEATHER || \
				 (otmp)->otyp == RUBBER_HOSE)
#else
#define is_flimsy(otmp)		(objects[(otmp)->otyp].oc_material <= LEATHER)
#endif

/* helpers, simple enough to be macros */
#define is_plural(o)	((o)->quan > 1 || \
			 (o)->oartifact == ART_EYES_OF_THE_OVERWORLD)

/* Flags for get_obj_location(). */
#define CONTAINED_TOO	0x1
#define BURIED_TOO	0x2

#endif /* OBJ_H */
