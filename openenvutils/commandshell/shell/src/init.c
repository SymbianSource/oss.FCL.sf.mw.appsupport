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
 * Copyright (c) 1992-1997 Paul Falstad
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and to distribute modified versions of this software for any
 * purpose, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * In no event shall Paul Falstad or the Zsh Development Group be liable
 * to any party for direct, indirect, special, incidental, or consequential
 * damages arising out of the use of this software and its documentation,
 * even if Paul Falstad and the Zsh Development Group have been advised of
 * the possibility of such damage.
 *
 * Paul Falstad and the Zsh Development Group specifically disclaim any
 * warranties, including, but not limited to, the implied warranties of
 * merchantability and fitness for a particular purpose.  The software
 * provided hereunder is on an "as is" basis, and Paul Falstad and the
 * Zsh Development Group have no obligation to provide maintenance,
 * support, updates, enhancements, or modifications.
 *
 */
#include "zsh.mdh"

#include "zshpaths.h"
#include "zshxmods.h"

#include "init.pro"

#include "version.h"

#ifdef __SYMBIAN32__
#ifdef __WINSCW__
#pragma warn_unusedarg off
#pragma warn_possunwant off
#endif//__WINSCW__
#endif//__SYMBIAN32__

#ifdef __SYMBIAN32__
#include "dummy.h"
char getSystemDriveChar(void);
#endif //__SYMBIAN32__

/**/
int noexitct = 0;

int pipeUsed;
/* buffer for $_ and its length */

/**/
char *underscore;

/**/
int underscorelen, underscoreused;

/* what level of sourcing we are at */
 
/**/
int sourcelevel;

/* the shell tty fd */

/**/
mod_export int SHTTY;

/* the FILE attached to the shell tty */

/**/
mod_export FILE *shout;

/* termcap strings */
 
/**/
mod_export char *tcstr[TC_COUNT];

/* lengths of each termcap string */
 
/**/
mod_export int tclen[TC_COUNT];

/* Values of the li, co and am entries */

/**/
int tclines, tccolumns;
/**/
mod_export int hasam;

/* Pointer to read-key function from zle */

/**/
mod_export int (*getkeyptr) _((int));

/* SIGCHLD mask */

/**/
mod_export sigset_t sigchld_mask;

/**/
mod_export struct hookdef zshhooks[] = {
    HOOKDEF("exit", NULL, HOOKF_ALL),
    HOOKDEF("before_trap", NULL, HOOKF_ALL),
    HOOKDEF("after_trap", NULL, HOOKF_ALL),
};

/* keep executing lists until EOF found */

/**/
void
loop(int toplevel, int justonce)
{
    Eprog prog;

    pushheap();
    for (;;) {
	freeheap();
	if (stophist == 3)	/* re-entry via preprompt() */
	    hend(NULL);
	hbegin(1);		/* init history mech        */
	if (isset(SHINSTDIN)) {
	    setblock_stdin();
	    if (interact) {
	        int hstop = stophist;
		stophist = 3;
		preprompt();
		if (stophist != 3)
		    hbegin(1);
		else
		    stophist = hstop;
		errflag = 0;
	    }
	}
	intr();			/* interrupts on            */
	lexinit();              /* initialize lexical state */
	if (!(prog = parse_event())) {	/* if we couldn't parse a list */
	    hend(NULL);
	    if ((tok == ENDINPUT && !errflag) ||
		(tok == LEXERR && (!isset(SHINSTDIN) || !toplevel)) ||
		justonce)
		break;
	    if (tok == LEXERR && !lastval)
		lastval = 1;
	    continue;
	}
	if (hend(prog)) {
	    int toksav = tok;
	    Eprog preprog;

	    if (toplevel && (preprog = getshfunc("preexec")) != &dummy_eprog) {
		LinkList args;
		int osc = sfcontext;
		char *cmdstr;

		args = znewlinklist();
		zaddlinknode(args, "preexec");
		/* If curline got dumped from the history, we don't know
		 * what the user typed. */
		if (hist_ring && curline.histnum == curhist)
		    zaddlinknode(args, hist_ring->text);
		else
		    zaddlinknode(args, "");
		zaddlinknode(args, getjobtext(prog, NULL));
		zaddlinknode(args, cmdstr = getpermtext(prog, NULL));

		sfcontext = SFC_HOOK;
		doshfunc("preexec", preprog, args, 0, 1);
		sfcontext = osc;
		zsfree(cmdstr);
		freelinklist(args, (FreeFunc) NULL);
		errflag = 0;
	    }
	    if (stopmsg)	/* unset 'you have stopped jobs' flag */
		stopmsg--;
	    execode(prog, 0, 0);
	    tok = toksav;
	    if (toplevel)
		noexitct = 0;
	}
	if (ferror(stderr)) {
	    zerr("write error", NULL, 0);
	    clearerr(stderr);
	}
	if (subsh)		/* how'd we get this far in a subshell? */
	    exit(lastval);
	if (((!interact || sourcelevel) && errflag) || retflag)
	    break;
	if (trapreturn) {
	    lastval = trapreturn;
	    trapreturn = 0;
	}
	if (isset(SINGLECOMMAND) && toplevel) {
	    if (sigtrapped[SIGEXIT])
		dotrap(SIGEXIT);
	    exit(lastval);
	}
	if (justonce)
	    break;
    }
    popheap();
}

static char *cmd;
static int restricted;

