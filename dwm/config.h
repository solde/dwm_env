/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* Include layouts */
#include "fibonacci.c"
#include "tatami.c"

/* Patches*/
void movestack(const Arg *arg);

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gap pixel between windows */
static const unsigned int snap      = 24;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh     		= 20;	 	/* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "Fira Sans:size=6" };
static const char dmenufont[]       = "Fira Sans:size=6";
static const char col_arc1[]        = "#404552";
static const char col_arc2[]        = "#383c4a";
static const char col_ard3[]        = "#4b5162";
static const char col_arc4[]        = "#5294e2";
static const char col_arc5[]        = "#7c818c";
static const char col_white[]	    = "#ffffff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_arc2, col_arc5 },
	[SchemeSel]  = { col_white, col_arc2, col_arc4 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
 	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "|+|",      tatami },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define CTRLKEY ControlMask
#define SHIFTKEY ShiftMask
#define VOLUPKEY XF86XK_AudioRaiseVolume
#define VOLDOWNKEY XF86XK_AudioLowerVolume
#define VOLMUTEKEY XF86XK_AudioMute
#define BRUPKEY XF86XK_MonBrightnessUp
#define BRDOWNKEY XF86XK_MonBrightnessDown

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-fn", "Fira Sans-6", "-nb", col_arc2, "-nf", col_white, "-sb", col_arc4, "-sf", col_white, NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *web[]  = { "chromium", NULL };
static const char *gimp[]  = { "gimp", NULL };
static const char *termcd2[]  = { "gnome-terminal", NULL };
static const char *steam[]  = { "steam", NULL };
static const char *discord[]  = { "discord-canary", NULL };
static const char *vscode[]  = { "code-insiders", NULL };
static const char *nmtui[]  = { "kitty", "-e", "nmtui"};
static const char *pavucontrol[]  = { "pavucontrol", NULL };
static const char *screenshot[]  = { "escrotum", "-s", "-C" };
static const char *lockscreen[]  = { "betterlockscreen", "-l", "dim" };
static const char *pcmanfm[]  = { "pcmanfm", NULL};
static const char *calculator[]  = { "galculator", NULL};
static const char *whatsapp[]  = { "whatsapp", NULL};
static const char *volup[]  = { "amixer", "-q", "-D", "pulse", "sset", "Master", "5%+", "unmute"};
static const char *voldown[]  = { "amixer", "-q", "-D", "pulse", "sset", "Master", "5%-", "unmute"};
static const char *volmute[]  = { "amixer", "-q", "-D", "pulse", "sset", "Master", "toggle"};
static const char *BrUp[]  = { "xbacklight", "-inc", "5"};
static const char *BrDown[]  = { "xbacklight", "-dec", "5"};
static const char *poweroff[]  = { "poweroff", NULL };
static const char *musiqueta[] = 	{ "mpdmenu", NULL };
static const char *mpdVolUp[] = 	{ "mpdmenu", "volume", "+5" };
static const char *mpdVolDw[] = 	{ "mpdmenu", "volume", "-5" };
static const char *mpdNext[] = 		{ "mpdmenu", "next" };
static const char *mpdPrev[] = 		{ "mpdmenu", "prev" };
static const char *mpdToggle[] = 	{ "mpdmenu", "toggle" };
static const char *mpdStop[] = 	{ "mpdmenu", "stop" };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,             			XK_Return, spawn,          {.v = termcmd } },
	{ ALTKEY,           			XK_w, 	   spawn,          {.v = web } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Right,  setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_Left,   setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  cyclelayout,    {.i = +1} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Up,     movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Down,   movestack,      {.i = -1 } },
	{ ALTKEY,             			XK_Return, spawn,      	   {.v = termcd2 } },
	{ ALTKEY,             			XK_c,      spawn,      	   {.v = vscode } },
	{ ALTKEY,             			XK_g,      spawn,      	   {.v = gimp } },
	{ MODKEY,             			XK_g,      spawn,      	   {.v = steam } },
	{ MODKEY,             			XK_n,      spawn,      	   {.v = nmtui } },
	{ MODKEY,             			XK_p,      spawn,      	   {.v = pavucontrol } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,      	   {.v = screenshot } },
	{ ALTKEY,             			XK_f,      spawn,      	   {.v = pcmanfm } },
	{ ALTKEY,             			XK_q,      spawn,      	   {.v = screenshot } },
	{ ALTKEY,             			XK_q,      spawn,      	   {.v = calculator } },
	{ ALTKEY|CTRLKEY,      			XK_l,      spawn,      	   {.v = lockscreen } },
	{ ALTKEY,      			        XK_d,      spawn,      	   {.v = discord } },
	{ ALTKEY,      			        XK_m,      spawn,      	   {.v = whatsapp } },
	{ 0,      			            VOLUPKEY,  spawn,      	   {.v = volup } },
	{ 0,      			            VOLDOWNKEY,spawn,      	   {.v = voldown } },
	{ 0,      			            VOLMUTEKEY,spawn,      	   {.v = volmute } },
	{ 0,      			            BRUPKEY,   spawn,      	   {.v = BrUp } },
    { 0,      			            BRDOWNKEY, spawn,      	   {.v = BrDown } },
	{ ALTKEY|CTRLKEY,      			XK_Escape, spawn,      	   {.v = poweroff } },
	{ CTRLKEY|ALTKEY,      			XK_Return, spawn,     	   {.v = musiqueta } },
	{ CTRLKEY|ALTKEY,      			XK_comma,  spawn,      	   {.v = mpdPrev } },
	{ CTRLKEY|ALTKEY,      			XK_period, spawn,          {.v = mpdNext } },
	{ CTRLKEY|ALTKEY|ShiftMask,		XK_comma,  spawn,      	   {.v = mpdVolDw } },
	{ CTRLKEY|ALTKEY|ShiftMask,		XK_period, spawn,      	   {.v = mpdVolUp } },
	{ CTRLKEY|ALTKEY,      			XK_p,      spawn,      	   {.v = mpdToggle } },
	{ CTRLKEY|ALTKEY|ShiftMask,     XK_p,      spawn,      	   {.v = mpdStop } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,            	XK_e,      quit,           {0} },
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
void movestack(const Arg *arg) {
	Client *c = NULL, *p = NULL, *pc = NULL, *i;

	if(arg->i > 0) {
		/* find the client after selmon->sel */
		for(c = selmon->sel->next; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);
		if(!c)
			for(c = selmon->clients; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);

	}
	else {
		/* find the client before selmon->sel */
		for(i = selmon->clients; i != selmon->sel; i = i->next)
			if(ISVISIBLE(i) && !i->isfloating)
				c = i;
		if(!c)
			for(; i; i = i->next)
				if(ISVISIBLE(i) && !i->isfloating)
					c = i;
	}
	/* find the client before selmon->sel and c */
	for(i = selmon->clients; i && (!p || !pc); i = i->next) {
		if(i->next == selmon->sel)
			p = i;
		if(i->next == c)
			pc = i;
	}

	/* swap c and selmon->sel selmon->clients in the selmon->clients list */
	if(c && c != selmon->sel) {
		Client *temp = selmon->sel->next==c?selmon->sel:selmon->sel->next;
		selmon->sel->next = c->next==selmon->sel?c:c->next;
		c->next = temp;

		if(p && p != c)
			p->next = c;
		if(pc && pc != selmon->sel)
			pc->next = selmon->sel;

		if(selmon->sel == selmon->clients)
			selmon->clients = c;
		else if(c == selmon->clients)
			selmon->clients = selmon->sel;

		arrange(selmon);
	}
}
