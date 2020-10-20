/*      SCCS Id: @(#)tech.h    3.2     98/Oct/30                  */
/* Original Code by Warren Cheung                                 */
/* Copyright 1986, M. Stephenson				  */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef TECH_H
#define TECH_H

/* An array of this struct holds your current techs */
struct tech {
	int         t_id;                  /* tech id 
					    * 	Unique number for the tech
					    *	see the #defines below
					    */
	xchar       t_lev;                 /* power level 
					    *   This is the level 
					    *   you were when you got the tech
				            */
	
	int         t_tout;                /* timeout - time until tech can be 
					    * used again 
					    */
	int         t_inuse;               /* time till effect stops 
					    */
	long        t_intrinsic;           /* Source(s) */
#define OUTSIDE_LEVEL	TIMEOUT            /* Level "from outside" started at */
};

struct innate_tech {
	schar   ulevel; /* gains tech at level */
	short   tech_id; /* the tech unique ID*/ 
	int	tech_lev; /* starts at level */
};

struct blitz_tab {
        const char *blitz_cmd; /* the typed command */
        const int blitz_len; /* The length of blitz_cmd */
        int (*blitz_funct)(void); /* function called when the command is executed */
        const int blitz_tech; /* the tech designation - determines tech needed
         		       * to access this blitz and the name of the blitz
          		       */
        const int blitz_type; /* the type of blitz chain */
#define BLITZ_START 	0 /* Starts the chain */
#define BLITZ_CHAIN 	1 /* Goes anywhere in the chain (usually middle) */
#define BLITZ_END 	2 /* Finishes the chain */
};


#define NO_TECH 	0
#define T_BERSERK 	1
#define T_KIII 		2
#define T_RESEARCH 	3
#define T_SURGERY 	4
#define T_REINFORCE 	5
#define T_FLURRY 	6
#define T_PRACTICE 	7
#define T_EVISCERATE 	8
#define T_HEAL_HANDS 	9
#define T_CALM_STEED 	10
#define T_TURN_UNDEAD 	11
#define T_VANISH 	12
#define T_CUTTHROAT 	13
#define T_BLESSING 	14
#define T_E_FIST 	15
#define T_PRIMAL_ROAR 	16
#define T_LIQUID_LEAP 	17
#define T_CRIT_STRIKE 	18
#define T_SIGIL_CONTROL 19
#define T_SIGIL_TEMPEST 20
#define T_SIGIL_DISCHARGE 21
#define T_RAISE_ZOMBIES 22
#define T_REVIVE 	23
#define T_WARD_FIRE 	24
#define T_WARD_COLD 	25
#define T_WARD_ELEC 	26
#define T_TINKER 	27
#define T_RAGE	 	28
#define T_BLINK	 	29
#define T_CHI_STRIKE 	30
#define T_DRAW_ENERGY 	31
#define T_CHI_HEALING 	32
#define T_DISARM 	33
#define T_DAZZLE 	34
#define T_BLITZ 	35
#define T_PUMMEL 	36
#define T_G_SLAM 	37
#define T_DASH	 	38
#define T_POWER_SURGE 	39
#define T_SPIRIT_BOMB 	40
#define T_DRAW_BLOOD	41

#define T_WORLD_FALL	42
#define T_CREATE_AMMO	43
#define T_POKE_BALL	44
#define T_ATTIRE_CHARM	45
#define T_SUMMON_TEAM_ANT	46
#define T_APPRAISAL	47
#define T_EGG_BOMB	48
#define T_BOOZE	49

#define T_INVOKE_DEITY	50

#define T_DOUBLE_TROUBLE	51

#define T_PHASE_DOOR	52

#define T_SECURE_IDENTIFY	53

#define T_IRON_SKIN	54
#define T_POLYFORM	55
#define T_CONCENTRATING	56
#define T_SUMMON_PET	57
#define T_DOUBLE_THROWNAGE	58
#define T_SHIELD_BASH	59
#define T_RECHARGE	60
#define T_SPIRITUALITY_CHECK	61
#define T_EDDY_WIND	62

#define T_BLOOD_RITUAL	63
#define T_ENT_S_POTION	64
#define T_LUCKY_GAMBLE	65

#define T_PANIC_DIGGING	66

#define T_DECONTAMINATE	67
#define T_WONDERSPELL	68
#define T_RESET_TECHNIQUE	69

