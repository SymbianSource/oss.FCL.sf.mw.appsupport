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
 * Copyright (c) 2002 Peter Stephenson
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and to distribute modified versions of this software for any
 * purpose, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * In no event shall Peter Stephenson or the Zsh Development
 * Group be liable to any party for direct, indirect, special, incidental,
 * or consequential damages arising out of the use of this software and
 * its documentation, even if Peter Stephenson, and the Zsh
 * Development Group have been advised of the possibility of such damage.
 *
 * Peter Stephenson and the Zsh Development Group specifically
 * disclaim any warranties, including, but not limited to, the implied
 * warranties of merchantability and fitness for a particular purpose.  The
 * software provided hereunder is on an "as is" basis, and Peter Stephenson
 * and the Zsh Development Group have no obligation to provide maintenance,
 * support, updates, enhancements, or modifications.
 *
 */
#include "socket.mdh"
#include "socket.pro"

#ifdef __SYMBIAN32__
#include "dummy.h" //poll
#endif //__SYMBIAN32__


#include <sys/socket.h>
#ifndef __SYMBIAN32__
#include <sys/un.h>
#else
#include "un.h"
#endif

#ifndef UNIX_PATH_MAX
# define UNIX_PATH_MAX 108
#endif

/*
 * We need to include the zsh headers later to avoid clashes with
 * the definitions on some systems, however we need the configuration
 * file to decide whether we can include netinet/in_systm.h, which
 * doesn't exist on cygwin.
 */

/*
 * We use poll() in preference to select because some subset of manuals says
 * that's the thing to do, plus it's a bit less fiddly.  I don't actually
 * have access to a system with poll but not select, however, though
 * both bits of the code have been tested on a machine with both.
 */
#ifdef HAVE_POLL_H
#ifndef __SYMBIAN32__
# include <sys/poll.h> 
#else
# include "poll.h"
#endif
#endif
#if defined(HAVE_POLL) && !defined(POLLIN) && !defined(POLLNORM)
# undef HAVE_POLL
#endif

#ifdef __SYMBIAN32__
#ifdef __WINSCW__
#pragma warn_unusedarg off
#endif//__WINSCW__
#endif//__SYMBIAN32__