/**/
void
parseargs(char **argv)
{
    int optionbreak = 0;
    char **x;
    int action, optno;
    LinkList paramlist;

    argzero = *argv++;
    SHIN = 0;

    /* There's a bit of trickery with opts[INTERACTIVE] here.  It starts *
     * at a value of 2 (instead of 1) or 0.  If it is explicitly set on  *
     * the command line, it goes to 1 or 0.  If input is coming from     *
     * somewhere that normally makes the shell non-interactive, we do    *
     * "opts[INTERACTIVE] &= 1", so that only a *default* on state will  *
     * be changed.  At the end of the function, a value of 2 gets        *     
     * changed to 1.                                                     */
     
#ifdef __SYMBIAN32__     
    opts[INTERACTIVE] = 2; /*consider pipe end also as interative*/
#else
    opts[INTERACTIVE] = isatty(0) ? 2 : 0;
#endif    

    opts[SHINSTDIN] = 0;
    opts[SINGLECOMMAND] = 0;

    /* loop through command line options (begins with "-" or "+") */
    while (!optionbreak && *argv && (**argv == '-' || **argv == '+')) {
	char *args = *argv;
	action = (**argv == '-');
	if (!argv[0][1])
	    *argv = "--";
	while (*++*argv) {
	    if (**argv == '-') {
		if(!argv[0][1]) {
		    /* The pseudo-option `--' signifies the end of options. */
		    argv++;
		    goto doneoptions;
		}
		if(*argv != args+1 || **argv != '-')
		    goto badoptionstring;
		/* GNU-style long options */
		++*argv;
		if (!strcmp(*argv, "version")) {
		    printf("zsh %s (%s-%s-%s)\n",
			    ZSH_VERSION, MACHTYPE, VENDOR, OSTYPE);
		    exit(0);
		}
		if (!strcmp(*argv, "help")) {
		    printhelp();
		    exit(0);
		}
		/* `-' characters are allowed in long options */
		for(args = *argv; *args; args++)
		    if(*args == '-')
			*args = '_';
		goto longoptions;
	    }

	    if (unset(SHOPTIONLETTERS) && **argv == 'b') {
		/* -b ends options at the end of this argument */
		optionbreak = 1;
	    } else if (**argv == 'c') {
		/* -c command */
		cmd = *argv;
		opts[INTERACTIVE] &= 1;
		opts[SHINSTDIN] = 0;
		scriptname = ztrdup("zsh");
	    } else if (**argv == 'o') {
		if (!*++*argv)
		    argv++;
		if (!*argv) {
		    zerr("string expected after -o", NULL, 0);
		    exit(1);
		}
	    longoptions:
		if (!(optno = optlookup(*argv))) {
		    zerr("no such option: %s", *argv, 0);
		    exit(1);
		} else if (optno == RESTRICTED)
		    restricted = action;
		else
		    dosetopt(optno, action, 1);
              break;
	    } else if (isspace(STOUC(**argv))) {
		/* zsh's typtab not yet set, have to use ctype */
		while (*++*argv)
		    if (!isspace(STOUC(**argv))) {
		    badoptionstring:
			zerr("bad option string: `%s'", args, 0);
			exit(1);
		    }
		break;
	    } else {
	    	if (!(optno = optlookupc(**argv))) {
		    zerr("bad option: -%c", NULL, **argv);
		    exit(1);
		} else if (optno == RESTRICTED)
		    restricted = action;
		else
		    dosetopt(optno, action, 1);
	    }
	}
	argv++;
    }
    doneoptions:
    paramlist = znewlinklist();
    if (cmd) {
	if (!*argv) {
	    zerr("string expected after -%s", cmd, 0);
	    exit(1);
	}
	cmd = *argv++;
    }
    if (*argv) {
	if (unset(SHINSTDIN)) {
	    argzero = *argv;
	    if (!cmd)
		SHIN = movefd(open(unmeta(argzero), O_RDONLY | O_NOCTTY));
	    if (SHIN == -1) {
		zerr("can't open input file: %s", argzero, 0);
		exit(1);
	    }
	    opts[INTERACTIVE] &= 1;
	    argv++;
	}
	while (*argv)
	    zaddlinknode(paramlist, ztrdup(*argv++));
    } else
	opts[SHINSTDIN] = 1;
    if(isset(SINGLECOMMAND))
	opts[INTERACTIVE] &= 1;
    opts[INTERACTIVE] = !!opts[INTERACTIVE];
    pparams = x = (char **) zshcalloc((countlinknodes(paramlist) + 1) * sizeof(char *));

    while ((*x++ = (char *)getlinknode(paramlist)));
    free(paramlist);
    argzero = ztrdup(argzero);
}

/**/
static void
printhelp(void)
{
    printf("Usage: %s [<options>] [<argument> ...]\n", argzero);
    printf("\nSpecial options:\n");
    printf("  --help     show this message, then exit\n");
    printf("  --version  show zsh version number, then exit\n");
    if(unset(SHOPTIONLETTERS))
	printf("  -b         end option processing, like --\n");
    printf("  -c         take first argument as a command to execute\n");
    printf("  -o OPTION  set an option by name (see below)\n");
    printf("\nNormal options are named.  An option may be turned on by\n");
    printf("`-o OPTION', `--OPTION', `+o no_OPTION' or `+-no-OPTION'.  An\n");
    printf("option may be turned off by `-o no_OPTION', `--no-OPTION',\n");
    printf("`+o OPTION' or `+-OPTION'.  Options are listed below only in\n");
    printf("`--OPTION' or `--no-OPTION' form.\n");
    printoptionlist();
}

