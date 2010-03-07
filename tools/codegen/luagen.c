#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libIDL/IDL.h>

typedef struct 
{
	FILE* h;
	FILE* c;
	IDL_ns ns;
	IDL_tree tree;
	char interface[128];
	char interface_lower[256];
	GString* str_types;
	GString* str_funcs;
	GString* str_init;
	gboolean only_globals;
}CodeGenInfo;

static void code_gen_info_reset(CodeGenInfo* info)
{
	if(info->str_types != NULL)
	{
		g_string_free(info->str_types, 1);
		info->str_types = NULL;
	}

	if(info->str_funcs != NULL)
	{
		g_string_free(info->str_funcs, 1);
		info->str_funcs = NULL;
	}

	if(info->str_init != NULL)
	{
		g_string_free(info->str_init, 1);
		info->str_init = NULL;
	}

	if(info->h != NULL && info->h != stdout)
	{
		fclose(info->h);
		info->h = NULL;
	}

	if(info->c != NULL && info->c != stdout)
	{
		fclose(info->c);
		info->c = NULL;
	}

	return;
}

static char* change_func_name(const char* src, char* dst, int skip)
{
	int i = 0;
	int j = 0;

	dst[0] = toupper(src[0]);
	for(i = 1, j = 1; src[i]; i++)
	{
		if(src[i] == '_')
		{
			continue;
		}
		else if(src[i-1] == '_')
		{
			dst[j++] = toupper(src[i]);
		}
		else
		{
			dst[j++] = src[i];
		}
	}
	dst[j] = '\0';

	return dst + skip;
}

static void code_gen_info_init(CodeGenInfo* info, const char* intf)
{
	int i = 0;
	int j = 0;
	char filename[260] = {0};
	const char* name = intf;

	code_gen_info_reset(info);	
	info->str_types = g_string_sized_new(4096);
	info->str_funcs = g_string_sized_new(8192);
	info->str_init = g_string_sized_new(4096);

	strncpy(info->interface, name, sizeof(info->interface));
	for(i = 0, j = 0; name[i] != '\0' && j < sizeof(info->interface_lower); i++, j++)
	{
		if(isupper(name[i]))
		{
			if(i > 0)
			{
				info->interface_lower[j++] = '_';
			}
		}
		info->interface_lower[j] = tolower(name[i]);
	}
	info->interface_lower[j] = '\0';

	snprintf(filename, sizeof(filename), "lua_%s.h", info->interface_lower);
	info->h = fopen(filename, "w+");
	if(info->h == NULL) info->h = stdout;

	snprintf(filename, sizeof(filename), "lua_%s.c", info->interface_lower);
	info->c = fopen(filename, "w+");
	if(info->c == NULL) info->c = stdout;
	
	fprintf(info->c, "#include \"ftk.h\"\n");
	fprintf(info->c, "#include \"%s.h\"\n", info->interface_lower);
	fprintf(info->c, "#include \"tolua++.h\"\n\n");

	return;
}

static gboolean lua_code_gen_begin_interface(const char* name, CodeGenInfo *info)
{
	code_gen_info_init(info, name);

	g_string_append_printf(info->str_init, "int lua_%s_init(lua_State* tolua_S)\n",
		info->interface_lower);
	g_string_append(info->str_init, "{\n");
	g_string_append(info->str_init, "	tolua_open(tolua_S);\n");
	g_string_append(info->str_init, "	tolua_reg_types(tolua_S);\n");
	g_string_append(info->str_init, "	tolua_module(tolua_S, NULL, 0);\n");
	g_string_append_printf(info->str_init,  "	tolua_beginmodule(tolua_S, NULL);\n");
	if(!info->only_globals)
	{
		g_string_append_printf(info->str_init,  "	tolua_cclass(tolua_S,\"%s\", \"%s\", \"\", NULL);\n",
			info->interface, info->interface);
		g_string_append_printf(info->str_init,  "	tolua_beginmodule(tolua_S, \"%s\");\n", info->interface);
	}
	g_string_append(info->str_types, "static void tolua_reg_types (lua_State* tolua_S)\n{\n");

	if(!info->only_globals)
	{
		g_string_append_printf(info->str_types, "	tolua_usertype(tolua_S, \"%s\");\n", info->interface);
	}

	return TRUE;
}

