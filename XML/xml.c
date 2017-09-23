#include "xml.h"
#include "filebuffer.h"
#include "lex.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define X xml_X
#define Xattr xml_attr_X
#define Xcont xml_content_X
#define Xparser xml_parser_X

struct Xattr;
typedef struct Xattr *Xattr;

struct Xcont;
typedef struct Xcont *Xcont;

struct Xparser;
typedef struct Xparser *Xparser;

struct xml_parser_X {
	FILE *err_fp;
	lexer_L lexer;
	filebuffer_F buffer;
	int tok;
};



struct xml_X {
	char *name;
	list_L attrs;
	list_L content;
};

struct xml_attr_X {
	char *key;
	char *value;
};

struct xml_content_X {
	enum xml_content_type {
		CONTENT_NODE,
		XML_NODE
	} type;
	union {
		xml_X xml_node;
		char *content_node;
	} value;
};

void xml_xmlcontprint(FILE *fp, Xcont cont, int offset);
void xml_xmlattrprint(FILE *fp, Xattr attr);
void xml_xmlprint(FILE *fp, X xml, int offset);
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


X xml_node(char *name, list_L attrs, list_L content) {
	assert(name);
	X xml_node;
	xml_node = malloc(sizeof(*xml_node));
	assert(xml_node);
	xml_node->name = name;
	xml_node->attrs = (attrs == NULL ? list_new() : attrs);	
	xml_node->content = (content == NULL ? list_new() : content);
	return xml_node;
}


Xattr xml_attr(char *key, char *value) {
	Xattr xml_attr;
	xml_attr = malloc(sizeof(*xml_attr));
	assert(xml_attr);
	xml_attr->key = key;
	xml_attr->value = value;
	return xml_attr;
}

Xcont xml_contentstr(char *content) {
	Xcont xml_cont;
	xml_cont = malloc(sizeof(*xml_cont));
	assert(xml_cont);
	xml_cont->type = CONTENT_NODE;
	xml_cont->value.content_node = content;
	
	return xml_cont;
}

Xcont xml_contentnode(X xml_node) {
	Xcont xml_cont;
	xml_cont = malloc(sizeof(*xml_cont));
	assert(xml_cont);
	xml_cont->type = XML_NODE;
	xml_cont->value.xml_node = xml_node;
	return xml_cont;
}

Xparser xml_parser(FILE *inp, FILE *err) {
	Xparser parser;
	parser = malloc(sizeof(*parser));
	assert(parser);
	if(inp == NULL) inp = stdin;
	if(err == NULL) err = stderr;

	parser->err_fp = err;
	parser->buffer = filebuffer_new(inp);
	assert(parser->buffer);
	parser->lexer = lexer_new(parser->buffer);
	assert(parser->lexer);
	
	return parser;
}

X    xml_parse(Xparser parser) {
	assert(parser);
	parser->tok = lexer_gettoken(parser->lexer);
	return xml_parserxml(parser);
}
/*
	<html file="kiran">
		<value/>
		Kiran
	</html>	
	------------------
	XML {
		name=html,
		attr {
			key="file"
			name="kiran"
		}
		cont {
			XML {
			 name=value
			},
			"Kiran"
		}
   */
int xml_parsermatches(Xparser parser, int tok) {
	assert(parser);
	if(tok == parser->tok) {
		return 1;
	} else
		return 0;
}

void xml_parserconsume(Xparser parser, int tok) {
	if(xml_parsermatches(parser, tok)) {
		parser->tok = lexer_gettoken(parser->lexer);
	}
}

