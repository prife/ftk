#include <stdio.h>
#include "ftk_path.h"
#include "fconf_xml.h"
#include "ftk_allocator.h"

typedef enum _XmlNodeAttr
{
	NODE_ATTR_READONLY = 1,
	NODE_ATTR_MODIFIED = 2
}XmlNodeAttr;

typedef struct _XmlNode
{
	char* name;
	char* value;
	unsigned int attr;
	struct _XmlNode* next;
	struct _XmlNode* prev;
	struct _XmlNode* parent;
	struct _XmlNode* children;
}XmlNode;

static XmlNode* xml_node_create(const char* name, const char* value)
{
	XmlNode* node = NULL;
	return_val_if_fail(name != NULL, NULL);

	node = FTK_ZALLOC(sizeof(XmlNode));

	if(node != NULL)
	{
		node->name = ftk_strdup(name);
		if(node->name != NULL)
		{
			if(value != NULL)
			{
				node->value = ftk_strdup(value);
			}
		}
		else
		{	
			FTK_FREE(node);
		}
	}

	return node;
}

static Ret xml_node_set_value(XmlNode* node, const char* value)
{
	return_val_if_fail(node != NULL, RET_FAIL);
	return_val_if_fail((node->attr & NODE_ATTR_READONLY) == 0, RET_FAIL);

	FTK_FREE(node->value);
	if(value != NULL)
	{
		node->value = ftk_strdup(value);
	}

	return RET_OK;
}

static Ret xml_node_set_readonly(XmlNode* node, int readonly)
{
	return_val_if_fail(node != NULL, RET_FAIL);

	if(readonly)
	{
		node->attr |= NODE_ATTR_READONLY;
	}
	else
	{
		node->attr &= ~NODE_ATTR_READONLY;
	}

	return RET_OK;
}

static Ret xml_node_set_modified(XmlNode* node, int modified)
{
	return_val_if_fail(node != NULL, RET_FAIL);

	if(modified)
	{
		node->attr |= NODE_ATTR_MODIFIED;
	}
	else
	{
		node->attr &= ~NODE_ATTR_MODIFIED;
	}

	return RET_OK;
}

static int xml_node_get_child_count(XmlNode* node)
{
	int nr = 0;
	XmlNode* iter = NULL;
	return_val_if_fail(node != NULL, 0);

	for(iter = node->children; iter != NULL; iter = iter->next)
	{
		nr++;
	}

	return nr;
}

static XmlNode* xml_node_get_child(XmlNode* node, size_t index)
{
	XmlNode* iter = NULL;
	return_val_if_fail(node != NULL, 0);

	for(iter = node->children; iter != NULL; iter = iter->next)
	{
		if(index == 0)
		{
			return iter;
		}
		else
		{
			index--;
		}
	}	

	return NULL;
}

static XmlNode* xml_node_find(XmlNode* node, FtkPath* path)
{
	return_val_if_fail(node != NULL && node->name != NULL && path != NULL, NULL);

	for(; node != NULL; node = node->next)
	{
		if(strcmp(node->name, ftk_path_current(path)) == 0)
		{
			if(ftk_path_is_leaf(path))
			{
				return node;
			}
			else if(node->children != NULL)
			{
				ftk_path_down(path);
				return xml_node_find(node->children, path);
			}
			else
			{
				return NULL;
			}
		}
	}

	return NULL;
}

static XmlNode* xml_node_append_sibling(XmlNode* node, const char* name, const char* value)
{
	XmlNode* iter = NULL;
	XmlNode* sibling = NULL;
	return_val_if_fail(node != NULL && name != NULL, NULL);

	if((sibling = xml_node_create(name, value)) != NULL)
	{
		for(iter = node; iter->next != NULL; iter = iter->next)
		{
		}
		
		iter->next = sibling;
		sibling->prev = iter;
		sibling->parent = node->parent;
	}

	return sibling;
}

static Ret xml_node_add(XmlNode* node, FtkPath* path, const char* value)
{
	XmlNode* iter = node;
	return_val_if_fail(node != NULL && node->name != NULL && path != NULL, RET_FAIL);

	for(; iter != NULL; iter = iter->next)
	{
		if(strcmp(iter->name, ftk_path_current(path)) == 0)
		{
			if(ftk_path_is_leaf(path))
			{
				xml_node_set_value(iter, value);
			}
			else
			{
				ftk_path_down(path);
				if(iter->children == NULL)
				{
					iter->children = xml_node_create(ftk_path_current(path), NULL);
					iter->children->parent = iter;
				}
				xml_node_add(iter->children, path, value);
			}
			
			return RET_OK;
		}
	}

	/*FIXME*/
	if(ftk_path_is_leaf(path))
	{
		iter = xml_node_append_sibling(node, ftk_path_current(path), value);
	}
	else
	{
		iter = xml_node_append_sibling(node, ftk_path_current(path), NULL);
		return xml_node_add(iter, path, value);
	}

	return RET_OK;
}

