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


/*	$NetBSD: mmfile.c,v 1.3 2006/05/15 21:12:21 rillig Exp $	*/

/*-
 * Copyright (c) 1999 James Howard and Dag-Erling Codan Smrgrav
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/cdefs.h>
#ifndef lint
__RCSID("$NetBSD: mmfile.c,v 1.3 2006/05/15 21:12:21 rillig Exp $");
#endif /* not lint */

#include "grep.h"

#define MAX_MAP_LEN (SIZE_T_MAX / 64)	/* ~70M is reasonable size */

/* Open the mmap file */
mmf_t *
mmopen(const char *fn, const char *mode)
{
	mmf_t *mmf;
	struct stat st;

	/* XXX ignore mode for now */
	mode = mode;

	mmf = (mmf_t *)grep_malloc(sizeof *mmf);	
	if ((mmf->fd = open(fn, O_RDONLY)) == -1)
		goto ouch1;
	if (fstat(mmf->fd, &st) == -1)
		goto ouch2;
	if (st.st_size > MAX_MAP_LEN)		/* too big to mmap */		
		goto ouch2;
	if ((st.st_mode & S_IFREG) == 0)	/* only mmap regular files */
		goto ouch2;
	mmf->len = (size_t)st.st_size;
	mmf->base = (char *)mmap(NULL, mmf->len, PROT_READ, MAP_PRIVATE, mmf->fd, 0);
	if (mmf->base == MAP_FAILED)
		goto ouch2;
	mmf->ptr = mmf->base;
	mmf->end = mmf->base + mmf->len;
	return mmf;

ouch2:
	close(mmf->fd);
ouch1:
	free(mmf);
	return (mmf_t*)NULL;
}

/* Clode the mmap file */
void
mmclose(mmf_t *mmf)
{
	munmap(mmf->base, mmf->len);
	close(mmf->fd);
	free(mmf);
}

/* Get a line of data from the mmaped file */
char *
mmfgetln(mmf_t *mmf, size_t *l)
{
	static char *p;

	if (mmf->ptr >= mmf->end)
		return NULL;
	for (p = mmf->ptr; mmf->ptr < mmf->end; ++mmf->ptr)
		if (*mmf->ptr == line_endchar)
			break;
	*l = mmf->ptr - p;
	++mmf->ptr;
	return p;
}

/* Rewind in mmaped file */
void
mmrewind(mmf_t *mmf)
{
	mmf->ptr = mmf->base;
}