/**/
mod_export void
init_io(void)
{
    static char outbuf[BUFSIZ], errbuf[BUFSIZ];

#ifdef RSH_BUG_WORKAROUND
    int i;
#endif

/* stdout, stderr fully buffered */
#ifdef _IOFBF
    setvbuf(stdout, outbuf, _IOFBF, BUFSIZ);
    setvbuf(stderr, errbuf, _IOFBF, BUFSIZ);
#else
    setbuffer(stdout, outbuf, BUFSIZ);
    setbuffer(stderr, errbuf, BUFSIZ);
#endif

/* This works around a bug in some versions of in.rshd. *
 * Currently this is not defined by default.            */
#ifdef RSH_BUG_WORKAROUND
    if (cmd) {
	for (i = 3; i < 10; i++)
	    close(i);
    }
#endif

    if (shout) {
	/*
	 * Check if shout was set to stderr, if so don't close it.
	 * We do this if we are interactive but don't have a
	 * terminal.
	 */
	if (shout != stderr)
	    fclose(shout);
	shout = 0;
    }
    if (SHTTY != -1) {
	zclose(SHTTY);
	SHTTY = -1;
    }

    /* Send xtrace output to stderr -- see execcmd() */
    xtrerr = stderr;

    /* Make sure the tty is opened read/write. */
    if (isatty(0)) {
	zsfree(ttystrname);
	if ((ttystrname = ztrdup((char*)ttyname(0)))) {
	    SHTTY = movefd(open(ttystrname, O_RDWR | O_NOCTTY));
#ifdef TIOCNXCL
	    /*
	     * See if the terminal claims to be busy.  If so, and fd 0
	     * is a terminal, try and set non-exclusive use for that.
	     * This is something to do with Solaris over-cleverness.
	     */
	    if (SHTTY == -1 && errno == EBUSY)
		ioctl(0, TIOCNXCL, 0);
#endif
	}
	/*
	 * xterm, rxvt and probably all terminal emulators except
	 * dtterm on Solaris 2.6 & 7 have a bug. Applications are
	 * unable to open /dev/tty or /dev/pts/<terminal number here>
	 * because something in Sun's STREAMS modules doesn't like
	 * it. The open() call fails with EBUSY which is not even
	 * listed as a possibility in the open(2) man page.  So we'll
	 * try to outsmart The Company.  -- <dave@srce.hr>
	 *
	 * Presumably there's no harm trying this on any OS, given that
	 * isatty(0) worked but opening the tty didn't.  Possibly we won't
	 * get the tty read/write, but it's the best we can do -- pws
	 *
	 * Try both stdin and stdout before trying /dev/tty. -- Bart
	 */
#if defined(HAVE_FCNTL_H) && defined(F_GETFL)
#define rdwrtty(fd)	((fcntl(fd, F_GETFL, 0) & O_RDWR) == O_RDWR)
#else
#define rdwrtty(fd)	1
#endif
	if (SHTTY == -1 && rdwrtty(0)) {
	    SHTTY = movefd(dup(0));
	}
    }
    if (SHTTY == -1 && isatty(1) && rdwrtty(1) &&
	(SHTTY = movefd(dup(1))) != -1) {
	zsfree(ttystrname);
	ttystrname = ztrdup((char*)ttyname((1)));
    }
    if (SHTTY == -1 &&
	(SHTTY = movefd(open("/dev/tty", O_RDWR | O_NOCTTY))) != -1) {
	zsfree(ttystrname);
	ttystrname = ztrdup((char*)ttyname(SHTTY));
    }
    if (SHTTY == -1) {
	zsfree(ttystrname);
	ttystrname = ztrdup("");
    } else if (!ttystrname) {
	ttystrname = ztrdup("/dev/tty");
    }

    /* We will only use zle if shell is interactive, *
     * SHTTY != -1, and shout != 0                   */
    if (interact) {
	init_shout();
	if(!SHTTY || !shout)
	    opts[USEZLE] = 0;
    } else
	opts[USEZLE] = 0;

#ifdef JOB_CONTROL
    /* If interactive, make sure the shell is in the foreground and is the
     * process group leader.
     */
    mypid = (zlong)getpid();
    if (opts[MONITOR] && interact && (SHTTY != -1)) {
	origpgrp = GETPGRP();
        acquire_pgrp(); /* might also clear opts[MONITOR] */
    } else
	opts[MONITOR] = 0;
#else
    opts[MONITOR] = 0;
#endif
}

/**/
mod_export void
init_shout(void)
{
    static char shoutbuf[BUFSIZ];
#if defined(JOB_CONTROL) && defined(TIOCSETD) && defined(NTTYDISC)
    int ldisc;
#endif

    if (SHTTY == -1)
    {
	/* Since we're interative, it's nice to have somewhere to write. */
	shout = stderr;
	return;
    }

#if defined(JOB_CONTROL) && defined(TIOCSETD) && defined(NTTYDISC)
    ldisc = NTTYDISC;
    ioctl(SHTTY, TIOCSETD, (char *)&ldisc);
#endif

    /* Associate terminal file descriptor with a FILE pointer */
    shout = fdopen(SHTTY, "w");
#ifdef _IOFBF
    setvbuf(shout, shoutbuf, _IOFBF, BUFSIZ);
#endif
  
    gettyinfo(&shttyinfo);	/* get tty state */
#if defined(__sgi)
    if (shttyinfo.tio.c_cc[VSWTCH] <= 0)	/* hack for irises */
	shttyinfo.tio.c_cc[VSWTCH] = CSWTCH;
#endif
}

/* names of the termcap strings we want */

static char *tccapnams[TC_COUNT] = {
    "cl", "le", "LE", "nd", "RI", "up", "UP", "do",
    "DO", "dc", "DC", "ic", "IC", "cd", "ce", "al", "dl", "ta",
    "md", "so", "us", "me", "se", "ue", "ch",
    "ku", "kd", "kl", "kr"
};

/* Initialise termcap */

