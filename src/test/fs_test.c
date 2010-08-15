#include "ftk_globals.h"
#include "ftk_file_system.h"
#include "ftk_allocator_default.h"

int main(int argc, char* argv[])
{
	char text[100] = {0};
	char cwd[FTK_MAX_PATH+1];
	char cwd1[FTK_MAX_PATH+1];
	FtkFsHandle handle = NULL;
	char mime_type[FTK_MIME_TYPE_LEN+1];

	ftk_set_allocator(ftk_allocator_default_create());

	assert(ftk_file_get_mime_type("test.jpg", mime_type) == RET_OK && strcmp(mime_type, "image/jpeg") == 0);
	assert(ftk_file_get_mime_type("test.txt", mime_type) == RET_OK && strcmp(mime_type, "text/plain") == 0);
	assert(ftk_fs_get_cwd(cwd) == RET_OK);
	assert(ftk_fs_create_dir("./testdir/a/b/c") == RET_OK);
	assert(ftk_fs_change_dir("./testdir/a/b/c") == RET_OK);
	handle = ftk_file_open("fstest.txt", "wb+");
	assert(handle != NULL);
	assert(ftk_file_write(handle, "test", 4) == 4);
	ftk_file_close(handle);

	assert(ftk_fs_change_dir(cwd) == RET_OK);
	assert(ftk_fs_move("./testdir/a", "./testdir/d") == RET_OK);
	handle = ftk_file_open("./testdir/d/b/c/fstest.txt", "r");
	assert(handle != NULL);
	assert(ftk_file_read(handle, text, 4) == 4);
	assert(strncmp(text, "test", 4) == 0);
	ftk_file_close(handle);
	assert(ftk_fs_get_cwd(cwd1) == RET_OK);
	assert(strcmp(cwd, cwd1) == 0);
	assert(ftk_fs_delete("./testdir/d/b/c") == RET_OK);
	assert(ftk_fs_get_cwd(cwd1) == RET_OK);
	assert(strcmp(cwd, cwd1) == 0);
	assert(ftk_fs_delete("./testdir/d") == RET_OK);

	return 0;
}