static gboolean lua_code_gen_end_interface(CodeGenInfo *info)
{
	if(info->interface[0])
	{
		if(!info->only_globals)
		{
			g_string_append_printf(info->str_init, "	tolua_endmodule(tolua_S);\n");
		}
		g_string_append_printf(info->str_init, "	tolua_endmodule(tolua_S);\n");
		g_string_append_printf(info->str_init, "\n	return 1;\n");
		g_string_append_printf(info->str_init, "}\n");
		g_string_append(info->str_types, "}\n\n");
	}

	return TRUE;
}

#define STR_LENGTH 256

typedef struct _TypeInfo
{
	char name[STR_LENGTH+1];
	char lua_name[STR_LENGTH+1];
	char check[STR_LENGTH+1];
	char pop[STR_LENGTH+1];
	char push[STR_LENGTH+1];
}TypeInfo;


static void int_type_init(TypeInfo* info)
{
	strcpy(info->name, "int");
	strcpy(info->lua_name, "lua_Number");
	strcpy(info->check    , "tolua_isnumber(tolua_S, %d, 0, &err)");
	strcpy(info->pop, "tolua_tonumber");
	strcpy(info->push, "tolua_pushnumber");

	return;
}

static void ret_type_init(TypeInfo* info)
{
	strcpy(info->name     , "Ret");
	strcpy(info->lua_name , "lua_Number");
	strcpy(info->check    , "tolua_isnumber(tolua_S, %d, 0, &err)");
	strcpy(info->pop      , "tolua_tonumber");
	strcpy(info->push     , "tolua_pushnumber");

	return;
}

static void str_type_init(TypeInfo* info)
{
	strcpy(info->name     , "char*");
	strcpy(info->lua_name , "char*");
	strcpy(info->check    , "tolua_isstring(tolua_S, %d, 0, &err)");
	strcpy(info->pop      , "tolua_tostring");
	strcpy(info->push     , "tolua_pushstring");

	return;
}

static void userdata_type_init(const char* name, TypeInfo* info)
{
	strcpy(info->name     , name);
	strcpy(info->lua_name , "void*");
	strcpy(info->check    , "tolua_isusertype(tolua_S, %d, \"");
	strcat(info->check    ,  name);
	strcat(info->check    , "\", 0, &err)");
	snprintf(info->pop, sizeof(info->pop), "*(%s*)tolua_tousertype", name);
	strcpy(info->push     , "lua_pushlightuserdata");

	return;
}

static void userdata_light_type_init(const char* name, TypeInfo* info)
{
	char* ptr = strstr(name, "Ptr");

	strcpy(info->name     , name);
	strcpy(info->lua_name , "void*");
	strcpy(info->check    , "tolua_isusertype(tolua_S, %d, \"");
	if(ptr != NULL)
	{
		strncat(info->check, name,  ptr - name);
	}
	else
	{
		strcat(info->check    ,  name);
	}
	strcat(info->check    , "\", 0, &err)");
	strcpy(info->pop      , "tolua_tousertype");
	strcpy(info->push     , "lua_pushlightuserdata");

	return;
}

static gboolean is_pointer(const char* str)
{
	const char* ptr = strstr(str, "Ptr");

	return ptr != NULL && strlen(ptr) == 3;
}