Xattr xml_parsexmlattr(Xparser parser) {
	assert(parser);
	char *key = NULL;
	char *value = NULL;
	if(xml_parsermatches(parser, ATTRIBUTENAME)){
		key = malloc(sizeof(*key) * 
				lexer_lexemelen(parser->lexer));
		assert(key);
		lexer_lexemecopy(parser->lexer, key,lexer_lexemelen(parser->lexer));
		xml_parserconsume(parser,ATTRIBUTENAME);
		if(xml_parsermatches(parser, EQUALSIGN)) {
			xml_parserconsume(parser, EQUALSIGN);	
			if(xml_parsermatches(parser, ATTRIBUTEVALUE)) {
				value = malloc(sizeof(*value) * lexer_lexemelen(parser->lexer));
				assert(value);
				lexer_lexemecopy(parser->lexer, value, lexer_lexemelen(parser->lexer));
				xml_parserconsume(parser, ATTRIBUTEVALUE);
			}
		}

		Xattr attr = xml_attr(key, value);
		assert(attr);
		return attr;
	} else if(xml_parsermatches(parser, EQUALSIGN)) {
		fprintf(parser->err_fp, "XML_PARSER[%d, %d]: Attribute has no key\n", filebuffer_lineno(parser->buffer), filebuffer_columnno(parser->buffer));
		xml_parserconsume(parser, EQUALSIGN);
		if(xml_parsermatches(parser, ATTRIBUTEVALUE)){
				value = malloc(sizeof(*value) * lexer_lexemelen(parser->lexer));
				assert(value);
				lexer_lexemecopy(parser->lexer, value, lexer_lexemelen(parser->lexer));
				xml_parserconsume(parser, ATTRIBUTEVALUE);
		} else {
			fprintf(parser->err_fp, "XML_PARSER[%d, %d]: Attribute has no value\n", filebuffer_lineno(parser->buffer), filebuffer_columnno(parser->buffer));
		}

		Xattr attr = xml_attr(key, value);
		assert(attr);
		return attr;
	} else if(xml_parsermatches(parser, ATTRIBUTEVALUE)) {
		fprintf(parser->err_fp, "XML_PARSER[%d, %d]: Attribute has no key\n", filebuffer_lineno(parser->buffer), filebuffer_columnno(parser->buffer));
		value = malloc(sizeof(*value) * lexer_lexemelen(parser->lexer));
		assert(value);
		lexer_lexemecopy(parser->lexer, value, lexer_lexemelen(parser->lexer));
		xml_parserconsume(parser, ATTRIBUTEVALUE);
		Xattr attr = xml_attr(key, value);
		assert(attr);
		return attr;
	} else {
		return NULL;
	}
	return NULL;
}

X xml_parserxml(Xparser parser) {
	assert(parser);
	char *name;
	list_L list_attr = NULL;
       	list_L list_cont = NULL;

	if(xml_parsermatches(parser, TAGBEGIN)) {
		list_attr = list_new();
		list_cont = list_new();
		name = malloc(sizeof(*name) * lexer_lexemelen(parser->lexer));
		assert(name);
		lexer_lexemecopy(parser->lexer, name, lexer_lexemelen(parser->lexer));
		xml_parserconsume(parser, TAGBEGIN);
		Xattr attr;
		while((attr = xml_parsexmlattr(parser)) != NULL) 
			list_push(list_attr, attr);	
		if(xml_parsermatches(parser, TAGENDANDCLOSE)) {
			xml_parserconsume(parser, TAGENDANDCLOSE);
			X xml = xml_node(name, list_attr, list_cont);
			assert(xml);
			return xml;
		} else if(xml_parsermatches(parser, TAGEND)) {
			xml_parserconsume(parser,TAGEND);
			Xcont xcont;
			while((xcont = xml_parsexmlcont(parser)) != NULL) 
				list_push(list_cont, xcont);
			if(xml_parsermatches(parser, TAGCLOSE)) {
				char *temp;
				temp = malloc(sizeof(*temp) * lexer_lexemelen(parser->lexer));
				assert(temp);
				lexer_lexemecopy(parser->lexer, temp, lexer_lexemelen(parser->lexer));
				
				char *tmp_name = name;
				char *tmp_tmp = temp;
				int isequal = 1;

				while(isequal && *tmp_tmp != '\0' && *tmp_name != '\0') {
					if(*tmp_tmp++ != *tmp_name++) {
						isequal = 0;
					}
				}
				if(!isequal) {
					fprintf(parser->err_fp, "XML_PARSER[%d, %d]: End tag %s doesn't match %s\n", filebuffer_lineno(parser->buffer), filebuffer_columnno(parser->buffer), temp, name);
				}
				free(temp);
				temp = NULL;
				xml_parserconsume(parser, TAGCLOSE);
				X xml = xml_node(name, list_attr, list_cont);
				assert(xml);
				return xml;
			} else {
					fprintf(parser->err_fp, "XML_PARSER[%d, %d]: %s has no closing tag\n", filebuffer_lineno(parser->buffer), filebuffer_columnno(parser->buffer), name);
					xml_parserconsume(parser, parser->tok);
					X xml = xml_node(name, list_attr, list_cont);
					assert(xml);
					return xml;
			}
		} else {

					fprintf(parser->err_fp, "XML_PARSER[%d, %d]: %s has no closing >\n", filebuffer_lineno(parser->buffer), filebuffer_columnno(parser->buffer), name);
					xml_parserconsume(parser, parser->tok);
					X xml = xml_node(name, list_attr, list_cont);
					assert(xml);
					return xml;
		}
	}

	return NULL;
}

