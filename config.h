/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  	= 5;        /* border pixel of windows */
static const unsigned int floatborderpx = 0;        /* border pixel of windows */
static const unsigned int snap      	= 32;       /* snap pixel */
static const unsigned int gappih   	= 20;       /* horiz inner gap between windows */
static const unsigned int gappiv   	= 20;       /* vert inner gap between windows */
static const unsigned int gappoh   	= 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov   	= 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps         	= 0;        /* 1 means no outer gap when there is only one window */
static const int showbar           	= 1;        /* 0 means no bar */
static const int topbar            	= 1;        /* 0 means bottom bar */
static const char *fonts[]         	= { "roboto:bold:size=15" };
static const char dmenufont[]      	= "roboto:size=15";

// solar system scheme
// static char normbgcolor[]          	= "#251d1b"; 
// static char normbordercolor[]      	= "#251d1b";
// static char normfgcolor[]          	= "#c88d33";
// static char selfgcolor[]           	= "#251d1b";
// static char selbordercolor[]       	= "#c88d33";
// static char selbgcolor[]           	= "#c88d33";

// skullknight scheme
static char normbgcolor[]          	= "#310f16"; 
static char normbordercolor[]      	= "#310f16";
static char normfgcolor[]          	= "#adebfa";
static char selfgcolor[]           	= "#310f16";
static char selbordercolor[]       	= "#adebfa";
static char selbgcolor[]           	= "#adebfa";

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "α'", "β'", "γ'", "δ'", "ε'", "στ'", "ζ'", "η'", "θ'" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor    float x,y,w,h         floatborderpx*/
	{ "Gimp",     NULL,       NULL,       0,            1,           -1,        50,50,500,500,        5 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1,        50,50,500,500,        5 },
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
	{ MOD, XK_f,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_d,     ACTION##stack, {.i = INC(-1) } }, \
	// { MOD, XK_a,     ACTION##stack, {.i = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]       = { "dmenu_run","-l", "20", "-m", dmenumon, NULL };
static const char *bookmarks[]      = {"/home/basil/.luggage/scripts/bookmarks.sh", NULL };
static const char *dmenu_shutdown[] = {"/home/basil/.luggage/scripts/dmenu-shutdown.sh", NULL };
static const char *pick_color[]     = {"/home/basil/.luggage/scripts/pick-color.sh", NULL };
static const char *termcmd[]        = { "st", NULL };
static const char *xklayout[]         = {"/home/basil/.luggage/scripts/xklayout.sh", NULL};
static const char *up_vol[]   = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%",   NULL };
static const char *down_vol[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%",   NULL };
static const char *mute_vol[] = { "pactl", "set-sink-mute",   "@DEFAULT_SINK@", "toggle", NULL };
static const char *audio_play_pause[] = {"playerctl", "play-pause", NULL};

#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        			function        argument */
	//Spawners
	{ MODKEY,                       XK_apostrophe,	spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_u, 		    	spawn,          {.v = bookmarks } },
	{ MODKEY|ShiftMask,             XK_BackSpace,		spawn,          {.v = dmenu_shutdown} },
	{ MODKEY|ShiftMask,	        		XK_c,	   	    	spawn,          {.v = pick_color } },
	{ Mod1Mask,		                	XK_Shift_L,     spawn,        	{.v = xklayout} }, // change keyboard layout
	{ MODKEY,			            			XK_Return, 	    spawn,          SHCMD("kitty") },
	{ MODKEY,			            			XK_semicolon,		spawn,          SHCMD("brave") },
	{ MODKEY|ShiftMask,		        	XK_semicolon,		spawn,          SHCMD("brave --incognito") },
	{ MODKEY,			            			XK_c,	   	    	spawn,          SHCMD("flameshot gui") },
	{ MODKEY|ShiftMask,		        	XK_y,	   	    	spawn,          SHCMD("mullvad-vpn") },
	{ MODKEY,			            			XK_y,	   	    	spawn,          SHCMD("pavucontrol") },
	{ MODKEY,			            			XK_w,	   	    	spawn,          SHCMD("kitty -e ranger") },
	{ MODKEY|ShiftMask,			    		XK_w,		    		spawn,          SHCMD("nautilus") },
	{ MODKEY,			            			XK_t,	   	    	spawn,          SHCMD("discord") },
	{ MODKEY,			            			XK_i,	   	    	spawn,          SHCMD("sxiv ~/School/9th-Semester/Schedule.png") },
	{ MODKEY,			            			XK_a,	   	    	spawn,          SHCMD("kill -s USR1 $(pidof deadd-notification-center)") },

	//Navigation
	STACKKEYS(MODKEY,                          	focus)
	STACKKEYS(MODKEY|ShiftMask,                	push)
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
	{ MODKEY,                       XK_F5,     	xrdb,           {.v = NULL } },
{ MODKEY,                       	XK_0,      	view,           {.ui = ~0 } },
{ MODKEY|ShiftMask,             	XK_0,      	tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_h,      	setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_s,      	setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      	setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_g,      	setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_o,      	shiftview,      {.i = -1} },
	{ MODKEY,                       XK_e,      	shiftview,      {.i = -1} },
	{ MODKEY|ShiftMask,             XK_o,      	shifttag,       {.i = -1} },
	{ MODKEY|ShiftMask,             XK_e,      	shifttag,       {.i = -1} },
	{ MODKEY,                       XK_p,      	shiftview,      {.i = +1} },
	{ MODKEY,                       XK_r,      	shiftview,      {.i = +1} },
	{ MODKEY|ShiftMask,             XK_p,      	shifttag,       {.i = +1} },
	{ MODKEY|ShiftMask,             XK_r,      	shifttag,       {.i = +1} },
	{ MODKEY,												XK_q,      	killclient,     {0} },
//	toggle bar
	{ MODKEY,												XK_grave,		togglebar,			{0} },
	//Vanitygaps
	{ MODKEY,             	        XK_m,     	incrgaps,       {.i = +5 } },
	{ MODKEY|ShiftMask,   	        XK_m,     	incrgaps,       {.i = -5 } },
	//Layouts
	{ MODKEY,                       XK_x,      	setlayout,      {.v = &layouts[0]} },/* Fibonacci Layout*/
	{ MODKEY|ShiftMask,             XK_x,      	setlayout,      {.v = &layouts[1]} },/* Monocle*/
	{ MODKEY,                       XK_z,      	setlayout,      {.v = &layouts[3]} },/* BStack*/
	{ MODKEY|ShiftMask,             XK_z,      	setlayout,      {.v = &layouts[5]} },/* BStack Horizontal*/
	{ MODKEY,                       XK_space,  	setlayout,      {0} },
	{ MODKEY,             XK_v,  	togglefloating, {0} },
	//Navigate Tabs
	// volume
	{ 0, XF86XK_AudioLowerVolume, spawn, {.v = down_vol} },
	{ 0, XF86XK_AudioRaiseVolume, spawn, {.v = up_vol} },
	{ 0, XF86XK_AudioPlay          , spawn, {.v = audio_play_pause} },
	{ 0, XF86XK_AudioPause         , spawn, {.v = audio_play_pause} },
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