/**/
mod_export int
init_term(void)
{
#ifndef TGETENT_ACCEPTS_NULL
    static char termbuf[2048];	/* the termcap buffer */
#endif

    if (!*term) {
	termflags |= TERM_UNKNOWN;
	return 0;
    }

    /* unset zle if using zsh under emacs */
    if (!strcmp(term, "emacs"))
	opts[USEZLE] = 0;

#ifdef TGETENT_ACCEPTS_NULL
    /* If possible, we let tgetent allocate its own termcap buffer */
    if (tgetent(NULL, term) != TGETENT_SUCCESS)
#else
    if (tgetent(termbuf, term) != TGETENT_SUCCESS)
#endif
    {
	if (isset(INTERACTIVE))
	    zerr("can't find terminal definition for %s", term, 0);
	errflag = 0;
	termflags |= TERM_BAD;
	return 0;
    } else {
	char tbuf[1024], *pp;
	int t0;

	termflags &= ~TERM_BAD;
	termflags &= ~TERM_UNKNOWN;
	for (t0 = 0; t0 != TC_COUNT; t0++) {
	    pp = tbuf;
	    zsfree(tcstr[t0]);
	/* AIX tgetstr() ignores second argument */
	    if (!(pp = tgetstr(tccapnams[t0], &pp)))
		tcstr[t0] = NULL, tclen[t0] = 0;
	    else {
		tclen[t0] = strlen(pp);
		tcstr[t0] = (char *) zalloc(tclen[t0] + 1);
		memcpy(tcstr[t0], pp, tclen[t0] + 1);
	    }
	}

	/* check whether terminal has automargin (wraparound) capability */
	hasam = tgetflag("am");

	tclines = tgetnum("li");
	tccolumns = tgetnum("co");

	/* if there's no termcap entry for cursor up, use single line mode: *
	 * this is flagged by termflags which is examined in zle_refresh.c  *
	 */
	if (tccan(TCUP))
	    termflags &= ~TERM_NOUP;
	else {
	    tcstr[TCUP] = NULL;
	    termflags |= TERM_NOUP;
	}

	/* if there's no termcap entry for cursor left, use \b. */
	if (!tccan(TCLEFT)) {
	    tcstr[TCLEFT] = ztrdup("\b");
	    tclen[TCLEFT] = 1;
	}

	/* if the termcap entry for down is \n, don't use it. */
	if (tccan(TCDOWN) && tcstr[TCDOWN][0] == '\n') {
	    tclen[TCDOWN] = 0;
	    zsfree(tcstr[TCDOWN]);
	    tcstr[TCDOWN] = NULL;
	}

	/* if there's no termcap entry for clear, use ^L. */
	if (!tccan(TCCLEARSCREEN)) {
	    tcstr[TCCLEARSCREEN] = ztrdup("\14");
	    tclen[TCCLEARSCREEN] = 1;
	}
    }
    return 1;
}

/* Initialize lots of global variables and hash tables */

