#include <string.h>
#include <assert.h>
#include <ctype.h>

#include <confuse/config.h>


#ifndef HAVE_STRDUP
# ifdef HAVE__STRDUP
#  define strdup _strdup
# else
/*
 * Copyright (c) 1988, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
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
 */
static char *strdup(const char *s)
{
	size_t siz;
	char *copy;

	siz = strlen(str) + 1;
	if ((copy = malloc(siz)) == NULL)
		return(NULL);
	memcpy(copy, str, siz);
	return(copy);
}
# endif
#endif

#ifndef HAVE_STRNDUP
static char *strndup(const char *s, size_t n)
{
    char *r;

    if(s == 0)
        return 0;

    r = malloc(n + 1);
    assert(r);
    strncpy(r, s, n);
    r[n] = 0;
    return r;
}
#endif

#ifndef HAVE_STRCASECMP
int strcasecmp(const char *s1, const char *s2)
{
    assert(s1);
    assert(s2);

    while(*s1)
    {
	int c1 = tolower(*(const unsigned char *)s1);
	int c2 = tolower(*(const unsigned char *)s2);
	if(c1 < c2)
	    return -1;
	if(c1 > c2)
	    return +1;

	++s1;
	++s2;
    }

    if(*s2 != 0)
	return -1;

    return 0;
}
#endif
