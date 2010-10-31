
#include "ftk_log.h"
#include "ftk_mmap.h"
#include "ftk_allocator.h"
#include <zip.h>

struct _FtkMmap
{
	void* data;
	size_t length;
	size_t offset;
	size_t size;
};

extern zip* apk;

int      ftk_mmap_exist(const char* filename)
{
	return_val_if_fail(filename != NULL, 0);

	ftk_logd("ftk_mmap_exist: %s\n", filename);
	filename++;
	ftk_logd("ftk_mmap_exist: %s\n", filename);

	if(strncmp(filename, "assets", strlen(filename)) == 0)
	{
		return 1;
	}

	if(zip_name_locate(apk, filename, 0) != -1)
	{
		return 1;
	}

	return 0;
}

FtkMmap* ftk_mmap_create(const char* filename, size_t offset, size_t size)
{
	FtkMmap* thiz = NULL;
	zip_file* file;
	struct zip_stat st = {0};
	return_val_if_fail(filename != NULL, NULL);

	ftk_logd("ftk_mmap_create: %s\n", filename);
	filename++;
	ftk_logd("ftk_mmap_create: %s\n", filename);

	zip_stat_init(&st);
	if(zip_stat(apk, filename, 0, &st) == -1)
	{
		ftk_loge("%s zip_stat %s failed.\n", __func__, filename);
		return NULL;
	}

	return_val_if_fail(offset < st.size, NULL);

	thiz = (FtkMmap*)FTK_ZALLOC(sizeof(FtkMmap));
	return_val_if_fail(thiz != NULL, NULL);

	file = zip_fopen(apk, filename, 0);
	if(file != NULL)
	{
		thiz->length = st.size;
		thiz->offset = offset;
		thiz->size = size;
		thiz->data = FTK_ZALLOC(st.size);

		zip_fread(file, thiz->data, st.size);
		zip_fclose(file);
	}
	else
	{
		FTK_ZFREE(thiz, sizeof(*thiz));
		ftk_loge("%s zip_fopen %s failed.\n", __func__, filename);
	}

	return thiz;
}

void*    ftk_mmap_data(FtkMmap* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return (unsigned char*)thiz->data + thiz->offset;
}

size_t   ftk_mmap_length(FtkMmap* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->size;
}

void     ftk_mmap_destroy(FtkMmap* thiz)
{
	if(thiz != NULL)
	{
		FTK_FREE(thiz->data);
		FTK_ZFREE(thiz, sizeof(*thiz));
	}
}
