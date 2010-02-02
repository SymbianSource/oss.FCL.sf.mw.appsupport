/*	$Id: ls.h,v 1.2 2000/08/22 22:20:23 amai Exp $	*/
/*	$NetBSD: ls.h,v 1.7 1995/03/21 09:06:33 cgd Exp $	*/

/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Michael Fischbein.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)ls.h	8.1 (Berkeley) 5/31/93
 */

#include <sys/types.h>
#include <sys/stat.h>
#include "fts.h"

#ifndef S_ISBLK
#warning Cannot identify block devices (drives)!
#define S_ISBLK(m) (0)
#endif


#define NO_PRINT	1

extern long blocksize;		/* block size units */

extern int f_accesstime;	/* use time of last access */
extern int f_flags;		/* show flags associated with a file */
extern int f_inode;		/* print inode */
extern int f_longform;		/* long listing format */
extern int f_nonprint;		/* show unprintables as ? */
extern int f_sectime;		/* print the real time for all files */
extern int f_size;		/* list size in short listing */
extern int f_statustime;	/* use time of last mode change */
extern int f_type;		/* add type character for non-regular files */
extern int f_typedir;		/* add type character for directories */

typedef struct {
	FTSENT *list;
	u_long btotal;
	int bcfile;
	int entries;
	int maxlen;
	int s_block;
	int s_flags;
	int s_group;
	int s_inode;
	int s_nlink;
	int s_size;
	int s_user;
} DISPLAY;

typedef struct {
	char *user;
	char *group;
	char *flags;
	char data[1];
} NAMES;

int	 acccmp __P((const FTSENT *, const FTSENT *));
int	 revacccmp __P((const FTSENT *, const FTSENT *));
int	 modcmp __P((const FTSENT *, const FTSENT *));
int	 revmodcmp __P((const FTSENT *, const FTSENT *));
int	 namecmp __P((const FTSENT *, const FTSENT *));
int	 revnamecmp __P((const FTSENT *, const FTSENT *));
int	 statcmp __P((const FTSENT *, const FTSENT *));
int	 revstatcmp __P((const FTSENT *, const FTSENT *));
int	 sizecmp __P((const FTSENT *, const FTSENT *));
int	 revsizecmp __P((const FTSENT *, const FTSENT *));

char	*flags_to_string __P((int, char *));
int	 putname __P((char *));
void	 printcol __P((DISPLAY *));
void	 printacol __P((DISPLAY *));
void	 printlong __P((DISPLAY *));
void	 printscol __P((DISPLAY *));
void	 printstream __P((DISPLAY *));
void	 usage __P((void));