static int get_type_info(IDL_tree type, TypeInfo* info)
{
	if(IDL_NODE_IS_TYPE(type))
	{
		switch(IDL_NODE_TYPE(type))
		{
			case IDLN_TYPE_INTEGER:
			{
				int_type_init(info);
				break;
			}
			case IDLN_TYPE_STRING:
			{
				break;
			}
			case IDLN_TYPE_CHAR:
			{
				break;
			}
			case IDLN_TYPE_BOOLEAN:
			{
				break;
			}
			case IDLN_TYPE_FLOAT:
			{
				break;
			}
			default:break;
		}
	}
	else if(IDL_NODE_TYPE(type) == IDLN_IDENT)
	{
		const char* type_str = IDL_IDENT(type).str;
		if(strcmp(type_str, "StrPtr") == 0 || strcmp(type_str, "CStrPtr") == 0) 
		{
			str_type_init(info);
		}
		else if(is_pointer(type_str))
		{
			char* p = NULL;
			userdata_light_type_init(type_str, info);
			p = strstr(info->name, "Ptr");
			strcpy(p, "*");

		}
		else if(strcmp(type_str, "Ret") == 0) 
		{
			ret_type_init(info);
		}
		else if(strcmp(type_str, "int") == 0 
			|| strcmp(type_str, "uint") == 0 
			|| strcmp(type_str, "short") == 0 
			|| strcmp(type_str, "ushort") == 0 
			|| strcmp(type_str, "char") == 0 
			|| strcmp(type_str, "uchar") == 0 
			|| strcmp(type_str, "long") == 0 
			|| strcmp(type_str, "ulong") == 0 ) 
		{
			int_type_init(info);
		}
		else
		{
			userdata_type_init(type_str, info);
		}
	}

	return 0;
}

static gboolean lua_code_gen_on_func_decl(struct _IDL_OP_DCL func, CodeGenInfo *info)
{
	int i = 0;
	char new_name[64] = {0};
	struct _IDL_LIST iter = {0};
	TypeInfo type_info = {0};
	GString* call = g_string_sized_new(512);
	GString* retv = g_string_sized_new(512);
	GString* param_get = g_string_sized_new(512);
	GString* param_decl = g_string_sized_new(512);
	GString* param_check = g_string_sized_new(512);
	const char* func_name = IDL_IDENT(func.ident).str;

	if(func.op_type_spec != NULL)
	{
		get_type_info(func.op_type_spec, &type_info);
		g_string_append_printf(call, "	retv = %s(", func_name);
		g_string_append_printf(param_decl, "	%s retv;\n", type_info.name);
		g_string_append_printf(retv, "	%s(tolua_S, (%s)retv);\n", type_info.push, type_info.lua_name);
	}
	else
	{
		g_string_append_printf(call, "	%s(", func_name);
	}
	
	g_string_append_printf(info->str_funcs, "static int lua_%s(lua_State* tolua_S)\n", func_name);
	g_string_append(info->str_funcs, "{\n");
	
	for (i = 1, iter = IDL_LIST(func.parameter_dcls); iter.data != NULL; iter = IDL_LIST(iter.next), i++) 
	{
		const char* param_name = NULL;
		
		get_type_info(IDL_PARAM_DCL(iter.data).param_type_spec, &type_info);
		param_name = IDL_IDENT(IDL_PARAM_DCL(iter.data).simple_declarator).str;

		if(i > 1)
		{
			g_string_append_printf(param_check, " && ");
			g_string_append(call, ", ");
		}
		g_string_append_printf(call, "%s", param_name);

		g_string_append_printf(param_check, type_info.check, i);
		g_string_append_printf(param_get, "	%s = %s(tolua_S, %d, 0);\n",
				param_name, type_info.pop, i);
		g_string_append_printf(param_decl, "	%s %s;\n", type_info.name, param_name);
	
		if(iter.next == NULL) break;
	}
	g_string_append(call, ");\n");

	g_string_append_printf(info->str_funcs, "	tolua_Error err = {0};\n", param_check->str);
	g_string_append_printf(info->str_funcs, "	int param_ok = %s;\n", param_check->str);
	g_string_append(info->str_funcs, param_decl->str);
	g_string_append(info->str_funcs, "\n	return_val_if_fail(param_ok, 0);\n\n");
	g_string_append(info->str_funcs, param_get->str);
	g_string_append(info->str_funcs, call->str);
	g_string_append(info->str_funcs, retv->str);
	g_string_append_printf(info->str_funcs, "\n	return 1;\n}\n\n", param_check->str);

	g_string_append_printf(info->str_init, "	tolua_function(tolua_S, \"%s\", lua_%s);\n",
		change_func_name(func_name, new_name, strlen(info->interface)), func_name);

	g_string_free(call, 1);
	g_string_free(retv, 1);
	g_string_free(param_decl, 1);
	g_string_free(param_check, 1);
	g_string_free(param_get, 1);

	return FALSE;
}

