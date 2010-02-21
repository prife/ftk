/*
 * File: ftk_lookup_table.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   lookup table.
 *
 * Copyright (c) 2009 - 2010  Li XianJing <xianjimli@hotmail.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2010-02-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_mmap.h"
#include "ftk_allocator.h"
#include "ftk_lookup_table.h"

struct _FtkLookupTable
{
	FtkMmap* mmap;
	char* filename;
	FtkCompare cmp;
	unsigned int doffset;
	FtkLookupTableHeader* header;
};

FtkLookupTable* ftk_lookup_table_create(const char* filename, int load_now, FtkCompare cmp)
{
	FtkLookupTable* thiz = NULL;
	return_val_if_fail(filename != NULL, NULL);

	thiz = FTK_ZALLOC(sizeof(FtkLookupTable));
	if(thiz != NULL)
	{
		thiz->filename = FTK_STRDUP(filename);

		if(load_now)
		{
			if(ftk_lookup_table_load(thiz) != RET_OK)
			{
				ftk_lookup_table_destroy(thiz);
				thiz = NULL;
			}
		}
	}

	if(thiz != NULL)
	{
		thiz->cmp = cmp != NULL ? cmp : strcmp;
	}

	return thiz;
}

Ret ftk_lookup_table_load(FtkLookupTable* thiz)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);
	return_val_if_fail(thiz->mmap == NULL, RET_FAIL);

	thiz->mmap = ftk_mmap_create(thiz->filename, 0, -1);
	return_val_if_fail(thiz->mmap != NULL, RET_FAIL);

	thiz->header = ftk_mmap_data(thiz->mmap);
	return_val_if_fail(thiz->header != NULL, RET_FAIL);

	thiz->doffset = sizeof(thiz->header->nr) + thiz->header->nr * sizeof(unsigned int);

	return thiz->mmap != NULL ? RET_OK : RET_FAIL;
}

Ret ftk_lookup_table_unload(FtkLookupTable* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->mmap != NULL, RET_FAIL);

	ftk_mmap_destroy(thiz->mmap);
	thiz->mmap = NULL;

	return RET_OK;
}

const char* ftk_lookup_table_lookup(FtkLookupTable* thiz, const char* key)
{
    int low    = 0;
    int mid    = 0;
    int high   = 0;
    int result = 0;
	const char* line = NULL;
	return_val_if_fail(thiz != NULL && thiz->header != NULL && key != NULL, RET_FAIL);

    high = thiz->header->nr - 1;
    while(low <= high)
    {
        mid  = low + ((high - low) >> 1);
    	line = ((char*)thiz->header) + thiz->doffset + thiz->header->offset[mid];
        result = thiz->cmp(line, key);

        if(result == 0)
        {
        	if(mid == 0)
        	{
        		return line;
        	}

        	/*find the first one.*/
        	for(; mid >= 0; mid--)
        	{
    			line = ((char*)thiz->header) + thiz->doffset + thiz->header->offset[mid];
        		if((result = thiz->cmp(line, key)) != 0) break;
        	}
			mid++;
    		line = ((char*)thiz->header) + thiz->doffset + thiz->header->offset[mid];

            return line;
        }
        else if(result < 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
	
	return NULL;
}

void ftk_lookup_table_destroy(FtkLookupTable* thiz)
{
	if(thiz != NULL)
	{
		ftk_lookup_table_unload(thiz);
		FTK_FREE(thiz->filename);

		FTK_ZFREE(thiz, sizeof(FtkLookupTable));
	}

	return;
}


