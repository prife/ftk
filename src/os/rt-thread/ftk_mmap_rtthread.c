
#include "ftk_log.h"
#include "ftk_mmap.h"
#include "ftk_allocator.h"

#undef SEEK_CUR
#undef SEEK_END
#undef SEEK_SET

#include <dfs_posix.h>

struct _FtkMmap
{
	int fd;
	void* data;
	size_t length;
};

int      ftk_mmap_exist(const char* filename)
{
	struct _stat st = {0};
	return_val_if_fail(filename != NULL, 0);

	return stat(filename, &st) == 0;
}

FtkMmap* ftk_mmap_create(const char* filename, size_t offset, size_t size)
{
	FtkMmap* thiz = NULL;
	struct _stat st = {0};
	ssize_t ret = 0;
	return_val_if_fail(filename != NULL, NULL);
	return_val_if_fail(stat(filename, &st) == 0, NULL);
	return_val_if_fail(offset < st.st_size, NULL);

	size = (offset + size) <= st.st_size ? size : st.st_size - offset;

	thiz = FTK_ZALLOC(sizeof(FtkMmap));
	return_val_if_fail(thiz != NULL, NULL);

	thiz->fd = open(filename, O_RDONLY, 0);
	if (thiz->fd < 0)
	{
		FTK_ZFREE(thiz, sizeof(*thiz));
		return NULL;
	}

	thiz->length = size;
	thiz->data = FTK_ALLOC(size);
	if(thiz->data == NULL)
	{
		close(thiz->fd);
		FTK_ZFREE(thiz, sizeof(*thiz));
		return NULL;
	}

	ret = read(thiz->fd, thiz->data, size);
	close(thiz->fd);
	if (ret != size)
	{
		FTK_ZFREE(thiz->data, size);
		FTK_ZFREE(thiz, sizeof(*thiz));
		return NULL;
	}

	return thiz;
}

void*    ftk_mmap_data(FtkMmap* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->data;
}

size_t   ftk_mmap_length(FtkMmap* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->length;
}

void     ftk_mmap_destroy(FtkMmap* thiz)
{
	if (thiz != NULL)
	{
		FTK_ZFREE(thiz->data, thiz->length);
		FTK_ZFREE(thiz, sizeof(*thiz));
	}
}