/**/
void
setupvals(void)
{
#ifdef HAVE_GETPWUID
    struct passwd *pswd;
#endif
    struct timezone dummy_tz;
    char *ptr;
    int i, j;
#if defined(SITEFPATH_DIR) || defined(FPATH_DIR)
    char **fpathptr;
# if defined(FPATH_DIR) && defined(FPATH_SUBDIRS)
    char *fpath_subdirs[] = FPATH_SUBDIRS;
# endif
# ifdef SITEFPATH_DIR
    int fpathlen = 1;
# else
    int fpathlen = 0;
# endif
#endif
    int close_fds[10], tmppipe[2];

    /*
     * Workaround a problem with NIS (in one guise or another) which
     * grabs file descriptors and keeps them for future reference.
     * We don't want these to be in the range where the user can
     * open fd's, i.e. 0 to 9 inclusive.  So we make sure all
     * fd's in that range are in use.
     */
    memset(close_fds, 0, 10*sizeof(int));
    if (pipe(tmppipe) == 0) {
	/*
	 * Strategy:  Make sure we have at least fd 0 open (hence
	 * the pipe).  From then on, keep dup'ing until we are
	 * up to 9.  If we go over the top, close immediately, else
	 * mark for later closure.
	 */
	i = -1;			/* max fd we have checked */
	while (i < 9) {
	    /* j is current fd */
	    if (i < tmppipe[0])
		j = tmppipe[0];
	    else if (i < tmppipe[1])
		j = tmppipe[1];
	    else {
		j = dup(0);
		if (j == -1)
		    break;
	    }
	    if (j < 10)
		close_fds[j] = 1;
	    else
		close(j);
	    if (i < j)
		i = j;
	}
	if (i < tmppipe[0])
	    close(tmppipe[0]);
	if (i < tmppipe[1])
	    close(tmppipe[1]);
    }

    addhookdefs(argzero, zshhooks, sizeof(zshhooks)/sizeof(*zshhooks));

    init_eprog();

    zero_mnumber.type = MN_INTEGER;
    zero_mnumber.u.l = 0;

    getkeyptr = NULL;

    lineno = 1;
    noeval = 0;
    curhist = 0;
    histsiz = DEFAULT_HISTSIZE;
    inithist();

    cmdstack = (unsigned char *) zalloc(CMDSTACKSZ);
    cmdsp = 0;

    bangchar = '!';
    hashchar = '#';
    hatchar = '^';
    termflags = TERM_UNKNOWN;
    curjob = prevjob = coprocin = coprocout = -1;
    gettimeofday(&shtimer, &dummy_tz);	/* init $SECONDS */
    srand((unsigned int)(shtimer.tv_sec + shtimer.tv_usec)); /* seed $RANDOM */

    /* Set default path */
#ifndef __SYMBIAN32__        
    path    = (char **) zalloc(sizeof(*path) * 5);
    path[0] = ztrdup("/bin");
    path[1] = ztrdup("/usr/bin");
    path[2] = ztrdup("/usr/ucb");
    path[3] = ztrdup("/usr/local/bin");
    path[4] = NULL;
#else
    path    = (char **) zalloc(sizeof(*path) * 1);
    path[0] = NULL;
#endif
    cdpath   = mkarray(NULL);
    manpath  = mkarray(NULL);
    fignore  = mkarray(NULL);

#if defined(SITEFPATH_DIR) || defined(FPATH_DIR)
# ifdef FPATH_DIR
#  ifdef FPATH_SUBDIRS
    fpathlen += sizeof(fpath_subdirs)/sizeof(char *);
#  else
    fpathlen++;
#  endif
# endif
    fpath = fpathptr = (char **)zalloc((fpathlen+1)*sizeof(char *));
# ifdef SITEFPATH_DIR
    *fpathptr++ = ztrdup(SITEFPATH_DIR);
    fpathlen--;
# endif
# ifdef FPATH_DIR
#  ifdef FPATH_SUBDIRS
    for (j = 0; j < fpathlen; j++)
	*fpathptr++ = tricat(FPATH_DIR, "/", fpath_subdirs[j]);
#  else
    *fpathptr++ = ztrdup(FPATH_DIR);
#  endif
# endif
    *fpathptr = NULL;
#else
    fpath    = mkarray(NULL);
#endif

    mailpath = mkarray(NULL);
    watch    = mkarray(NULL);
    psvar    = mkarray(NULL);
    module_path = mkarray(ztrdup(MODULE_DIR));
    modules = znewlinklist();
    linkedmodules = znewlinklist();

    /* Set default prompts */
    if(unset(INTERACTIVE)) {
	prompt = ztrdup("");
	prompt2 = ztrdup("");
    } else if (emulation == EMULATE_KSH || emulation == EMULATE_SH) {
	prompt  = ztrdup(privasserted() ? "# " : "$ ");
	prompt2 = ztrdup("> ");
    } else {
	prompt  = ztrdup("%m%# ");
	prompt2 = ztrdup("%_> ");
    }
    prompt3 = ztrdup("?# ");
    prompt4 = (emulation == EMULATE_KSH || emulation == EMULATE_SH)
	? ztrdup("+ ") : ztrdup("+%N:%i> ");
    sprompt = ztrdup("zsh: correct '%R' to '%r' [nyae]? ");

    ifs         = ztrdup(DEFAULT_IFS);
    wordchars   = ztrdup(DEFAULT_WORDCHARS);
    postedit    = ztrdup("");
    underscore  = (char *) zalloc(underscorelen = 32);
    underscoreused = 1;
    *underscore = '\0';

    zoptarg = ztrdup("");
    zoptind = 1;

    ppid  = (zlong) getppid();
    mypid = (zlong) getpid();
    term  = ztrdup("");

    nullcmd     = ztrdup("cat");
    readnullcmd = ztrdup("more");

    /* We cache the uid so we know when to *
     * recheck the info for `USERNAME'     */
    cached_uid = getuid();

    /* Get password entry and set info for `HOME' and `USERNAME' */
#ifdef HAVE_GETPWUID
    if ((pswd = getpwuid(cached_uid))) {
#ifdef __SYMBIAN32__ 
	char sysDrv[2 +1];
	sprintf(sysDrv, "%c:", getSystemDriveChar());
	home = metafy(sysDrv, -1, META_DUP);
	
	//change the pwd to the system drive.
	chdir(sysDrv);
#else
	home = metafy(pswd->pw_dir, -1, META_DUP);
#endif	
	cached_username = ztrdup(pswd->pw_name);
    } else
#endif /* HAVE_GETPWUID */
	   {
	home = ztrdup("/");
	cached_username = ztrdup("");
    }

    /* Try a cheap test to see if we can *
     * initialize `PWD' from `HOME'      */
    if (ispwd(home))
	pwd = ztrdup(home);
    else if ((ptr = zgetenv("PWD")) && (strlen(ptr) < PATH_MAX) &&
	     (ptr = metafy(ptr, -1, META_STATIC), ispwd(ptr)))
	pwd = ztrdup(ptr);
    else
	pwd = metafy(zgetcwd(), -1, META_DUP);

    oldpwd = ztrdup(pwd);  /* initialize `OLDPWD' = `PWD' */

    inittyptab();     /* initialize the ztypes table */
    initlextabs();    /* initialize lexing tables    */

    createreswdtable();     /* create hash table for reserved words    */
    createaliastables();    /* create hash tables for aliases           */
    createcmdnamtable();    /* create hash table for external commands */
    createshfunctable();    /* create hash table for shell functions   */
    createbuiltintable();   /* create hash table for builtin commands  */
    createnameddirtable();  /* create hash table for named directories */
    createparamtable();     /* create parameter hash table             */

    condtab = NULL;
    wrappers = NULL;

#ifdef TIOCGWINSZ
    adjustwinsize(0);
#else
    /* columns and lines are normally zero, unless something different *
     * was inhereted from the environment.  If either of them are zero *
     * the setiparam calls below set them to the defaults from termcap */
    setiparam("COLUMNS", columns);
    setiparam("LINES", lines);
#endif

#ifdef HAVE_GETRLIMIT
    for (i = 0; i != RLIM_NLIMITS; i++) {
	getrlimit(i, current_limits + i);
	limits[i] = current_limits[i];
    }
#endif

    breaks = loops = 0;
    lastmailcheck = time(NULL);
    locallevel = sourcelevel = 0;
    sfcontext = SFC_NONE;
    trapreturn = 0;
    noerrexit = -1;
    nohistsave = 1;
    dirstack = znewlinklist();
    bufstack = znewlinklist();
    prepromptfns = znewlinklist();
    hsubl = hsubr = NULL;
    lastpid = 0;
    bshin = SHIN ? fdopen(SHIN, "r") : stdin;
    if (isset(SHINSTDIN) && !SHIN && unset(INTERACTIVE)) {
#ifdef _IONBF
	setvbuf(stdin, NULL, _IONBF, 0);
#else
	setlinebuf(stdin);
#endif
    }

    get_usage();

    /* Close the file descriptors we opened to block off 0 to 9 */
    for (i = 0; i < 10; i++)
	if (close_fds[i])
	    close(i);
}

/* Initialize signal handling */

/**/
void
init_signals(void)
{
#ifndef __SYMBIAN32__
    if (interact) {
	int i;
	signal_setmask(signal_mask(0));
	for (i=0; i<NSIG; ++i)
	    signal_default(i);
    }
    sigchld_mask = signal_mask(SIGCHLD);

    intr();

#ifndef QDEBUG
    signal_ignore(SIGQUIT);
#endif

    if (signal_ignore(SIGHUP) == SIG_IGN)
	opts[HUP] = 0;
    else
	install_handler(SIGHUP);
    install_handler(SIGCHLD);
#ifdef SIGWINCH
    install_handler(SIGWINCH);
#endif
    if (interact) {
	install_handler(SIGALRM);
	signal_ignore(SIGTERM);
    }
    if (jobbing) {
	signal_ignore(SIGTTOU);
	signal_ignore(SIGTSTP);
	signal_ignore(SIGTTIN);
    }
#endif  //__SYMBIAN32__  
}

