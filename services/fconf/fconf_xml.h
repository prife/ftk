#ifndef FCONF_XML_H
#define FCONF_XML_H

#include "fconf.h"

FTK_BEGIN_DECLS

FConf* fconf_xml_create(const char* dir);
Ret fconf_xml_save(FConf* thiz);

FTK_END_DECLS

#endif/*FCONF_XML_H*/

