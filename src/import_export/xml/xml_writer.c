#include <stdio.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <xml_writer.h>
#include <libxml/parser.h>

void add_module(char **attributes, int *status, char *mod_name)
{
   int rc;
   xmlDoc         *document;
   xmlNode        *root, *first_child, *node;
}

void parser_xml(char *filename)
{
    xmlDoc         *document;
    xmlNode        *root, *first_child, *node;

    document = xmlReadFile(filename, NULL, 0);

    root = xmlDocGetRootElement(document);

    printf("Root is <%s> (%i)\n", root->name, root->type);

    first_child = root->children;

    for (node = first_child; node; node = node->next)
    {
        printf("\t Child is <%s> (%i)\n", node->name, node->type);
    }

    printf("...\n");

    xmlFreeDoc(document);
}