/* Source the init scripts.  If called as "ksh" or "sh"  *
 * then we source the standard sh/ksh scripts instead of *
 * the standard zsh scripts                              */

/**/
void
run_init_scripts(void)
{
    noerrexit = -1;

    if (emulation == EMULATE_KSH || emulation == EMULATE_SH) {
	if (islogin)
	    source("/etc/profile");
	if (unset(PRIVILEGED)) {
	    char *s = getsparam("ENV");
	    if (islogin)
		sourcehome(".profile");
	    noerrs = 2;
	    if (s && !parsestr(s)) {
		singsub(&s);
		noerrs = 0;
		source(s);
	    }
	    noerrs = 0;
	} else
	    source("/etc/suid_profile");
    } else {
#ifdef GLOBAL_ZSHENV
	source(GLOBAL_ZSHENV);
#endif
	if (isset(RCS) && unset(PRIVILEGED))
	    sourcehome(".zshenv");
	if (islogin) {
#ifdef GLOBAL_ZPROFILE
	    if (isset(RCS) && isset(GLOBALRCS))
		    source(GLOBAL_ZPROFILE);
#endif
	    if (isset(RCS) && unset(PRIVILEGED))
		sourcehome(".zprofile");
	}
	if (interact) {
#ifdef GLOBAL_ZSHRC
	    if (isset(RCS) && isset(GLOBALRCS))
		source(GLOBAL_ZSHRC);
#endif
	    if (isset(RCS) && unset(PRIVILEGED))
		sourcehome(".zshrc");
	}
	if (islogin) {
#ifdef GLOBAL_ZLOGIN
	    if (isset(RCS) && isset(GLOBALRCS))
		source(GLOBAL_ZLOGIN);
#endif
	    if (isset(RCS) && unset(PRIVILEGED))
		sourcehome(".zlogin");
	}
    }
    noerrexit = 0;
    nohistsave = 0;
}

/* Miscellaneous initializations that happen after init scripts are run */

/**/
void
init_misc(void)
{
#ifndef RESTRICTED_R
    if ( restricted )
#else
    if (*zsh_name == 'r' || restricted)
#endif
	dosetopt(RESTRICTED, 1, 0);
    if (cmd) {
	if (SHIN >= 10)
	    fclose(bshin);
	SHIN = movefd(open("/dev/null", O_RDONLY | O_NOCTTY));
	bshin = fdopen(SHIN, "r");
	execstring(cmd, 0, 1);
	stopmsg = 1;
	zexit(lastval, 0);
    }

    if (interact && isset(RCS))
	readhistfile(NULL, 0, HFILE_USE_OPTIONS);
}

/* source a file */

/**/
int
source(char *s)
{
    Eprog prog;
    int tempfd = -1, fd, cj, oldlineno;
    int oldshst, osubsh, oloops;
    FILE *obshin;
    char *old_scriptname = scriptname, *us;
    unsigned char *ocs;
    int ocsp;

    if (!s || 
	(!(prog = try_source_file((us = unmeta(s)))) &&
	 (tempfd = movefd(open(us, O_RDONLY | O_NOCTTY))) == -1)) {
	return 1;
    }

    /* save the current shell state */
    fd        = SHIN;            /* store the shell input fd                  */
    obshin    = bshin;          /* store file handle for buffered shell input */
    osubsh    = subsh;           /* store whether we are in a subshell        */
    cj        = thisjob;         /* store our current job number              */
    oldlineno = lineno;          /* store our current lineno                  */
    oloops    = loops;           /* stored the # of nested loops we are in    */
    oldshst   = opts[SHINSTDIN]; /* store current value of this option        */
    ocs = cmdstack;
    ocsp = cmdsp;
    cmdstack = (unsigned char *) zalloc(CMDSTACKSZ);
    cmdsp = 0;

    if (!prog) {
	SHIN = tempfd;
	bshin = fdopen(SHIN, "r");
    }
    subsh  = 0;
    lineno = 1;
    loops  = 0;
    dosetopt(SHINSTDIN, 0, 1);
    scriptname = s;

    sourcelevel++;
    if (prog) {
	pushheap();
	errflag = 0;
	execode(prog, 1, 0);
	popheap();
    } else
	loop(0, 0);		     /* loop through the file to be sourced  */
    sourcelevel--;

    /* restore the current shell state */
    if (prog)
	freeeprog(prog);
    else {
	fclose(bshin);
	fdtable[SHIN] = 0;
	SHIN = fd;		     /* the shell input fd                   */
	bshin = obshin;		     /* file handle for buffered shell input */
    }
    subsh = osubsh;                  /* whether we are in a subshell         */
    thisjob = cj;                    /* current job number                   */
    lineno = oldlineno;              /* our current lineno                   */
    loops = oloops;                  /* the # of nested loops we are in      */
    dosetopt(SHINSTDIN, oldshst, 1); /* SHINSTDIN option                     */
    errflag = 0;
    if (!exit_pending)
	retflag = 0;
    scriptname = old_scriptname;
    free(cmdstack);
    cmdstack = ocs;
    cmdsp = ocsp;

    return 0;
}

/* Try to source a file in the home directory */

/**/
void
sourcehome(char *s)
{
    char *h;

    queue_signals();
    if (emulation == EMULATE_SH || emulation == EMULATE_KSH ||
	!(h = getsparam("ZDOTDIR")))
	h = home;

    {
	/* Let source() complain if path is too long */
	VARARR(char, buf, strlen(h) + strlen(s) + 2);
	sprintf(buf, "%s/%s", h, s);
	unqueue_signals();
	source(buf);
    }
}

/**/
void
init_bltinmods(void)
{

#include "bltinmods.list"

    load_module("zsh/main");
}

/**/
mod_export void
noop_function(void)
{
    /* do nothing */
}

/**/
mod_export void
noop_function_int(UNUSED(int nothing))
{
    /* do nothing */
}