static int
bin_zsocket(char *nam, char **args, Options ops, UNUSED(int func))
{
    int err=1, verbose=0, test=0, targetfd=0;
    ZSOCKLEN_T len;
    struct sockaddr_un soun;
    int sfd;

    if (OPT_ISSET(ops,'v'))
	verbose = 1;

    if (OPT_ISSET(ops,'t'))
	test = 1;

    if (OPT_ISSET(ops,'d')) {
	targetfd = atoi(OPT_ARG(ops,'d'));
	if (!targetfd) {
	    zwarnnam(nam, "%s is an invalid argument to -d",
		     OPT_ARG(ops, 'd'), 0);
	    return 1;
	}
    }

    if (OPT_ISSET(ops,'l')) {
	char *localfn;

	if (!args[0]) {
	    zwarnnam(nam, "-l requires an argument", NULL, 0);
	    return 1;
	}

	localfn = args[0];

	sfd = socket(PF_UNIX, SOCK_STREAM, 0);

	if (sfd == -1) {
	    zwarnnam(nam, "socket error: %e ", NULL, errno);
	    return 1;
	}

	soun.sun_family = AF_UNIX;
	strncpy(soun.sun_path, localfn, UNIX_PATH_MAX);

	if (bind(sfd, (struct sockaddr *)&soun, sizeof(struct sockaddr_un)))
	{
	    zwarnnam(nam, "could not bind to %s: %e", soun.sun_path, errno);
	    close(sfd);
	    return 1;
	}

	if (listen(sfd, 1))
	{
	    zwarnnam(nam, "could not listen on socket: %e", NULL, errno);
	    close(sfd);
	    return 1;
	}

	if (targetfd) {
	    redup(sfd, targetfd);
	    sfd = targetfd;
	}
	else {
	    /* move the fd since no one will want to read from it */
	    sfd = movefd(sfd);
	}

	setiparam("REPLY", sfd);

	if (verbose)
	    printf("%s listener is on fd %d\n", soun.sun_path, sfd);

	return 0;

    }
    else if (OPT_ISSET(ops,'a'))
    {
	int lfd, rfd;

	if (!args[0]) {
	    zwarnnam(nam, "-a requires an argument", NULL, 0);
	    return 1;
	}

	lfd = atoi(args[0]);

	if (!lfd) {
	    zwarnnam(nam, "invalid numerical argument", NULL, 0);
	    return 1;
	}

	if (test) {
#if defined(HAVE_POLL) || defined(HAVE_SELECT)
# ifdef HAVE_POLL
	    struct pollfd pfd;
	    int ret;

	    pfd.fd = lfd;
	    pfd.events = POLLIN;
	    if ((ret = poll(&pfd, 1, 0)) == 0) return 1;
	    else if (ret == -1)
	    {
		zwarnnam(nam, "poll error: %e", NULL, errno);
		return 1;
	    }
# else
	    fd_set rfds;
	    struct timeval tv;
	    int ret;
	    
	    FD_ZERO(&rfds);
	    FD_SET(lfd, &rfds);
	    tv.tv_sec = 0;
	    tv.tv_usec = 0;
	    
	    if ((ret = select(lfd+1, &rfds, NULL, NULL, &tv))) return 1;
	    else if (ret == -1)
	    {
		zwarnnam(nam, "select error: %e", NULL, errno);
		return 1;
	    }
	    
# endif
	    
#else
	    zwarnnam(nam, "not currently supported", NULL, 0);
	    return 1;
#endif
	}

	len = sizeof(soun);
	if ((rfd = accept(lfd, (struct sockaddr *)&soun, &len)) == -1)
	{
	    zwarnnam(nam, "could not accept connection: %e", NULL, errno);
	    return 1;
	}

	if (targetfd) {
	    redup(rfd, targetfd);
	    sfd = targetfd;
	}
	else {
	    sfd = rfd;
	}

	setiparam("REPLY", sfd);

	if (verbose)
	    printf("new connection from %s is on fd %d\n", soun.sun_path, sfd);
    }
    else
    {
	if (!args[0]) {
	    zwarnnam(nam, "zsocket requires an argument", NULL, 0);
	    return 1;
	}

	sfd = socket(PF_UNIX, SOCK_STREAM, 0);

	if (sfd == -1) {
	    zwarnnam(nam, "socket creation failed: %e", NULL, errno);
	    return 1;
	}

	soun.sun_family = AF_UNIX;
	strncpy(soun.sun_path, args[0], UNIX_PATH_MAX);
	
	if ((err = connect(sfd, (struct sockaddr *)&soun, sizeof(struct sockaddr_un)))) {
	    zwarnnam(nam, "connection failed: %e", NULL, errno);
	    close(sfd);
	    return 1;
	}
	else
	{
	    if (targetfd) {
		redup(sfd, targetfd);
		sfd = targetfd;
	    }

	    setiparam("REPLY", sfd);

	    if (verbose)
		printf("%s is now on fd %d\n", soun.sun_path, sfd);
	}
	
    }

    return 0;
}

static struct builtin bintab[] = {
    BUILTIN("zsocket", 0, bin_zsocket, 0, 3, 0, "ad:ltv", NULL),
};

/* The load/unload routines required by the zsh library interface */

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
    return !addbuiltins(m->nam, bintab, sizeof(bintab)/sizeof(*bintab));
}


/**/
int
cleanup_(Module m)
{
    deletebuiltins(m->nam, bintab, sizeof(bintab)/sizeof(*bintab));
    return 0;
}

/**/
int
finish_(UNUSED(Module m))
{
    return 0;
}
