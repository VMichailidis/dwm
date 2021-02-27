/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 40;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 40;       /* vert inner gap between windows */
static const unsigned int gappoh    = 50;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 50;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "roboto:size=11" };
static const char dmenufont[]       = "roboto:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#092a48";  /*#005577*/
static const char col_cyanout[]     = "#0a5c42";  /*#a188a6*/
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_cyan, col_gray2 }, /*bg = col_gray1*/
	[SchemeSel]  = { col_gray4, col_gray1,  col_cyanout  }, /*bg = col_cyan*/
};

/* tagging */
static const char *tags[] = { "α'", "β'", "γ'", "δ'", "ε'", "στ'", "ζ'", "η'", "θ'" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
//#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "[Φ]",      spiral }, /* first entry is default */
	{ "[M]",      monocle },
	{ "[]=",      tile },    
// 	{ "[\\]",      dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_v,     ACTION##stack, {.i = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run","-l", "20", "-m", dmenumon, "-fn", dmenufont, NULL };
static const char *bookmarks[] = {"/home/basil/.local/bin/bookmarks.sh", NULL };
static const char *termcmd[]  = { "st", NULL };

#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	//Spawners
	{ MODKEY,                       XK_u, 	spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_n, 	spawn,          {.v = bookmarks } },
	{ MODKEY,			XK_Return, 	spawn,          SHCMD("alacritty") },
	{ MODKEY,			XK_semicolon,	spawn,          SHCMD("brave") },
	{ MODKEY|ShiftMask,		XK_s,	   	spawn,          SHCMD("flameshot gui") },
	{ MODKEY,			XK_f,	   	spawn,          SHCMD("alacritty -e ranger") },
	{ MODKEY,			XK_t,	   	spawn,          SHCMD("transmission-gtk") },
	{ MODKEY,			XK_d,	   	spawn,          SHCMD("discord") },
	{ MODKEY,			XK_i,	   	spawn,          SHCMD("sxiv ~/Pictures/Schedule.png") },

	//Navigation
	STACKKEYS(MODKEY,                          	focus)
	STACKKEYS(MODKEY|ShiftMask,                	push)
	{ MODKEY,                       XK_h,      	setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      	setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_o,      	shiftview,      {.i = -1} },
	{ MODKEY|ShiftMask,             XK_o,      	shifttag,       {.i = -1} },
	{ MODKEY,                       XK_p,      	shiftview,      {.i = +1} },
	{ MODKEY|ShiftMask,             XK_p,      	shifttag,       {.i = +1} },
	{ MODKEY,			XK_q,      	killclient,     {0} },
	//Vanitygaps
	{ MODKEY,             	        XK_m,     	incrgaps,       {.i = +5 } },
	{ MODKEY|ShiftMask,   	        XK_m,     	incrgaps,       {.i = -5 } },
	//Layouts
	{ MODKEY,                       XK_a,      	setlayout,      {.v = &layouts[0]} },/* Fibonacci Layout*/
	{ MODKEY|ShiftMask,             XK_a,      	setlayout,      {.v = &layouts[1]} },/* Monocle*/
	{ MODKEY,                       XK_z,      	setlayout,      {.v = &layouts[2]} },/* Floating*/
	{ MODKEY|ShiftMask,             XK_z,      	setlayout,      {.v = &layouts[3]} },/* Tile*/
	{ MODKEY,                       XK_space,  	setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  	togglefloating, {0} },
	//Navigate Tabs
	{ MODKEY,                       XK_0,      	view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      	tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,      	                0)
	TAGKEYS(                        XK_2,      	                1)
	TAGKEYS(                        XK_3,      	                2)
	TAGKEYS(                        XK_4,      	                3)
	TAGKEYS(                        XK_5,      	                4)
	TAGKEYS(                        XK_6,      	                5)
	TAGKEYS(                        XK_7,      	                6)
	TAGKEYS(                        XK_8,      	                7)
	TAGKEYS(                        XK_9,      	                8)
	{ MODKEY|ShiftMask,             XK_q,      	quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

