/*
 * File: ftk_config.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief: configs
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
 * 2010-03-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ftk_mmap.h"
#include "ftk_config.h"
#include "ftk_allocator.h"
#include "ftk_xml_parser.h"

struct _FtkConfig
{
	char theme[32];
	int enable_cursor;
	int enable_status_bar;
	char data_dir[FTK_MAX_PATH+1];
	char data_root_dir[FTK_MAX_PATH+1];
	char test_data_dir[FTK_MAX_PATH+1];
};

FtkConfig* ftk_config_create()
{
	FtkConfig* thiz = FTK_ZALLOC(sizeof(FtkConfig));
	
	if(thiz != NULL)
	{
		strcpy(thiz->theme, "default");
		if(ftk_mmap_exist(DATA_DIR))
		{
			strcpy(thiz->data_dir, DATA_DIR);
		}
		else
		{
			strcpy(thiz->data_dir, LOCAL_DATA_DIR);
		}
		strcpy(thiz->data_root_dir, FTK_DATA_ROOT);
		strcpy(thiz->test_data_dir, TESTDATA_DIR);

		thiz->enable_status_bar = 1;
	}

	return thiz;
}

static Ret  ftk_config_show_help(FtkConfig* thiz)
{
	return RET_OK;
}

static FtkBool ftk_path_is_abs(const char* path)
{
	const char* p = NULL;
	return_val_if_fail(path != NULL, FTK_FALSE);

	if(path[0] == '/')
	{
		return FTK_TRUE;
	}

	p = strstr(path, ":\\");
	if(p != NULL && (p - path) == 1)
	{
		return FTK_TRUE;
	}

	return FTK_FALSE;
}

Ret ftk_config_load_file(FtkConfig* thiz, const char* filename)
{
	FtkMmap* m = NULL;
	m = ftk_mmap_create(filename, 0, -1);
	if(m != NULL)
	{
		const char* data = ftk_mmap_data(m);
		size_t length = ftk_mmap_length(m);
		ftk_config_parse(thiz, 	data, length);
		ftk_mmap_destroy(m);
	}
	
	ftk_logd("%s:%d load %s %s\n", __func__, __LINE__, filename, m != NULL ? "ok" : "fail");

	return RET_OK;
}

Ret ftk_config_load(FtkConfig* thiz, const char* progname)
{
	char path[FTK_MAX_PATH+1] = {0};
	char cwd[FTK_MAX_PATH+1] = {0};
	char filename[FTK_MAX_PATH+1] = {0};
	const char* base = progname + strlen(progname);

	while(base > progname)
	{
		if(*base == '/' || *base == '\\')
		{
			break;
		}
		base--;
	}

	if(base > progname)
	{
		ftk_strncpy(path, progname, base-progname);
	}

	if(ftk_path_is_abs(progname))
	{
		ftk_snprintf(filename, sizeof(filename), "%s/ftk.cnf", path);
	}
	else
	{
		ftk_getcwd(cwd, sizeof(cwd));
		ftk_snprintf(filename, sizeof(filename), "%s/%s/ftk.cnf", cwd, path);
	}

	if(!ftk_mmap_exist(filename))
	{
		return RET_FAIL;
	}

	return ftk_config_load_file(thiz, filename);
}

Ret  ftk_config_init(FtkConfig* thiz, int argc, char* argv[])
{
	int i = 0;
	return_val_if_fail(thiz != NULL && argv != NULL, RET_OK);

#ifdef FTK_CNF
	if(ftk_mmap_exist(FTK_CNF))
	{
		ftk_config_load_file(thiz, FTK_CNF);
	}
#endif
	ftk_config_load(thiz, argv[0]);
	for(i = 0; i < argc && argv != NULL && argv[i] != NULL; i++)
	{
		if(strcmp(argv[i], "--disable-statusbar") == 0)
		{
			ftk_config_set_enable_status_bar(thiz, 0);
			continue;
		}
		else if(strcmp(argv[i], "--enable-cursor") == 0)
		{
			ftk_config_set_enable_cursor(thiz, 1);
			continue;
		}
		else if(strncmp(argv[i], "--theme=", 8) == 0)
		{
			ftk_config_set_theme(thiz, argv[i]+8);
			continue;
		}
		else if(strncmp(argv[i], "--data-dir=", 11) == 0)
		{
			ftk_config_set_data_dir(thiz, argv[i]+11);
			continue;
		}
		else if(strncmp(argv[i], "--test-data-dir=", 16) == 0)
		{
			ftk_config_set_test_data_dir(thiz, argv[i]+16);
			continue;
		}
		else if(strncmp(argv[i], "--help", 6) == 0 || strncmp(argv[i], "-h", 2) == 0)
		{
			ftk_config_show_help(thiz);
			continue;
		}
	}

	return RET_OK;
}

typedef struct _PrivInfo
{
	FtkConfig* config;
}PrivInfo;

static int ftk_str2bool(const char* str)
{
	if(str == NULL || str[0] == '0' || strcmp(str, "false") == 0 || strcmp(str, "no") == 0)
	{
		return 0;
	}

	return 1;
}

static void ftk_config_builder_on_start(FtkXmlBuilder* thiz, const char* tag, const char** attrs)
{
	DECL_PRIV(thiz, priv);
	return_if_fail(thiz != NULL && tag != NULL && attrs != NULL);

	if(strcmp(tag, "statusbar") == 0)
	{
		ftk_config_set_enable_status_bar(priv->config, ftk_str2bool(attrs[1]));
		return;
	}
	else if(strcmp(tag, "cursor") == 0)
	{
		ftk_config_set_enable_cursor(priv->config, ftk_str2bool(attrs[1]));
		return;
	}
	else if(strcmp(tag, "theme") == 0)
	{
		ftk_config_set_theme(priv->config, attrs[1]);
		return;
	}
	else if(strcmp(tag, "data_dir") == 0)
	{
		ftk_config_set_data_dir(priv->config, attrs[1]);
		return;
	}
	else if(strcmp(tag, "test_data_dir") == 0)
	{
		ftk_config_set_test_data_dir(priv->config, attrs[1]);
		return;
	}

	return;
}

static void ftk_config_builder_on_end(FtkXmlBuilder* thiz, const char* tag)
{
	return;
}

static void ftk_config_builder_destroy(FtkXmlBuilder* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(FtkXmlBuilder) + sizeof(PrivInfo));
	}

	return;
}

static FtkXmlBuilder* ftk_config_builder_create(void)
{
	FtkXmlBuilder* thiz = FTK_ZALLOC(sizeof(FtkXmlBuilder) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		thiz->on_start_element = ftk_config_builder_on_start;
		thiz->on_end_element   = ftk_config_builder_on_end;
		thiz->destroy          = ftk_config_builder_destroy;
	}

	return thiz;
}

Ret  ftk_config_parse(FtkConfig* thiz, const char* xml, int length)
{
	FtkXmlParser* parser = NULL;
	FtkXmlBuilder* builder = NULL;
	return_val_if_fail(xml != NULL, RET_FAIL);

	parser  = ftk_xml_parser_create();
	return_val_if_fail(parser != NULL, RET_FAIL);
	
	builder = ftk_config_builder_create();
	if(builder != NULL)
	{
		PrivInfo* priv = (PrivInfo*)builder->priv;
		priv->config = thiz;
		ftk_xml_parser_set_builder(parser, builder);
		ftk_xml_parser_parse(parser, xml, strlen(xml));
	}
	ftk_xml_builder_destroy(builder);
	ftk_xml_parser_destroy(parser);

	return RET_OK;
}

const char* ftk_config_get_theme(FtkConfig* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->theme;
}

const char* ftk_config_get_data_dir(FtkConfig* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->data_dir;
}

const char* ftk_config_get_test_data_dir(FtkConfig* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->test_data_dir;
}

const char* ftk_config_get_data_root_dir(FtkConfig* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->data_root_dir;
}

int ftk_config_get_enable_cursor(FtkConfig* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->enable_cursor;
}

int ftk_config_get_enable_status_bar(FtkConfig* thiz)
{
	return_val_if_fail(thiz != NULL, 1);

	return thiz->enable_status_bar;
}

Ret ftk_config_set_theme(FtkConfig* thiz, const char* theme)
{
	return_val_if_fail(thiz != NULL && theme != NULL, RET_FAIL);

	ftk_strncpy(thiz->theme, theme, sizeof(thiz->theme));

	return RET_OK;
}

Ret ftk_config_set_data_dir(FtkConfig* thiz, const char* data_dir)
{
	return_val_if_fail(thiz != NULL && data_dir != NULL, RET_FAIL);
	
	ftk_strncpy(thiz->data_dir, data_dir, sizeof(thiz->data_dir));

	return RET_OK;
}

Ret ftk_config_set_test_data_dir(FtkConfig* thiz, const char* test_data_dir)
{
	return_val_if_fail(thiz != NULL && test_data_dir != NULL, RET_FAIL);

	ftk_strncpy(thiz->test_data_dir, test_data_dir, sizeof(thiz->test_data_dir));

	return RET_OK;
}

Ret ftk_config_set_enable_cursor(FtkConfig* thiz, int enable_cursor)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	thiz->enable_cursor = enable_cursor;

	return RET_OK;
}

Ret ftk_config_set_enable_status_bar(FtkConfig* thiz, int enable_status_bar)
{
	return_val_if_fail(thiz != NULL, RET_FAIL);

	thiz->enable_status_bar = enable_status_bar;

	return RET_OK;
}

void ftk_config_destroy(FtkConfig* thiz)
{
	if(thiz != NULL)
	{
		FTK_ZFREE(thiz, sizeof(FtkConfig));
	}

	return;
}