static void xml_node_destroy(XmlNode* node)
{
	XmlNode* iter = NULL;
	XmlNode* temp = NULL;
	if(node != NULL)
	{
		iter = node->children;
		while(iter != NULL)
		{
			temp = iter->next;
			xml_node_destroy(iter);
			iter = temp;
		}

		if(node->parent != NULL)
		{
			if(node->parent->children == node)
			{
				node->parent->children = node->next;
			}
		}

		if(node->prev != NULL)
		{
			node->prev->next = node->next;
		}

		if(node->next != NULL)
		{
			node->next->prev = node->prev;
		}

		FTK_ZFREE(node, sizeof(XmlNode));
	}

	return;
}

static Ret xml_node_save(XmlNode* node, FILE* fp)
{
	return_val_if_fail(node != NULL && fp != NULL, RET_FAIL);

	if(node->children == NULL)
	{
		fprintf(fp, "<%s value=\"%s\" readonly=\"%d\"/>\n", 
			node->name, node->value, node->attr & NODE_ATTR_READONLY ? 1 : 0);
	}
	else
	{
		XmlNode* iter = NULL;
		fprintf(fp, "<%s>\n", node->name);
		for(iter = node->children; iter != NULL; iter = iter->next)
		{
			xml_node_save(iter, fp);
		}
		fprintf(fp, "</%s>\n", node->name);
	}

	return RET_OK;
}

typedef struct _PrivInfo
{
	int modified;
	XmlNode* root;
	char* root_path;
	FtkPath* path;
	FConfOnChanged on_changed;
	void* on_changed_ctx;
}PrivInfo;

Ret fconf_xml_save(FConf* thiz)
{
	FILE* fp = NULL;
	XmlNode* iter = NULL;
	DECL_PRIV(thiz, priv);
	char filename[FTK_MAX_PATH+1] = {0};
	return_val_if_fail(thiz != NULL && priv->root != NULL, RET_FAIL);	

	for(iter = priv->root; iter != NULL; iter = iter->next)
	{
		if(iter->attr & NODE_ATTR_MODIFIED)
		{
			ftk_snprintf(filename, FTK_MAX_PATH, "%s/%s.cnf", priv->root_path, iter->name);
			if((fp = fopen(filename, "w+")) != NULL)
			{
				xml_node_save(iter, fp);
				xml_node_set_modified(iter, 0);
				fclose(fp);
			}
		}
	}

	priv->modified = 0;
	ftk_logd("%s: done\n", __func__);

	return RET_OK;
}

Ret fconf_xml_load(FConf* thiz, const char* dir)
{
	return RET_OK;
}

static Ret fconf_xml_reg_changed_notify(FConf* thiz, FConfOnChanged on_changed, void* ctx)
{
	DECL_PRIV(thiz, priv);

	priv->on_changed = on_changed;
	priv->on_changed_ctx = ctx;

	return RET_OK;
}

static Ret fconf_xml_on_changed(FConf* thiz, FConfChangeType change_type, const char* value)
{
	XmlNode* iter = NULL;
	DECL_PRIV(thiz, priv);

	for(iter = priv->root; iter != NULL; iter = iter->next)
	{
		if(strcmp(iter->name, ftk_path_current(priv->path)) == 0)
		{
			xml_node_set_modified(iter, 1);
			break;
		}
	}

	if(priv->on_changed != NULL)
	{
		priv->on_changed(priv->on_changed_ctx, 1, change_type, ftk_path_full(priv->path), value);
	}

	priv->modified = 1;

	return RET_OK;
}


Ret fconf_xml_remove(FConf* thiz, const char* path)
{
	Ret ret = RET_FAIL;
	XmlNode* node = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && priv->root != NULL && path != NULL, RET_FAIL);

	ftk_path_set_path(priv->path, path);
	if((node = xml_node_find(priv->root, priv->path)) != NULL)
	{
		if(priv->root == node)
		{
			priv->root = node->next;
		}

		xml_node_destroy(node);
		ret = RET_OK;
		fconf_xml_on_changed(thiz, FCONF_CHANGED_BY_REMOVE, NULL);
	}

	return ret;
}

Ret fconf_xml_get(FConf* thiz, const char* path, char** value)
{
	XmlNode* node = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && priv->root != NULL && path != NULL && value != NULL, RET_FAIL);

	ftk_path_set_path(priv->path, path);
	if((node = xml_node_find(priv->root, priv->path)) != NULL)
	{
		*value = node->value;
	}
	else
	{
		*value = NULL;
	}

	return *value != NULL ? RET_OK : RET_FAIL;
}

