/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


/*
 * This file is part of zsh, the Z shell.
 *
 * Copyright (c) 2000 Sven Wishnowsky, Clint Adams
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and to distribute modified versions of this software for any
 * purpose, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * In no event shall Sven Wishnowsky, Clint Adams or the Zsh Development Group
 * be liable to any party for direct, indirect, special, incidental, or
 * consequential damages arising out of the use of this software and its
 * documentation, even if Sven Wishnowsky, Clint Adams and the Zsh
 * Development Group have been advised of the possibility of such damage.
 *
 * Sven Wishnowsky, Clint Adams and the Zsh Development Group specifically
 * disclaim any warranties, including, but not limited to, the implied
 * warranties of merchantability and fitness for a particular purpose.
 * The software provided hereunder is on an "as is" basis, and Sven
 * Wishnowsky, Clint Adams and the Zsh Development Group have no obligation
 * to provide maintenance, support, updates, enhancements, or modifications.
 *
 */
#define USES_TERM_H 1
#include "terminfo.mdh"

#ifdef __SYMBIAN32__
#ifdef __WINSCW__
#pragma warn_unusedarg off
#endif//__WINSCW__
#endif//__SYMBIAN32__

#ifdef __SYMBIAN32__
#include "dummy.h"
#endif //__SYMBIAN32__

#if defined(HAVE_TIGETFLAG) && defined(HAVE_CURSES_H)
# define USE_TERMINFO_MODULE 1
#else
# undef USE_TERMINFO_MODULE
#endif

#include "terminfo.pro"
static char terminfo_nam[] = "terminfo";

/**/
#ifdef USE_TERMINFO_MODULE

/* The following two undefs are needed for Solaris 2.6 */
# ifdef VINTR
#  undef VINTR
# endif
# ifdef offsetof
#  undef offsetof
# endif
#ifndef __SYMBIAN32__
# include <curses.h> 
#endif//__SYMBIAN32__
# ifdef HAVE_TERM_H
#ifndef __SYMBIAN32__
#  include <term.h> //is mandatory as it defines 'boolnames', 'numnames', 'strnames'.
#endif //__SYMBIAN32__
# endif

static Param terminfo_pm;

/* echoti: output a terminfo capability */

