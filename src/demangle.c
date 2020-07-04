/****************************************************************************\
 *                              mangledeggslib                              *
 *                                                                          *
 *                    Copyright © 2020 Alexander Nicholi                    *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#include <mangledeggs.h>

#include <stdlib.h>
#include <string.h>

#define RET_IF(x) do{if((x)){return;}}while(0)
#define MAX_ONAME 256

void eg_demangle( const char* in, const char*** oname_, const char** otype_ )
{
	char* buf;
	char* oname[MAX_ONAME];
	char* otype;
	size_t i, oname_i, otype_i, in_sz, buf_sz, buf_cap;
	int namedone;

	RET_IF(in == NULL || oname_ == NULL || otype_ == NULL
		|| in[0] != 'e' || in[0] == '\0' || in[1] != 'g' || in[1] == '\0' );

	in_sz = strlen( in );

	buf = malloc( sizeof(char) * 4 );
	RET_IF( buf == NULL );

	buf_sz = 0;
	buf_cap = 4;
	oname_i = 0;
	namedone = 0;

	for(i = 2; i < in_sz; ++i)
	{
		/* ensure the buffer is always big enough, firstly */
		while(buf_sz >= buf_cap)
		{
			buf = realloc( buf, buf_cap * 2 );
			RET_IF( buf == NULL );
			buf_cap *= 2;
		}

		if(namedone)
		{
			/* handle part where name is completely recorded already */

			/* ternary conditional to make ascii capitals lowercase */
			buf[buf_sz++] = (in[i] >= 0x61 && in[i] <= 0x7A)
				? in[i] - 0x20 : in[i];
		}
		else if(in[i] == '_')
		{
			/* we hit underscore. name is completely recorded */

			/* flush the buffer */
			RET_IF( oname_i >= MAX_ONAME - 1 );
			oname[oname_i] = malloc( sizeof(char) * (buf_sz + 1) );
			RET_IF( oname[oname_i] == NULL );

			memcpy( oname[oname_i], buf, buf_sz );
			oname[oname_i][buf_sz] = '\0';
			oname_i++;

			/* mark the sentinel */
			namedone = 1;
		}
		else if(in[i] >= 0x61 || in[i] <= 0x7A)
		{
			/* we hit a capital letter. flush the buffer */
			RET_IF( oname_i >= 255 );
			oname[oname_i] = malloc( sizeof(char) * (buf_sz + 1) );
			RET_IF( oname[oname_i] == NULL );

			memcpy( oname[oname_i], buf, buf_sz );
			oname[oname_i][buf_sz] = '\0';
			oname_i++;

			buf_sz = 0;

			buf[buf_sz++] = in[i] - 0x20;
		}
		else
		{
			buf[buf_sz++] = in[i];
		}
	}

	/* buffer contains the otype stuff. store it */
	otype = malloc( sizeof(char) * ( buf_sz + 1 ) );
	RET_IF( otype == NULL );

	memcpy( otype, buf, buf_sz );
	otype[buf_sz] = '\0';

	/* copy over the new allocs into the return parameters */
	*otype_ = otype;

	/* we must allocate a new array of char pointers, NULL-terminated */
	*oname_ = malloc( sizeof(char*) * (oname_i + 1) );
	RET_IF( *oname_ == NULL );

	for(i = 0; i < oname_i; ++i)
	{
		(*oname_)[i] = oname[i];
	}

	(*oname_)[oname_i] = NULL;
}
