#include <stdio.h>
#include "fconf_xml.h"

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
	return_val_if_fail(name != NULL && value != NULL, NULL);

	node = FTK_ZALLOC(sizeof(XmlNode));

	if(node != NULL)
	{
		node->name = ftk_strdup(name);
		if(node->name != NULL)
		{
			node->value = ftk_strdup(value);
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
	return_val_if_fail(node != NULL && value != NULL, RET_FAIL);

	FTK_FREE(node->value);
	node->value = ftk_strdup(value);

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

static const char* xpath_next(const char* xpath)
{
	const char* next = NULL;
	return_val_if_fail(xpath != NULL, NULL);

	next = strchr(xpath, '/');
	if(next != NULL)
	{
		next++;
	}

	return next;
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

static int xpath_match(const char* xpath, const char* name)
{
	const char* end = NULL;
	const char* start = xpath;
	return_val_if_fail(xpath != NULL && name != NULL, 0);

	for(start = xpath; *start == '/'; start++);
	for(end = start; *end != '/' && *end != '\0'; end++);

	return strncmp(start, name, end - start - 1);
}

static XmlNode* xml_node_find(XmlNode* node, const char* xpath)
{
	return_val_if_fail(node != NULL && xpath != NULL, NULL);

	if(xpath_match(xpath, node->name))
	{
		const char* next = xpath_next(xpath);
		if(next == NULL || *next == '\0')
		{
			return node;
		}
		else
		{
			if(node->children != NULL)
			{
				return xml_node_find(node, next);
			}
		}
	}
	else
	{
		return xml_node_find(node->next, xpath);
	}

	return NULL;
}

static Ret xml_node_add(XmlNode* node, const char* xpath, const char* value)
{
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
	return RET_OK;
}

static XmlNode* xml_node_append_sibling(XmlNode* node, const char* name, const char* value)
{
	XmlNode* iter = NULL;
	XmlNode* sibling = NULL;
	return_val_if_fail(node != NULL && name != NULL && value != NULL, NULL);

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

typedef struct _PrivInfo
{
	XmlNode* root;
	char* root_path;
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

	return RET_OK;
}

Ret fconf_xml_load(FConf* thiz, const char* dir)
{
	return RET_OK;
}

Ret fconf_xml_remove(FConf* thiz, const char* xpath)
{
	XmlNode* node = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && priv->root != NULL && xpath != NULL, RET_FAIL);

	if((node = xml_node_find(priv->root, xpath)) != NULL)
	{
		if(priv->root == node)
		{
			priv->root = node->next;
		}

		xml_node_destroy(node);
	}

	return RET_OK;
}

Ret fconf_xml_get(FConf* thiz, const char* xpath, const char** value)
{
	XmlNode* node = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && priv->root != NULL && xpath != NULL && value != NULL, RET_FAIL);

	if((node = xml_node_find(priv->root, xpath)) != NULL)
	{
		*value = node->value;
	}
	else
	{
		*value = NULL;
	}

	return *value != NULL ? RET_OK : RET_FAIL;
}

Ret fconf_xml_set(FConf* thiz, const char* xpath, const char* value)
{
	Ret ret = RET_FAIL;
	XmlNode* node = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && priv->root != NULL && xpath != NULL && value != NULL, RET_FAIL);

	if((node = xml_node_find(priv->root, xpath)) != NULL)
	{
		ret = xml_node_set_value(node, value);
	}
	else
	{
		ret = xml_node_add(priv->root, xpath, value);
	}

	return ret;
}

Ret fconf_xml_get_child_count(FConf* thiz, const char* xpath, int* nr)
{
	Ret ret = RET_FAIL;
	XmlNode* node = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && priv->root != NULL && xpath != NULL && nr != NULL, RET_FAIL);

	if((node = xml_node_find(priv->root, xpath)) != NULL)
	{
		ret = RET_OK;
		*nr = xml_node_get_child_count(node);
	}

	return ret;
}

Ret fconf_xml_get_child(FConf* thiz, const char* xpath, size_t index, const char** child)
{
	Ret ret = RET_FAIL;
	XmlNode* node = NULL;
	DECL_PRIV(thiz, priv);
	return_val_if_fail(thiz != NULL && priv->root != NULL && child != NULL, RET_FAIL);

	if((node = xml_node_find(priv->root, xpath)) != NULL)
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
	return RET_OK;
}

static Ret fconf_xml_unlock(FConf* thiz)
{
	return RET_OK;
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
		FTK_FREE(priv->root_path);
		FTK_FREE(thiz);
	}

	return;
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
		thiz->destroy = fconf_xml_destroy;

		priv->root_path = ftk_strdup(dir);
		fconf_xml_load(thiz, dir);
	}

	return thiz;
}

#ifdef FCONF_XML_TEST
int main(int argc, char* argv[])
{
	return 0;
}
#endif