/**/
static int
bin_echoti(char *name, char **argv, UNUSED(Options ops), UNUSED(int func))
{
    char *s, *t, **u;
    int arg, num, strarg = 0;
    long pars[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    char *strcap[] = { "pfkey", "pfloc", "pfx", "pln", "pfxl", NULL };

    s = *argv++;
    /* This depends on the termcap stuff in init.c */
    if (termflags & TERM_BAD)
	return 1;
    if ((termflags & TERM_UNKNOWN) && (isset(INTERACTIVE) || !init_term()))
	return 1;
    /* if the specified capability has a numeric value, display it */
    if (((num = tigetnum(s)) != -1) && (num != -2)) {
	printf("%d\n", num);
	return 0;
    }

    switch (tigetflag(s)) {
    case -1:
	break;
    case 0:
	puts("no");
	return 0;
    default:
	puts("yes");
	return 0;
    }

/* get a string-type capability */
    t = (char *)tigetstr(s);
    if (!t || t == (char *)-1 || !*t) {
	/* capability doesn't exist, or (if boolean) is off */
	zwarnnam(name, "no such terminfo capability: %s", s, 0);
	return 1;
    }
    /* check that the number of arguments provided is not too high */
    if (arrlen(argv) > 9) {
        zwarnnam(name, "too many arguments", NULL, 0);
        return 1;
    }

    /* check if we have a capability taking non-integers for parameters */
    for (u = strcap; *u && !strarg; u++)
      strarg = !strcmp(s, *u);

    /* get the arguments */
    for (arg=0; argv[arg]; arg++) {
	if (strarg && arg > 0)
            pars[arg] = (long) argv[arg];
	else
            pars[arg] = atoi(argv[arg]);
    }

    /* output string, through the proper termcap functions */
    if (!arg)
        putp(t);
    else {
        putp(tparm(t, pars[0], pars[1], pars[2], pars[3], pars[4],
	              pars[5], pars[6], pars[7], pars[8]));
    }
    return 0;
}

/**/
#else /* !USE_TERMINFO_MODULE */

#define bin_echoti bin_notavail

/**/
#endif /* !USE_TERMINFO_MODULE */

static struct builtin bintab[] = {
    BUILTIN("echoti", 0, bin_echoti, 1, -1, 0, NULL, NULL),
};

/**/
#ifdef USE_TERMINFO_MODULE

/* Empty dummy function for special hash parameters. */

/**/
static void
shempty(void)
{
}

/* Create a simple special hash parameter. */

/**/
static Param
createtihash()
{
    Param pm;
    HashTable ht;

    unsetparam(terminfo_nam);

    if (!(pm = createparam(terminfo_nam, PM_SPECIAL|PM_HIDE|PM_HIDEVAL|
			   PM_REMOVABLE|PM_HASHED)))
	return NULL;

    pm->level = pm->old ? locallevel : 0;
    pm->gsu.h = &stdhash_gsu;
    pm->u.hash = ht = newhashtable(7, terminfo_nam, NULL);

    ht->hash        = hasher;
    ht->emptytable  = (TableFunc) shempty;
    ht->filltable   = NULL;
    ht->addnode     = (AddNodeFunc) shempty;
    ht->getnode     = ht->getnode2 = getterminfo;
    ht->removenode  = (RemoveNodeFunc) shempty;
    ht->disablenode = NULL;
    ht->enablenode  = NULL;
    ht->freenode    = (FreeNodeFunc) shempty;
    ht->printnode   = printparamnode;
    ht->scantab     = scanterminfo;

    return (terminfo_pm = pm);
}

/**/
static HashNode
getterminfo(UNUSED(HashTable ht), char *name)
{
    int len, num;
    char *tistr;
    Param pm = NULL;

    /* This depends on the termcap stuff in init.c */
    if (termflags & TERM_BAD)
	return NULL;
    if ((termflags & TERM_UNKNOWN) && (isset(INTERACTIVE) || !init_term()))
	return NULL;

    unmetafy(name, &len);

    pm = (Param) hcalloc(sizeof(struct param));
    pm->nam = dupstring(name);
    pm->flags = PM_READONLY;

    if (((num = tigetnum(name)) != -1) && (num != -2)) {
	pm->u.val = num;
	pm->flags |= PM_INTEGER;
	pm->gsu.i = &nullsetinteger_gsu;
    }
    else if ((num = tigetflag(name)) != -1) {
	pm->u.str = num ? dupstring("yes") : dupstring("no");
	pm->flags |= PM_SCALAR;
	pm->gsu.s = &nullsetscalar_gsu;
    }
    else if ((tistr = (char *)tigetstr(name)) != NULL && tistr != (char *)-1)
    {
	pm->u.str = dupstring(tistr);
	pm->flags |= PM_SCALAR;
	pm->gsu.s = &nullsetscalar_gsu;
    }
    else
    {
	/* zwarn("no such capability: %s", name, 0); */
	pm->u.str = dupstring("");
	pm->flags |= PM_UNSET;
	pm->gsu.s = &nullsetscalar_gsu;
    }
    return (HashNode) pm;
}

/**/
static void
scanterminfo(UNUSED(HashTable ht), ScanFunc func, int flags)
{
    Param pm = NULL;
    int num;
    char **capname, *tistr;

#ifndef HAVE_BOOLNAMES
    static char *boolnames[] = {
	"bw", "am", "bce", "ccc", "xhp", "xhpa", "cpix", "crxm", "xt", "xenl",
	"eo", "gn", "hc", "chts", "km", "daisy", "hs", "hls", "in", "lpix",
	"da", "db", "mir", "msgr", "nxon", "xsb", "npc", "ndscr", "nrrmc",
	"os", "mc5i", "xvpa", "sam", "eslok", "hz", "ul", "xon", NULL};
#endif
    
#ifndef HAVE_NUMNAMES
    static char *numnames[] = {
	"cols", "it", "lh", "lw", "lines", "lm", "xmc", "ma", "colors",
	"pairs", "wnum", "ncv", "nlab", "pb", "vt", "wsl", "bitwin",
	"bitype", "bufsz", "btns", "spinh", "spinv", "maddr", "mjump",
	"mcs", "mls", "npins", "orc", "orhi", "orl", "orvi", "cps", "widcs",
	NULL};
#endif

#ifndef HAVE_STRNAMES
    static char *strnames[] = {
	"acsc", "cbt", "bel", "cr", "cpi", "lpi", "chr", "cvr", "csr", "rmp",
	"tbc", "mgc", "clear", "el1", "el", "ed", "hpa", "cmdch", "cwin",
	"cup", "cud1", "home", "civis", "cub1", "mrcup", "cnorm", "cuf1",
	"ll", "cuu1", "cvvis", "defc", "dch1", "dl1", "dial", "dsl", "dclk",
	"hd", "enacs", "smacs", "smam", "blink", "bold", "smcup", "smdc",
	"dim", "swidm", "sdrfq", "smir", "sitm", "slm", "smicm", "snlq",
	"snrmq", "prot", "rev", "invis", "sshm", "smso", "ssubm", "ssupm",
	"smul", "sum", "smxon", "ech", "rmacs", "rmam", "sgr0", "rmcup",
	"rmdc", "rwidm", "rmir", "ritm", "rlm", "rmicm", "rshm", "rmso",
	"rsubm", "rsupm", "rmul", "rum", "rmxon", "pause", "hook", "flash",
	"ff", "fsl", "wingo", "hup", "is1", "is2", "is3", "if", "iprog",
	"initc", "initp", "ich1", "il1", "ip", "ka1", "ka3", "kb2", "kbs",
	"kbeg", "kcbt", "kc1", "kc3", "kcan", "ktbc", "kclr", "kclo", "kcmd",
	"kcpy", "kcrt", "kctab", "kdch1", "kdl1", "kcud1", "krmir", "kend",
	"kent", "kel", "ked", "kext", "kf0", "kf1", "kf10", "kf11", "kf12",
	"kf13", "kf14", "kf15", "kf16", "kf17", "kf18", "kf19", "kf2",
	"kf20", "kf21", "kf22", "kf23", "kf24", "kf25", "kf26", "kf27",
	"kf28", "kf29", "kf3", "kf30", "kf31", "kf32", "kf33", "kf34",
	"kf35", "kf36", "kf37", "kf38", "kf39", "kf4", "kf40", "kf41",
	"kf42", "kf43", "kf44", "kf45", "kf46", "kf47", "kf48", "kf49",
	"kf5", "kf50", "kf51", "kf52", "kf53", "kf54", "kf55", "kf56",
	"kf57", "kf58", "kf59", "kf6", "kf60", "kf61", "kf62", "kf63",
	"kf7", "kf8", "kf9", "kfnd", "khlp", "khome", "kich1", "kil1",
	"kcub1", "kll", "kmrk", "kmsg", "kmov", "knxt", "knp", "kopn",
	"kopt", "kpp", "kprv", "kprt", "krdo", "kref", "krfr", "krpl",
	"krst", "kres", "kcuf1", "ksav", "kBEG", "kCAN", "kCMD", "kCPY",
	"kCRT", "kDC", "kDL", "kslt", "kEND", "kEOL", "kEXT", "kind",
	"kFND", "kHLP", "kHOM", "kIC", "kLFT", "kMSG", "kMOV", "kNXT",
	"kOPT", "kPRV", "kPRT", "kri", "kRDO", "kRPL", "kRIT", "kRES",
	"kSAV", "kSPD", "khts", "kUND", "kspd", "kund", "kcuu1", "rmkx",
	"smkx", "lf0", "lf1", "lf10", "lf2", "lf3", "lf4", "lf5", "lf6",
	"lf7", "lf8", "lf9", "fln", "rmln", "smln", "rmm", "smm", "mhpa",
	"mcud1", "mcub1", "mcuf1", "mvpa", "mcuu1", "nel", "porder", "oc",
	"op", "pad", "dch", "dl", "cud", "mcud", "ich", "indn", "il", "cub",
	"mcub", "cuf", "mcuf", "rin", "cuu", "mcuu", "pfkey", "pfloc",
	"pfx", "pln", "mc0", "mc5p", "mc4", "mc5", "pulse", "qdial",
	"rmclk", "rep", "rfi", "rs1", "rs2", "rs3", "rf", "rc", "vpa",
	"sc", "ind", "ri", "scs", "sgr", "setb", "smgb", "smgbp", "sclk",
	"scp", "setf", "smgl", "smglp", "smgr", "smgrp", "hts", "smgt",
	"smgtp", "wind", "sbim", "scsd", "rbim", "rcsd", "subcs",
	"supcs", "ht", "docr", "tsl", "tone", "uc", "hu", "u0", "u1",
	"u2", "u3", "u4", "u5", "u6", "u7", "u8", "u9", "wait", "xoffc",
	"xonc", "zerom", "scesa", "bicr", "binel", "birep", "csnm",
	"csin", "colornm", "defbi", "devt", "dispc", "endbi", "smpch",
	"smsc", "rmpch", "rmsc", "getm", "kmous", "minfo", "pctrm",
	"pfxl", "reqmp", "scesc", "s0ds", "s1ds", "s2ds", "s3ds",
	"setab", "setaf", "setcolor", "smglr", "slines", "smgtb",
	"ehhlm", "elhlm", "elohlm", "erhlm", "ethlm", "evhlm", "sgr1",
	"slength", NULL};
#endif

    pm = (Param) hcalloc(sizeof(struct param));

    pm->flags = PM_READONLY | PM_SCALAR;
    pm->gsu.s = &nullsetscalar_gsu;

    for (capname = (char **)boolnames; *capname; capname++) {
	if ((num = tigetflag(*capname)) != -1) {
	    pm->u.str = num ? dupstring("yes") : dupstring("no");
	    pm->nam = dupstring(*capname);
	    func((HashNode) pm, flags);
	}
    }

    pm->flags = PM_READONLY | PM_INTEGER;
    pm->gsu.i = &nullsetinteger_gsu;

    for (capname = (char **)numnames; *capname; capname++) {
	if (((num = tigetnum(*capname)) != -1) && (num != -2)) {
	    pm->u.val = num;
	    pm->nam = dupstring(*capname);
	    func((HashNode) pm, flags);
	}
    }

    pm->flags = PM_READONLY | PM_SCALAR;
    pm->gsu.s = &nullsetscalar_gsu;

    for (capname = (char **)strnames; *capname; capname++) {
	if ((tistr = (char *)tigetstr(*capname)) != NULL &&
	    tistr != (char *)-1) {
	    pm->u.str = dupstring(tistr);
	    pm->nam = dupstring(*capname);
	    func((HashNode) pm, flags);
	}
    }
}

/**/
#endif /* USE_TERMINFO_MODULE */

/**/
int
setup_(UNUSED(Module m))
{
    return 0;
}

/**/
int
boot_(Module m)
{
#ifdef USE_TERMINFO_MODULE
# ifdef HAVE_SETUPTERM
    int errret;

    if (setupterm((char *)0, 1, &errret) == ERR)
	return 1;
# endif

    if (!createtihash())
    	return 1;
#else
    unsetparam(terminfo_nam);
#endif
    return  !addbuiltins(m->nam, bintab, sizeof(bintab)/sizeof(*bintab));
}

/**/
int
cleanup_(Module m)
{
#ifdef USE_TERMINFO_MODULE
    Param pm;

    if ((pm = (Param) paramtab->getnode(paramtab, terminfo_nam)) &&
	pm == terminfo_pm) {
	pm->flags &= ~PM_READONLY;
	unsetparam_pm(pm, 0, 1);
    }
#endif
    deletebuiltins(m->nam, bintab, sizeof(bintab)/sizeof(*bintab));
    return 0;
}

/**/
int
finish_(UNUSED(Module m))
{
    return 0;
}
