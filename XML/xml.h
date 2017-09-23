#ifndef XML_H
#define XML_H
#define X xml_X
#define Xattr xml_attr_X
#define Xcont xml_content_X
#define Xparser xml_parser_X

#include <stdio.h>
#include "list.h"

#define CUSTOM

#ifndef  CUSTOM

#define PRINTTAGSTART "<%s"
#define PRINTTAGEND   ">\n"
#define PRINTTAGCLOSE "</%s>\n"
#define PRINTATTR     "%s=\"%s\""
#define PRINTCONT     "%s\n"

#else

#define PRINTTAGSTART "TAGSTART(%s)"
#define PRINTTAGEND   "\n"
#define PRINTTAGCLOSE "TAGCLOSE(%s)\n"
#define PRINTATTR     "ATTR(%s=\"%s\")"
#define PRINTCONT     "CONTENT(%s)\n"

#endif

struct X;
typedef struct X *X;
struct Xattr;
typedef struct Xattr *Xattr;

struct Xcont;
typedef struct Xcont *Xcont;

struct Xparser;
typedef struct Xparser *Xparser;


Xcont   xml_parsexmlcont(Xparser parser);
X       xml_parserxml(Xparser parser);
Xattr   xml_parsexmlattr(Xparser parser);
void    xml_parserconsume(Xparser parser, int tok);
int     xml_parsermatches(Xparser parser, int tok);
X       xml_parse(Xparser parser);
Xparser xml_parser(FILE *inp, FILE *err);
Xcont   xml_contentnode(X xml_node);
Xcont   xml_contentstr(char *content);
Xattr   xml_attr(char *key, char *value);
X       xml_node(char *name, list_L attrs, list_L content);
void xml_print(FILE *fp, X xml);



#undef X
#endif
