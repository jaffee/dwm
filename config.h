/* See LICENSE file for copyright and license details. */
#include "gaplessgrid.c"

/* appearance */
static const char font[]            = "Liberation Mono:pixelsize=12:antialias=false:autohint=false";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#005577";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            True,        -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       False,       -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = True; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "###",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod1Mask
#define WINKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ WINKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ WINKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ WINKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ WINKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "uxterm", NULL };
static const char *slock[]    = { "slock", NULL };
static const char *volupcmd[]  = { "aumix", "-v", "+5", NULL };
static const char *voldowncmd[]  = { "aumix", "-v", "-5", NULL };
static const char *mutecmd[]  = { "aumix", "-v", "0", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ WINKEY,			XK_g, 	   setlayout,	   {.v = &layouts[3] } },
	{ WINKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ WINKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ WINKEY|ShiftMask,             XK_l,      spawn,          {.v = slock } },
	{ WINKEY,                       XK_b,      togglebar,      {0} },
	{ WINKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ WINKEY,                       XK_Tab,    focusstack,     {.i = +1 } },
	{ WINKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ WINKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ WINKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ WINKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ WINKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ WINKEY,                       XK_Return, zoom,           {0} },
	{ WINKEY|ShiftMask,             XK_Tab,    view,           {0} },
	{ WINKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ WINKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ WINKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ WINKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ WINKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ WINKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ WINKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ WINKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ WINKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ WINKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ WINKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
        {0,                             XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd} },
        {0,                             XF86XK_AudioMute, spawn, {.v = mutecmd} },
        {0,                             XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd} },
	{ MODKEY,                       XK_Tab,    x_nexttag,       {0} },
        { MODKEY|ShiftMask,             XK_Tab,    x_prevtag,       {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ WINKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

