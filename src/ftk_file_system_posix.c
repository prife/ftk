/*
 * File: ftk_file_system_posix.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  posix implemented file system adaptor.
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
 * 2010-08-14 Li XianJing <xianjimli@hotmail.com> created
 * 2010-10-02 Jiao JinXing <jiaojinxing1987@gmail.com> add rt-thread support.
 *
 */

#include "ftk_util.h"
#include "ftk_file_system.h"

Ret ftk_file_get_info(const char* file_name, FtkFileInfo* info)
{
	int ret = 0;
	struct stat st;
	return_val_if_fail(file_name != NULL && info != NULL, RET_FAIL);

	if((ret = stat(file_name, &st)) == 0)
	{
		info->uid  = st.st_uid;
		info->gid  = st.st_gid;
		info->mode = st.st_mode;
		info->size = st.st_size; 
		info->is_dir = S_ISDIR(st.st_mode);
		info->last_access = st.st_atime;
		info->last_modify = st.st_mtime;

		return RET_OK;
	}

	return RET_FAIL;
}

static int ftk_file_parse_mode(const char *mode)
{
	int f = 0;
 
  	for(;;)
	{
    	switch(*mode)
		{
		    case  0 : return f;
		    case 'b': break;
		    case 'r': f = O_RDONLY; break;
		    case 'w': f = O_WRONLY|O_CREAT|O_TRUNC; break;
		    case 'a': f = O_WRONLY|O_CREAT|O_APPEND; break;
		    case '+': f = (f&(~O_WRONLY))|O_RDWR; break;
    	}
    	++mode;
  	}
}

FtkFsHandle ftk_file_open(const char* file_name, const char* mode)
{
	return_val_if_fail(file_name != NULL && mode != NULL, NULL);

	return fopen(file_name, mode);
}

int  ftk_file_seek(FtkFsHandle file, size_t pos)
{
	return fseek(file, SEEK_SET, pos);
}

int  ftk_file_read(FtkFsHandle file, void* buffer, size_t length)
{
	return fread(buffer, 1, length, file);
}

int  ftk_file_write(FtkFsHandle file, const void* buffer, size_t length)
{
	return fwrite(buffer, 1, length, file);
}

void ftk_file_close(FtkFsHandle file)
{
	return_if_fail(file != NULL);

	fclose(file);
}

FtkFsHandle ftk_dir_open(const char* dir_name)
{
	return_val_if_fail(dir_name != NULL, NULL);

	return opendir(dir_name);
}

Ret  ftk_dir_read(FtkFsHandle dir, FtkFileInfo* info)
{
	struct dirent* ent = NULL;
	return_val_if_fail(dir != NULL && info != NULL, RET_FAIL);

	ent = readdir(dir);

	if(ent == NULL)
	{
		return RET_EOF;
	}

#ifdef DT_DIR
	if (ent->d_type != DT_UNKNOWN)
	{
		info->is_dir = ent->d_type & DT_DIR;
	}
	else
#endif
	{
		struct stat st = {0};
		stat(ent->d_name, &st);
		info->is_dir = S_ISDIR(st.st_mode);
	}

	ftk_strncpy(info->name, ent->d_name, FTK_MAX_PATH);

	return RET_OK;
}

void ftk_dir_close(FtkFsHandle dir)
{
	if(dir != NULL)
	{
		closedir(dir);
	}
}

Ret ftk_fs_get_cwd(char cwd[FTK_MAX_PATH+1])
{
	return_val_if_fail(cwd != NULL, RET_FAIL);

	ftk_getcwd(cwd, FTK_MAX_PATH);
	cwd[FTK_MAX_PATH] = '\0';

	return RET_OK;
}

Ret ftk_fs_delete_dir(const char* dir)
{
	return_val_if_fail(dir != NULL, RET_FAIL);

	return rmdir(dir) == 0 ? RET_OK : RET_FAIL;
}

Ret ftk_fs_delete_file(const char* file_name)
{
	return_val_if_fail(file_name != NULL, RET_FAIL);

	return unlink(file_name) == 0 ? RET_OK : RET_FAIL;
}

Ret ftk_fs_create_dir(const char* dir)
{
	char* p = NULL;
	FtkFileInfo info = {0};
	char name[FTK_MAX_PATH+1] = {0};
	return_val_if_fail(dir != NULL, RET_FAIL);

	strncpy(name, dir, FTK_MAX_PATH);
	ftk_normalize_path(name);
	p = name;
	while(1)
	{
		p = strchr(p + 1, FTK_PATH_DELIM);
		if(p != NULL)
		{
			*p = '\0';
		}
		
		if(ftk_file_get_info(name, &info) == RET_OK)
		{
			if(!info.is_dir)
			{
				return RET_EXIST;
			}
		}
		else
		{
			if(mkdir(name, 0770) != 0) break;
		}

		if(p != NULL)
		{
			*p = FTK_PATH_DELIM;
		}
		else
		{
			break;
		}
	}
	
	return RET_OK;
}

Ret ftk_fs_change_dir(const char* dir)
{
	return_val_if_fail(dir != NULL, RET_FAIL);

	return chdir(dir) == 0 ? RET_OK : RET_FAIL;
}

Ret ftk_fs_move(const char* dir_from, const char* dir_to)
{
	return_val_if_fail(dir_from != NULL && dir_to != NULL, RET_FAIL);

	return rename(dir_from, dir_to) == 0 ? RET_OK : RET_FAIL;
}

int ftk_fs_is_root(const char* path)
{
	return_val_if_fail(path != NULL, 0);

	return path[0] == FTK_PATH_DELIM && path[1] == '\0';
}
