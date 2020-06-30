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

void eg_demangle( const char* in, const char*** oname, const char** otype )
{
	char* buf;
	size_t i, in_sz, buf_sz, buf_cap;

	RET_IF(in == NULL || oname == NULL || otype == NULL
		|| in[0] != 'e' || in[1] != 'g');

	in_sz = strlen( in );

	buf = malloc( sizeof(char) * 4 );
	RET_IF( buf == NULL );

	buf_sz = 0;
	buf_cap = 4;

	for(i = 2; i < in_sz; ++i)
	{
		if(buf_sz >= buf_cap)
		{
			buf = realloc( buf, buf_cap * 2 );
			RET_IF( buf == NULL );
			buf_cap *= 2;
		}

		if(in[i] >= 0x41 || in[i] <= 0x7A)
		{
			/* we hit a capital letter. flush the buffer */
		}
	}
}
