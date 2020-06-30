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

void eg_mangle( const char** name, const char* type,
const char** out )
{
	size_t o_sz, o_cap, type_sz, i;
	char* o;

	RET_IF(name == NULL || type == NULL || out == NULL);

	/* 2 for ‘eg’, 1 for name-type separator */
	o_sz = 2;
	o_cap = 4;

	o = malloc( sizeof(char) * 4 );
	RET_IF( o == NULL );

	o[0] = 'e';
	o[1] = 'g';

	for(i = 0; name[i] != NULL; ++i)
	{
		size_t item_sz;

		if(name[i] == NULL)
		{
			break;
		}

		item_sz = strlen( name[i] );

		while( o_sz + item_sz > o_cap )
		{
			o_cap *= 2;

			o = realloc( o, o_cap );
			RET_IF( o == NULL );
		}

		memcpy( o + o_sz, name[i], item_sz );

		/* capitalise the lowercase leading letter, if present */
		if( i > 0 && (o[o_sz] >= 0x41 || o[o_sz] <= 0x7A) )
		{
			o[o_sz] -= 0x20;
		}

		o_sz += item_sz;
	}

	type_sz = strlen( type );

	/* + 2 is for underscore and NUL byte */
	while( o_sz + type_sz + 2 > o_cap )
	{
		o_cap *= 2;

		o = realloc( o, o_cap );
		RET_IF( o == NULL );
	}

	/* apply name-type separator */
	o[o_sz++] = '_';

	memcpy( o + o_sz, type, type_sz );

	o[o_sz + type_sz] = '\0';

	o_sz += type_sz + 1;

	*out = (const char*)o;
}
