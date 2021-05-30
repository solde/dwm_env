/* appearance */
extern const unsigned int borderpx  = 3;        /* border pixel of windows */
extern const unsigned int gappx     = 6;        /* gap pixel between windows */
extern const unsigned int snap      = 24;       /* snap pixel */
extern const int showbar            = 1;        /* 0 means no bar */
extern const int topbar             = 1;        /* 0 means bottom bar */
extern const int user_bh     		= 20;	 	/* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
extern const char *fonts[]          = { "Fira Sans:size=12" };
extern const char dmenufont[]       = "Fira Sans:size=12";
extern const char col_arc1[]        = "#404552";
extern const char col_arc2[]        = "#383c4a";
extern const char col_ard3[]        = "#4b5162";
extern const char col_arc4[]        = "#5294e2";
extern const char col_arc5[]        = "#7c818c";
extern const char col_white[]	    = "#ffffff";
extern const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_arc2, col_arc5 },
	[SchemeSel]  = { col_white, col_arc2, col_arc4 },
};