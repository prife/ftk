#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "ftk_xml_builder.h"

#define MAX_ATTR_NR 32
struct _FtkXmlParser;
typedef struct _FtkXmlParser FtkXmlParser;

FtkXmlParser* ftk_xml_parser_create(void);
void       ftk_xml_parser_set_builder(FtkXmlParser* thiz, FtkXmlBuilder* builder);
void ftk_xml_parser_parse(FtkXmlParser* thiz, const char* xml, size_t length);
void       ftk_xml_parser_destroy(FtkXmlParser* thiz);

#endif/*XML_PARSER_H*/