Xcont xml_parsexmlcont(Xparser parser) {
	assert(parser);
	
	if(xml_parsermatches(parser, TAGBEGIN)){
		X xml = xml_parserxml(parser);
		if(xml == NULL) {
			fprintf(parser->err_fp, "XML_PARSER[%d, %d]: Content XML is invalid\n", filebuffer_lineno(parser->buffer), filebuffer_columnno(parser->buffer));
		}
		Xcont cont = xml_contentnode(xml);
		assert(cont);
		return cont;
	} else if(xml_parsermatches(parser, CONTENT)) {
		char *str = malloc(sizeof(*str) * lexer_lexemelen(parser->lexer));
		assert(str);
		lexer_lexemecopy(parser->lexer, str, lexer_lexemelen(parser->lexer));
		Xcont cont = xml_contentstr(str);	
		assert(cont);
		return cont;
	}
	return NULL;
}


void xml_xmlcontprint(FILE *fp, Xcont cont, int offset) {
	if(cont != NULL) {
		if(cont->type == CONTENT_NODE) {
			int i;
			for(i = 0; i < offset; i++) {
				fprintf(fp, " ");
			}
			fprintf(fp, PRINTCONT, cont->value.content_node);
		} else if(cont->type == XML_NODE) {
			xml_xmlprint(fp, cont->value.xml_node, offset);
		}
	}
}

void xml_xmlattrprint(FILE *fp, Xattr attr) {
	if(attr != NULL) {
		fprintf(fp, PRINTATTR, attr->key, attr->value);
	}
}

void xml_xmlprint(FILE *fp, X xml, int offset) {
	if(xml != NULL) {
		int i;
		for(i = 0; i < offset; i++) {
			fprintf(fp, " ");
		}
		fprintf(fp, PRINTTAGSTART, xml->name);
		struct L_iterator iter = list_iterator(xml->attrs);
		
		while(list_iteratorhasnext(&iter)) {
			fprintf(fp, " ");
			Xattr attr = list_iteratornext(&iter);
			xml_xmlattrprint(fp, attr);
		}
		fprintf(fp, PRINTTAGEND);

		iter = list_iterator(xml->content);
		while(list_iteratorhasnext(&iter)) {
			Xcont cont = list_iteratornext(&iter);	
			xml_xmlcontprint(fp, cont, offset + 2);
		}
		for(i = 0; i < offset; i++) {
			fprintf(fp, " ");
		}
		fprintf(fp,PRINTTAGCLOSE, xml->name);
	
	}
}


void xml_print(FILE *fp, X xml) {
	xml_xmlprint(fp, xml, 0);
}
