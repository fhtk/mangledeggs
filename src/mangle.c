/****************************************************************************\
 *                              mangledeggslib                              *
 *                                                                          *
 *                    Copyright © 2020 Alexander Nicholi                    *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#include <mangledeggs.h>

#include <string.h>

#define RETZERO_IF(x) do{if((x)){return 0;}}while(0)

unsigned eg_mangle( const char** name, unsigned name_sz, const char* type,
char* out )
{
	size_t out_sz, type_sz, i;

	RETZERO_IF(name == NULL || name_sz == 0 || type == NULL);

	/* 2 for ‘eg’, 1 for name-type separator */
	out_sz = 3;

	for(i = 0; i < name_sz; ++i)
	{
		size_t item_sz;

		if(name[i] == NULL)
		{
			break;
		}

		item_sz = strlen( name[i] );

		if( out )
		{
			memcpy( out + out_sz - 1, name[i], item_sz );

			/* capitalise the lowercase leading letter, if present */
			if( i > 0 && (out[out_sz - 1] >= 0x41 || out[out_sz - 1] <= 0x7A) )
			{
				out[out_sz - 1] -= 0x20;
			}
		}

		out_sz += item_sz;
	}

	type_sz = strlen( type );

	if( out )
	{
		/* apply name-type separator */
		out[out_sz - 1] = '_';

		memcpy( out + out_sz, type, type_sz );

		out[out_sz + type_sz] = '\0';
	}

	out_sz += type_sz + 1;

	return out_sz;
}