/* ZLE entry point pointers.  They are defined here because the initial *
 * values depend on whether ZLE is linked in or not -- if it is, we     *
 * avoid wasting space with the fallback functions.  No other source    *
 * file needs to know which modules are linked in.                      */

#ifdef LINKED_XMOD_zshQszle

/**/
mod_export ZleVoidFn trashzleptr = noop_function;
/**/
mod_export ZleVoidFn zle_resetpromptptr = noop_function;
/**/
mod_export ZleVoidFn refreshptr = noop_function;
/**/
mod_export ZleVoidIntFn spaceinlineptr = noop_function_int;
/**/
mod_export ZleReadFn zlereadptr = autoload_zleread;
/**/
mod_export ZleVoidIntFn zlesetkeymapptr = noop_function_int;

#else /* !LINKED_XMOD_zshQszle */

mod_export ZleVoidFn trashzleptr = noop_function;
mod_export ZleVoidFn zle_resetpromptptr = noop_function;
mod_export ZleVoidFn refreshptr = noop_function;
mod_export ZleVoidIntFn spaceinlineptr = noop_function_int;
# ifdef UNLINKED_XMOD_zshQszle
mod_export ZleReadFn zlereadptr = autoload_zleread;
mod_export ZleVoidIntFn zlesetkeymapptr = autoload_zlesetkeymap;
# else /* !UNLINKED_XMOD_zshQszle */
mod_export ZleReadFn zlereadptr = fallback_zleread;
mod_export ZleVoidIntFn zlesetkeymapptr = noop_function_int;
# endif /* !UNLINKED_XMOD_zshQszle */

#endif /* !LINKED_XMOD_zshQszle */

/**/
unsigned char *
autoload_zleread(char **lp, char **rp, int ha, int con)
{
    zlereadptr = fallback_zleread;
#ifndef __SYMBIAN32__    
    if (load_module("zsh/zle"))
	load_module("zsh/compctl");
#endif    
    return zleread(lp, rp, ha, con);
}

/**/
mod_export unsigned char *
fallback_zleread(char **lp, UNUSED(char **rp), UNUSED(int ha), UNUSED(int con))
{
    char *pptbuf;
    int pptlen;

    pptbuf = unmetafy(promptexpand(lp ? *lp : NULL, 0, NULL, NULL), &pptlen);
    write(1, (WRITE_ARG_2_T)pptbuf, pptlen);
    free(pptbuf);

    return (unsigned char *)shingetline();
}

/**/
static void
autoload_zlesetkeymap(int mode)
{
    zlesetkeymapptr = noop_function_int;
    load_module("zsh/zle");
    (*zlesetkeymapptr)(mode);
}


/* compctl entry point pointers.  Similar to the ZLE ones. */

/**/
mod_export CompctlReadFn compctlreadptr = fallback_compctlread;

/**/
mod_export int
fallback_compctlread(char *name, UNUSED(char **args), UNUSED(Options ops), UNUSED(char *reply))
{
    zwarnnam(name, "option valid only in functions called from completion",
	    NULL, 0);
    return 1;
}


#ifdef __SYMBIAN32__
//typdef for command function ptr..
typedef int (*fptr)(char*, char**, Options, int);

//enumator to be used with zsh_main to located the internal command...
typedef enum
{
	CMD=0,
	CMDARGS,
	OPT_IND,
	OPT_ARGS,
	OPT_ARGCNT,
	OPT_ARGALLOC,
	FUNC_ID,
}ARG_SCAN;

//map of command name to the command function ptr...
typedef struct commandmap
{
	char* cmdname;
	fptr cmdptr;	
}CMD_MAP;

//fill this table with all the builtin functions....
static CMD_MAP cmd_tab[]=
{
	{"ls", bin_ls},
	{"cp", bin_cp },
	{"more", bin_more},
	{"echo", bin_print},	
	{"read", bin_read},	
	{"alias",	bin_alias},
	{"autoload",	bin_functions},
	{"break",	bin_break},
	{"cd",	bin_cd},
	{"dirs",	bin_dirs},
	{"echo",	bin_print},
	{"emulate",	bin_emulate},
	{"eval",	bin_eval},
	{"fc",	bin_fc},
	{"history",	bin_fc},
	{"let",	bin_let},
	{"print",	bin_print},
	{"printf",	bin_print},
	{"pwd",	bin_pwd},
	{"set",	bin_set},
	{"setopt",	bin_setopt},
	{"shift",	bin_shift},
	{"source",	bin_dot},
	{"true",	bin_true},
	{"type",	bin_whence},
	{"typeset",	bin_typeset},
	{"unalias",	bin_unhash},
	{"whence",	bin_whence},
	{"mkdir",	bin_mkdir},
	{"mv",	bin_ln},
	{"rm",	bin_rm},
	{"rmdir",	bin_rmdir},
	{"touch",	bin_touch},
	{"cat",	bin_cat},
		
	{NULL, NULL},
};

//map the command string to the function pointer..
fptr mapCommand(char* cmd)
{
	if(cmd==NULL)
		return NULL;
	
	else
		{
		int idx=0;
		while(cmd_tab[idx].cmdname!=NULL)	
			{
			if(strcmp(cmd, cmd_tab[idx].cmdname)==0)
				return cmd_tab[idx].cmdptr;
				
			idx++;	
			}
		}
	return NULL;	
}

//get the value for the given var from the env list...
char* get_value(char* var, char** env_list)
{
	char* pwd = NULL;
	size_t idx=0;
	
	if(!var || !env_list)
		return NULL;
	
	while(env_list[idx])
		{
		char* temp=env_list[idx];
		if(strstr(temp, var))
			{
			while(*temp!='='&& temp++);
			
			if(temp++)
				pwd=temp;
			break;
			}
		idx++;	
		}	
	return pwd; 	
}	
#endif //__SYMBIAN32__


/*
 * This is real main entry point. This has to be mod_export'ed
 * so zsh.exe can found it on Cygwin
 */