static gboolean lua_code_gen_on_const(struct _IDL_CONST_DCL constant, CodeGenInfo *info)
{
	if(IDL_NODE_TYPE (constant.const_type) == IDLN_TYPE_INTEGER)
	{
		const char* name = IDL_IDENT(constant.ident).str;
		g_string_append_printf(info->str_types, "	tolua_constant(tolua_S, \"%s\", %s);\n", name, name);
	}

	return FALSE;
}

static void lua_code_gen_get_func(CodeGenInfo *info, const char* name, TypeInfo* type_info, const char* var)
{

	g_string_append_printf(info->str_funcs, "static int tolua_get_%s_%s(lua_State* tolua_S)\n", name, var);
	g_string_append(info->str_funcs, "{\n");
	g_string_append_printf(info->str_funcs, "	%s* thiz = (%s*)  tolua_tousertype(tolua_S, 1, 0);\n", name, name);
	g_string_append(info->str_funcs, "	return_val_if_fail(thiz != NULL, 0);\n");
	g_string_append_printf(info->str_funcs, "	%s(tolua_S, (%s)thiz->%s);\n", type_info->push, type_info->lua_name, var);

	g_string_append(info->str_funcs, "\n	return 1;\n");
	g_string_append(info->str_funcs, "}\n\n");

	return;
}

static void lua_code_gen_set_func(CodeGenInfo *info, const char* name, TypeInfo* type_info, const char* var)
{
	g_string_append_printf(info->str_funcs, "static int tolua_set_%s_%s(lua_State* tolua_S)\n", name, var);
	g_string_append(info->str_funcs, "{\n");
	g_string_append_printf(info->str_funcs, "	%s* thiz = (%s*)  tolua_tousertype(tolua_S, 1, 0);\n", name, name);
	g_string_append(info->str_funcs, "	return_val_if_fail(thiz != NULL, 0);\n");
	g_string_append_printf(info->str_funcs, "	thiz->%s = ((%s) %s(tolua_S, 2, 0));\n", var, type_info->name, type_info->pop);
	g_string_append(info->str_funcs, "\n	return 1;\n");
	g_string_append(info->str_funcs, "}\n\n");

	return;
}

static gboolean lua_code_gen_on_struct(struct _IDL_TYPE_STRUCT s, CodeGenInfo *info)
{
	struct _IDL_LIST iter = {0};
	const char* name = IDL_IDENT(s.ident).str;
	
	if(!info->only_globals)
	{
		return FALSE;
	}
	
	g_string_append_printf(info->str_types, "	tolua_usertype(tolua_S, \"%s\");\n", name);
	
	g_string_append_printf(info->str_init, "	tolua_cclass(tolua_S, \"%s\", \"%s\", \"\", NULL);\n",
			name, name);
	g_string_append_printf(info->str_init, "	tolua_beginmodule(tolua_S, \"%s\");\n", name);
	for (iter = IDL_LIST(s.member_list); iter.data != NULL; iter = IDL_LIST(iter.next)) 
	{
		TypeInfo type_info = {0};
		const char* var = NULL;
		struct _IDL_MEMBER member = IDL_MEMBER(iter.data);

		get_type_info(member.type_spec, &type_info);
		var = IDL_IDENT(IDL_LIST(member.dcls).data).str;

		lua_code_gen_get_func(info, name, &type_info, var);
		lua_code_gen_set_func(info, name, &type_info, var);
		g_string_append_printf(info->str_init, "	tolua_variable(tolua_S, \"%s\",tolua_get_%s_%s, tolua_set_%s_%s);\n",
			var, name, var, name, var);

		if(iter.next == NULL) break;
	}
  	g_string_append_printf(info->str_init, "	tolua_endmodule(tolua_S);\n");

	return FALSE;
}

