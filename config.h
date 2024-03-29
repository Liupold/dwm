/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include <stdlib.h>
static char TERMINAL[] = "st";

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 8;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Fira Code:size=13", "Font Awesome 5 Free Solid:size=14" };
static const char dmenufont[]       = "Fira Code:size=14";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { " 1 ", " 2 ", " 3 ", " 4 ", " 5 ", " 6 ", " 7 ", " 8 ", " 9 " };

static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
   */
  /* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
  { "Gimp",            NULL,     NULL,           0,         1,          0,           0,        -1 },
  { "St",              NULL,     NULL,           0,         0,          1,          -1,        -1 },
  { "URxvt",           NULL,     NULL,           0,         0,          1,          -1,        -1 },
  { NULL,              NULL,     "Event Tester", 0,         1,          0,           1,        -1 }, /* xev */
  { "GNU Octave",      NULL,     NULL,           0,         1,          0,           1,        -1 }, /* octave */
  { "matplotlib",      NULL,     NULL,           0,         1,          0,           1,        -1 }, /* matplotlib */
  { "F_TERM",          NULL,     NULL,           0,         1,          0,           1,        -1 }, /* floating term */
  { "gnuplot_qt",      NULL,     NULL,           0,         1,          0,           1,        -1 }, /* floating term */
  { NULL,              NULL, "F_TERM",           0,         1,          0,           1,        -1 }, /* floating term */
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "",      tile },    /* first entry is default */
  { "",      NULL },    /* no layout function means floating behavior */
  { "",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define TERMCMD(cmd) { .v = (const char*[]){ "urxvt","-e","/bin/sh","-c",cmd,NULL } }
#define FTERMCMD(cmd) { .v = (const char*[]){ "urxvt", "-g", "=100x25+490+306", "-title",  "F_TERM", "-e","/bin/sh","-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char *alt_termcmd[]  = { "tabbed", "-c", TERMINAL, "-embed", NULL};
static const char *upvol[]   = { "amixer", "-q", "-D", "pulse", "sset", "Master", "5%+", NULL };
static const char *downvol[]   = { "amixer", "-q", "-D", "pulse", "sset", "Master", "5%-", NULL };
static const char *mutevol[] = { "/usr/bin/amixer", "sset","0", "toggle",NULL };
static const char *uplight[] = { "/usr/bin/light", "-A",   "+5",  NULL };
static const char *downlight[] = { "/usr/bin/light", "-U",   "+5",  NULL };

static Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
  { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
  { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = alt_termcmd } },
  { MODKEY,                       XK_b,      togglebar,      {0} },
  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
  { MODKEY|ShiftMask,             XK_k,      incnmaster,     {.i = +1 } },
  { MODKEY|ShiftMask,             XK_j,      incnmaster,     {.i = -1 } },
  { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_f,      togglefullscr,  {0} },
  { MODKEY,                       XK_m,      zoom,           {0} },
  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY,                       XK_c,      killclient,     {0} },
  { MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
  { MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_space,  setlayout,      {0} },
  { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
  TAGKEYS(                        XK_1,                      0)
  TAGKEYS(                        XK_2,                      1)
  TAGKEYS(                        XK_3,                      2)
  TAGKEYS(                        XK_4,                      3)
  TAGKEYS(                        XK_5,                      4)
  TAGKEYS(                        XK_6,                      5)
  TAGKEYS(                        XK_7,                      6)
  TAGKEYS(                        XK_8,                      7)
  TAGKEYS(                        XK_9,                      8)
  { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    { MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
  { MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
  { MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
  { 0,               XF86XK_AudioLowerVolume,   spawn, {.v = downvol  } },
  { 0,               XF86XK_AudioMute,          spawn, {.v = mutevol  } },
  { 0,               XF86XK_AudioRaiseVolume,   spawn, {.v = upvol    } },
  { 0,               XF86XK_MonBrightnessUp,    spawn, {.v = uplight  } },
  { 0,               XF86XK_MonBrightnessDown,  spawn, {.v = downlight} },
  { MODKEY,          XK_Down,                   spawn, {.v = downvol  } },
  { MODKEY,          XK_Up,                     spawn, {.v = upvol  } },
  { MODKEY,          XK_Right,                  spawn, {.v = uplight  } },
  { MODKEY,          XK_Left,                   spawn, {.v = downlight  } },
  { MODKEY|ControlMask,          XK_grave,      spawn,     SHCMD("dunstctl history-pop") },
  { MODKEY|ControlMask,          XK_space,      spawn,     SHCMD("dunstctl close") },
  { MODKEY|ShiftMask|ControlMask,          XK_space,      spawn,     SHCMD("dunstctl close-all") },
  { MODKEY|ControlMask,          XK_Return,     spawn,     SHCMD("dunstctl action") },
  { MODKEY,          XK_w,                      spawn,     SHCMD("$BROWSER")    },
  { MODKEY,          XK_p,                      spawn,     FTERMCMD("$MUSIC_PLAYER")      },
  { MODKEY,          XK_slash,                  spawn,     TERMCMD("lf")        },
  { MODKEY,          XK_n,                      spawn,     FTERMCMD("newsboat")  },
  { MODKEY,          XK_v,                      spawn,     FTERMCMD("pydoro")  },
  { MODKEY,          XK_g,                      spawn,     FTERMCMD("clifm")  },
  { MODKEY,          XK_s,                      spawn,     SHCMD("dmenu-cmus")  },
  { MODKEY,          XK_backslash,              spawn,     SHCMD("playerctl -p $MUSIC_PLAYER,%any,chromium play-pause") },
  { MODKEY,          XK_bracketleft,            spawn,     SHCMD("playerctl -p $MUSIC_PLAYER,%any,chromium previous") },
  { MODKEY,          XK_bracketright,           spawn,     SHCMD("playerctl -p $MUSIC_PLAYER,%any,chromium next") },
  { 0,               XF86XK_Calculator,         spawn,     FTERMCMD("python3 -i -c 'from math import *'") },
  { MODKEY,          XK_i,                      spawn,     FTERMCMD("$EDITOR ~/.cache/spad") },
  { MODKEY,          XK_o,                      spawn,     SHCMD("~/.local/bin/dmenu-search") },
  { MODKEY|ShiftMask,          XK_Down,         spawn,     SHCMD("playerctl -p $MUSIC_PLAYER,%any -i chromium volume 0.1- && playerctl -p cmus,%any -i chromium metadata -f 'PLAYERCTL:[ {{emoji(status)}} {{trunc(title, 30)}} ({{trunc(artist, 20)}}) | {{emoji(volume)}} {{trunc(volume * 100, 4)}}  ]' > /tmp/dwmbar") },
  { MODKEY|ShiftMask,          XK_Up,           spawn,     SHCMD("playerctl -p $MUSIC_PLAYER,%any -i chromium volume 0.1+ && playerctl -p cmus,%any -i chromium metadata -f 'PLAYERCTL:[ {{emoji(status)}} {{trunc(title, 30)}} ({{trunc(artist, 20)}}) | {{emoji(volume)}} {{trunc(volume * 100, 4)}}  ]' > /tmp/dwmbar") },
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