Ret fconf_xml_set(FConf* thiz, const char* path, const char* value)
{
	Ret ret = RET_FAIL;
	XmlNode* node = NULL;
	DECL_PRIV(thiz, priv);
	FConfChangeType type = FCONF_CHANGED_BY_SET;
	return_val_if_fail(thiz != NULL && path != NULL && value != NULL, RET_FAIL);

	ftk_path_set_path(priv->path, path);
	if(priv->root == NULL)
	{
		type = FCONF_CHANGED_BY_ADD;
		if(ftk_path_is_leaf(priv->path))
		{
			priv->root = xml_node_create(ftk_path_current(priv->path), value);
		}
		else
		{
			priv->root = xml_node_create(ftk_path_current(priv->path), NULL);
			ret = xml_node_add(priv->root, priv->path, value);
		}
	}
	else
	{
		if((node = xml_node_find(priv->root, priv->path)) != NULL)
		{
			ret = xml_node_set_value(node, value);
		}
		else
		{
			ftk_path_root(priv->path);
			type = FCONF_CHANGED_BY_ADD;
			ret = xml_node_add(priv->root, priv->path, value);
		}
	}

	ftk_path_root(priv->path);

	if(ret == RET_OK)
	{
		fconf_xml_on_changed(thiz, type, value);
	}

	return ret;
}

Ret fconf_xml_get_child_count(FConf* thiz, const char* path, int* nr)
{
	Ret ret = RET_FAIL;
	XmlNode* node = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && priv->root != NULL && path != NULL && nr != NULL, RET_FAIL);

	ftk_path_set_path(priv->path, path);
	if((node = xml_node_find(priv->root, priv->path)) != NULL)
	{
		ret = RET_OK;
		*nr = xml_node_get_child_count(node);
	}

	return ret;
}

Ret fconf_xml_get_child(FConf* thiz, const char* path, int index, char** child)
{
	Ret ret = RET_FAIL;
	XmlNode* node = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && priv->root != NULL && path != NULL && child != NULL, RET_FAIL);

	ftk_path_set_path(priv->path, path);
	if((node = xml_node_find(priv->root, priv->path)) != NULL)
	{
		if((node = xml_node_get_child(node, index)) != NULL)
		{
			ret = RET_OK;
			*child = node->name;
		}
	}

	return ret;
}

static Ret fconf_xml_lock(FConf* thiz)
{
	return RET_FAIL;
}

static Ret fconf_xml_unlock(FConf* thiz)
{
	return RET_FAIL;
}

void fconf_xml_destroy(FConf* thiz)
{
	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);
		XmlNode* temp = NULL;
		XmlNode* iter = priv->root;
		
		fconf_xml_save(thiz);
		while(iter != NULL)
		{
			temp = iter->next;
			xml_node_destroy(iter);
			iter = temp;
		}
		ftk_path_destroy(priv->path);
		FTK_FREE(priv->root_path);
		FTK_FREE(thiz);
	}

	return;
}

int fconf_xml_is_dirty(FConf* thiz)
{
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL, 0);

	return priv->modified;
}

FConf* fconf_xml_create(const char* dir)
{
	FConf* thiz = NULL;
	
	return_val_if_fail(dir != NULL, NULL);
	thiz = FTK_ZALLOC(sizeof(FConf) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		DECL_PRIV(thiz, priv);

		thiz->lock = fconf_xml_lock;
		thiz->unlock = fconf_xml_unlock;
		thiz->remove = fconf_xml_remove;
		thiz->set = fconf_xml_set;
		thiz->get = fconf_xml_get;
		thiz->get_child_count = fconf_xml_get_child_count;
		thiz->get_child = fconf_xml_get_child;
		thiz->reg_changed_notify = fconf_xml_reg_changed_notify;
		thiz->destroy = fconf_xml_destroy;

		priv->path = ftk_path_create(NULL);
		priv->root_path = ftk_strdup(dir);
		fconf_xml_load(thiz, dir);
	}

	return thiz;
}

#ifdef FCONF_XML_TEST
#include <assert.h>
#include "fconf.c"
#include "ftk_allocator_default.h"

int main(int argc, char* argv[])
{
	FConf* thiz = NULL;
#ifndef USE_STD_MALLOC
	ftk_set_allocator((ftk_allocator_default_create()));
#endif
	thiz = fconf_xml_create("./config");
	fconf_test(thiz);
	fconf_xml_save(thiz);
	fconf_destroy(thiz);

	return 0;
}
#endif