#define T_SILENT_OCEAN	70
#define T_GLOWHORN	71
#define T_INTRINSIC_ROULETTE	72
#define T_SPECTRAL_SWORD	73
#define T_REVERSE_IDENTIFY	74
#define T_DETECT_TRAPS	75
#define T_DIRECTIVE	76
#define T_REMOVE_IMPLANT	77
#define T_REROLL_IMPLANT	78
#define T_TIME_STOP	79
#define T_STAT_RESIST	80
#define T_SWAP_WEAPON	81
#define T_MILDEN_CURSE	82
#define T_FORCE_FIELD	83
#define T_POINTINESS	84
#define T_BUG_SPRAY	85
#define T_WHIRLSTAFF	86
#define T_DELIBERATE_CURSE	87
#define T_ACQUIRE_STEED	88
#define T_SADDLING	89
#define T_SHOPPING_QUEEN	90
#define T_BEAUTY_CHARM	91
#define T_ASIAN_KICK	92
#define T_LEGSCRATCH_ATTACK	93
#define T_GROUND_STOMP	94
#define T_ATHLETIC_COMBAT	95
#define T_PRAYING_SUCCESS	96
#define T_OVER_RAY	97
#define T_ENCHANTERANG	98
#define T_BATMAN_ARSENAL	99
#define T_JOKERBANE	100
#define T_CALL_THE_POLICE	101
#define T_DOMINATE	102
#define T_INCARNATION	103
#define T_COMBO_STRIKE	104
#define T_FUNGOISM	105
#define T_BECOME_UNDEAD	106
#define T_JIU_JITSU	107
#define T_BLADE_ANGER	108
#define T_RE_TAMING	109
#define T_UNCURSE_SABER	110
#define T_ENERGY_CONSERVATION	111
#define T_ENCHANT_ROBE	112
#define T_WILD_SLASHING	113
#define T_ABSORBER_SHIELD	114
#define T_PSYCHO_FORCE	115
#define T_INTENSIVE_TRAINING	116
#define T_SURRENDER_OR_DIE	117
#define T_PERILOUS_WHIRL	118
#define T_SUMMON_SHOE	119
#define T_KICK_IN_THE_NUTS	120
#define T_DISARMING_KICK	121
#define T_INLAY_WARFARE	122

#define T_DIAMOND_BARRIER	123

#define T_STEADY_HAND	124
#define T_FORCE_FILLING	125
#define T_JEDI_TAILORING	126
#define T_INTRINSIC_SACRIFICE	127
#define T_BEAMSWORD	128
#define T_ENERGY_TRANSFER	129
#define T_SOFTEN_TARGET	130
#define T_PROTECT_WEAPON	131
#define T_POWERFUL_AURA	132
#define T_BOOSTAFF	133
#define T_CLONE_JAVELIN	134
#define T_REFUGE	135
#define T_DRAINING_PUNCH	136
#define T_ESCROBISM	137
#define T_PIRATE_BROTHERING	138
#define T_NUTS_AND_BOLTS	139
#define T_DECAPABILITY	140
#define T_NO_HANDS_CURSE	141
#define T_HIGH_HEELED_SNEAKERS	142
#define T_FORM_CHOICE	143
#define T_STAR_DIGGING	144
#define T_STARWARS_FRIENDS	145
#define T_USE_THE_FORCE_LUKE	146

#define T_WONDER_YONDER	147

#define T_ZAP_EM	148

#define T_CARD_TRICK	149
#define T_SKILLOMORPH	150

#define T_SHOTTY_BLAST	151
#define T_AMMO_UPGRADE	152
#define T_LASER_POWER	153
#define T_BIG_DADDY	154
#define T_SHUT_THAT_BITCH_UP	155
#define T_S_PRESSING	156
#define T_MELTEE	157
#define T_WOMAN_NOISES	158
#define T_EXTRA_LONG_SQUEAK	159
#define T_SEXUAL_HUG	160
#define T_SEX_CHANGE	161
#define T_EVEN_MORE_AMMO	162
#define T_DOUBLESELF	163
#define T_POLYFIX	164
#define T_SQUEAKY_REPAIR	165
#define T_BULLETREUSE	166
#define T_EXTRACHARGE	167

#define T_TERRAIN_CLEANUP	168

#define T_SYMBIOSIS	169
#define T_ADJUST_SYMBIOTE	170
#define T_HEAL_SYMBIOTE	171
#define T_BOOST_SYMBIOTE	172
#define T_POWERBIOSIS	173
#define T_IMPLANTED_SYMBIOSIS	174
#define T_ASSUME_SYMBIOTE	175
#define T_GENERATE_OFFSPRING	176

#define T_UNDERTOW	177
#define T_DEFINALIZE	178
#define T_ANTI_INERTIA	179
#define T_GRENADES_OF_COURSE	180

#define T_PREACHING	181
#define T_ON_THE_SAME_TEAM	182
#define T_PERMAMORPH	183

#define T_PACIFY	184
#define T_AFTERBURNER	185
#define T_BUGGARD	186
#define T_THUNDERSTORM	187
#define T_AUTOKILL	188
#define T_CHAIN_THUNDERBOLT	189
#define T_FLASHING_MISCHIEF	190
#define T_KAMEHAMEHA	191
#define T_SHADOW_MANTLE	192
#define T_VACUUM_STAR	193
#define T_BLADE_SHIELD	194
#define T_GREEN_WEAPON	195
#define T_BALLSLIFF	196
#define T_POLE_MELEE	197
#define T_CHOP_CHOP	198
#define T_BANISHMENT	199
#define T_PARTICIPATION_LOSS	200
#define T_WEAPON_BLOCKER	201
#define T_EXTRA_MEMORY	202
#define T_GRAP_SWAP	203
#define T_DIABOLIC_MINION	204

#define T_JEDI_JUMP	205
#define T_CHARGE_SABER	206
#define T_TELEKINESIS	207
 
#define MAXTECH 208

#endif /* TECH_H */