static gboolean lua_code_gen_on_enum(struct _IDL_TYPE_ENUM e, CodeGenInfo *info)
{
	struct _IDL_LIST iter = {0};
	
	if(!info->only_globals)
	{
		return FALSE;
	}

	for (iter = IDL_LIST(e.enumerator_list); iter.data != NULL; iter = IDL_LIST(iter.next)) 
	{
		const char* name = IDL_IDENT(iter.data).str;
		g_string_append_printf(info->str_types, "	tolua_constant(tolua_S, \"%s\", %s);\n", name, name);

		if(iter.next == NULL) break;
	}

	return FALSE;
}

static gboolean lua_code_gen_on_union(struct _IDL_TYPE_UNION u, CodeGenInfo *info)
{
	return FALSE;
}

static gboolean lua_gen_code(IDL_tree_func_data *tfd, CodeGenInfo *info)
{
	IDL_tree p = tfd->tree;

	if (IDL_NODE_TYPE (p) == IDLN_INTERFACE)
	{
		const char* name = IDL_IDENT(IDL_INTERFACE(p).ident).str;
		return lua_code_gen_begin_interface(name, info);
	}
	else if(IDL_NODE_TYPE (p) == IDLN_OP_DCL)
	{
		return lua_code_gen_on_func_decl(IDL_OP_DCL(p), info);
	}
	else if(IDL_NODE_TYPE (p) == IDLN_CONST_DCL)
	{
		return lua_code_gen_on_const(IDL_CONST_DCL(p), info);
	}
	else if(IDL_NODE_TYPE(p) == IDLN_TYPE_STRUCT)
	{
		return lua_code_gen_on_struct(IDL_TYPE_STRUCT(p), info);
	}
	else if(IDL_NODE_TYPE(p) == IDLN_TYPE_ENUM)
	{
		return lua_code_gen_on_enum(IDL_TYPE_ENUM(p), info);
	}
	else if(IDL_NODE_TYPE(p) == IDLN_TYPE_UNION)
	{
		return lua_code_gen_on_union(IDL_TYPE_UNION(p), info);
	}

	return TRUE;
}

int
main (int argc, char *argv[])
{
	int rv = 0;
	char *filename = NULL;
	IDL_ns ns = {0};
	IDL_tree tree = {0};
	CodeGenInfo info = {0};
	unsigned long parse_flags = 0;

#ifndef G_PLATFORM_WIN32
	{
		extern int __IDL_debug;
		__IDL_debug = argc >= 4 ? TRUE : FALSE; 
	}
#endif

	IDL_check_cast_enable (TRUE);

	if (argc < 2) 
	{
		fprintf (stderr, "usage: %s idl [globals]\n", argv[0]);

		return 1;
	}

	filename = argv[1];

	g_message ("IDL_parse_filename");
	rv = IDL_parse_filename (filename, NULL, NULL, &tree, &ns, parse_flags, IDL_WARNING1);

	if (rv == IDL_ERROR) 
	{
		g_message ("IDL_ERROR");
		exit (1);	
	} 
	else if (rv < 0) 
	{
		perror (filename);
		exit (1);
	}

	info.ns = ns;
	info.tree = tree;
	info.only_globals = argc == 3;

	if(info.only_globals)
	{
		lua_code_gen_begin_interface("FtkTypedef", &info);
	}
	IDL_tree_walk_in_order (tree, (IDL_tree_func) lua_gen_code, &info);
	lua_code_gen_end_interface(&info);	
	if(info.c != NULL)
	{
		fprintf(info.c, "%s", info.str_types->str);
		fprintf(info.c, "%s", info.str_funcs->str);
		fprintf(info.c, "%s", info.str_init->str);
	}
	code_gen_info_reset(&info);

	IDL_ns_free (ns);
	IDL_tree_free (tree);

	return 0;
}