/**/
mod_export int
zsh_main(int argc, char **argv, char** env)
{
    char **t;
    int t0;
	
#ifdef USE_LOCALE
    setlocale(LC_ALL, "");
#endif
	
#ifdef __SYMBIAN32__
	{
	if(argc>7 && (argv && *argv))	
		{
		char* command=NULL;
		char** cmdargs=NULL;    
		char* opt_ind=NULL;
		char** optargs=NULL;
		int optargcnt=0;
		int optargalloc=0;
		int funcid=0;
		ARG_SCAN arg_scan=CMD;
		char* path=get_value("PWD", env);
		
		setupvals();
		if(path)
			chdir(path);
		
		++argv; //ignore the first argument..		
		switch(arg_scan)
			{
			case CMD:
				command=argv[0];
				++argv;
				
			//fall thru	
			case CMDARGS:
				{
				int idx=0;
				int argcnt=atoi(*argv);
				cmdargs=(char**)calloc(argcnt+1, sizeof(char*));	
				if(cmdargs==NULL)
					exit(1);
				if(argcnt>0)
					{
					argzero=argv[1];
					while(idx<argcnt)
						{
						cmdargs[idx]=argv[idx+1];
						idx++;	
						}
					cmdargs[argcnt]=NULL;
					}
				argv=argv+argcnt+1;
				}
			//fall thru	
			case OPT_IND:
				{
				int argcnt=atoi(*argv);
				if(argcnt>0)
					{
					opt_ind=argv[1];	
					argv++;
					}
				argv++;	
				}
				
			//fall thru
			case OPT_ARGS:
				{
				int idx=0;
				int argcnt=atoi(*argv);
				optargs=(char**)calloc(argcnt+1, sizeof(char*));	
				if(optargs==NULL)
						exit(1);
				if(argcnt>0)
					{		
					while(idx<argcnt)
						{
						optargs[idx]=argv[idx+1];
						idx++;	
						}
					optargs[argcnt]=NULL;
					}
				argv=argv+argcnt+1;
				}
			//fall thru
			case OPT_ARGCNT:
				optargcnt=atoi(*argv);
				++argv;
				
			//fall thru
			case OPT_ARGALLOC:
				optargalloc=atoi(*argv);
				++argv;
							
			//fall thru			
			case FUNC_ID:
				funcid=atoi(*argv);
			break;
				
			default:
			break;	
			}
			
		if(command)
			{
			fptr cmd=mapCommand(command);
			pipeUsed=1;			
			if(cmd)
				{
				int idx=0;
				char temp[2];
				struct options ops;
				memset(ops.ind, 0, MAX_OPS);
				while(opt_ind && idx<MAX_OPS)
					{
					sprintf(temp, "%c", opt_ind[idx]);
					ops.ind[idx]=atoi(temp);
					
					idx++;	
					}
					
				ops.args=optargs;
				ops.argscount=optargcnt;
				ops.argsalloc=optargalloc;
				
				(*cmd)(command, cmdargs, &ops, funcid);				
				}

			fflush(stdout);				
			free(cmdargs);
			free(optargs);
			exit(0);
			}	
			
		free(cmdargs);
		free(optargs);
		exit(0);
		}
	}
#endif	//__SYMBIAN32__

    init_jobs(argv, environ);	
   	 	
    /*
     * Provisionally set up the type table to allow metafication.
     * This will be done properly when we have decided if we are
     * interactive
     */
    typtab['\0'] |= IMETA;
    typtab[STOUC(Meta)  ] |= IMETA;
    typtab[STOUC(Marker)] |= IMETA;
    for (t0 = (int)STOUC(Pound); t0 <= (int)STOUC(Nularg); t0++)
	typtab[t0] |= ITOK | IMETA;

    for (t = argv; *t; *t = metafy(*t, -1, META_ALLOC), t++);

    zsh_name = argv[0];
    do {
      char *arg0 = zsh_name;
      if (!(zsh_name = strrchr(arg0, '\\'))) 
	  zsh_name = arg0;
      else
	  zsh_name++;
      if (*zsh_name == '-')
	  zsh_name++;
      if (strcmp(zsh_name, "su") == 0) {
	  char *sh = zgetenv("SHELL");
	  if (sh && *sh && arg0 != sh)
	      zsh_name = sh;
	  else
	      break;
      } else
	  break;
    } while (zsh_name);

    fdtable_size = zopenmax();
    fdtable = zshcalloc(fdtable_size);

    createoptiontable();
    emulate(zsh_name, 1);   /* initialises most options */
    opts[LOGINSHELL] = (**argv == '-');
    opts[MONITOR] = 1;   /* may be unset in init_io() */
    opts[PRIVILEGED] = (getuid() != geteuid() || getgid() != getegid());
    opts[USEZLE] = 1;   /* may be unset in init_io() */
    parseargs(argv);   /* sets INTERACTIVE, SHINSTDIN and SINGLECOMMAND */

    SHTTY = -1;
    init_io();
    setupvals();
    init_signals();
    init_bltinmods();
    run_init_scripts();
    init_misc();

    for (;;) {
	/*
	 * See if we can free up some of jobtab.
	 * We only do this at top level, because if we are
	 * executing stuff we may refer to them by job pointer.
	 */
	maybeshrinkjobtab();

	do
	    loop(1,0);
	while (tok != ENDINPUT && (tok != LEXERR || isset(SHINSTDIN)));
	if (tok == LEXERR) {
	    /* Make sure a parse error exits with non-zero status */
	    if (!lastval)
		lastval = 1;
	    stopmsg = 1;
	    zexit(lastval, 0);
	}
	if (!(isset(IGNOREEOF) && interact)) {
#if 0
	    if (interact)
		fputs(islogin ? "logout\n" : "exit\n", shout);
#endif
	    zexit(lastval, 0);
	    continue;
	}
	noexitct++;
	if (noexitct >= 10) {
	    stopmsg = 1;
	    zexit(lastval, 0);
	}
	zerrnam("zsh", (!islogin) ? "use 'exit' to exit."
		: "use 'logout' to logout.", NULL, 0);
    }
}
