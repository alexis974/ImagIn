#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>

#include "../../tools/history/convert.h"
#include "xml.h"

void rc_handler(int rc, char *str)
{
    if (rc < 0)
    {
        errx(1, "%s\n", str);
    }
}

// TODO : Coding style : Fct 25 lines max
void create_xmp(const char *uri)
{
    int rc;
    xmlTextWriterPtr writer;

    /* Create a new XmlWriter for uri, with no compression. */
    writer = xmlNewTextWriterFilename(uri, 0);
    if (writer == NULL)
    {
        errx(1, "create_xml: Error creating the xml writer\n");
    }

    /* Start the document. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    rc_handler(rc, "create_xml: Error at xmlTextWriterStartDocument");

    /*
     ** Start an element named "IMAGE". Since thist is the first
     ** element, this will be the root element of the document.
     */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "IMAGE");
    rc_handler(rc, "create_xml: Error at xmlTextWriterStartElement");

    /* Add an attribute with name "xml:lang" and value "en" to IMAGE. */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xml:lang",
            BAD_CAST "en");
    rc_handler(rc, "create_xml: Error at xmlTextWriterWriteAttribute");

    /* Write a comment as child of IMAGE. */
    rc = xmlTextWriterWriteFormatComment(writer, "This is a comment");
    rc_handler(rc, "create_xml: Error at xmlTextWriterWriteComment");
    }

    /* Write a comment as child of IMAGE */
    rc = xmlTextWriterWriteFormatComment(writer, "This is another comment");
    rc_handler(rc, "create_xml: Error at xmlTextWriterWriteFormatComment");

    /* Start an element named "DESCRIPTION" as child of IMAGE. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "DESCRIPTION");
    rc_handler(rc, "create_xml: Error at xmlTextWriterStartElement");

    /* Write an element named "NAME" as child of DESCRIPTION. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "NAME",
            BAD_CAST "This is a text.");
    rc_handler(rc, "create_xml: Error at xmlTextWriterWriteElement");

    /* Start an element named "EXIF" as child of DEXCRIPTION. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "EXIF");
    rc_handler(rc, "create_xml: Error at xmlTextWriterStartElement");

    /* Write an element named "Date" as child of EXIF. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "Date",
            BAD_CAST "This is a text.");
    rc_handler(rc, "create_xml: Error at xmlTextWriterWriteElement");

    /* Close the element named EXIF. */
    rc = xmlTextWriterEndElement(writer);
    rc_handler(rc, "create_xml: Error at xmlTextWriterEndElement");

    /* Close the element named DESCRIPTION. */
    rc = xmlTextWriterEndElement(writer);
    rc_handler(rc, "create_xml: Error at xmlTextWriterEndElement");

    /* Start an element named "HISTORY" as child of IMAGE. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "HISTORY");
    rc_handler(rc, "create_xml: Error at xmlTextWriterStartElement");

    /* Write an element named "TEXT" as child of HISTORY. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "TEXT",
            BAD_CAST "This is a text.");
    rc_handler(rc, "create_xml: Error at xmlTextWriterWriteElement");

    /* Close the element named HISTORY. */
    rc = xmlTextWriterEndElement(writer);
    rc_handler(rc, "create_xml: Error at xmlTextWriterEndElement\n");

    /*
     ** Since we do not want to write any other elements, we simply call
     ** xmlTextWriterEndDocument, which will do all the work.
     */
    rc = xmlTextWriterEndDocument(writer);
    rc_handler(rc, "create_xml: Error at xmlTextWriterEndDocument\n");

    xmlFreeTextWriter(writer);
    xmlCleanupParser();
    xmlMemoryDump();
}


/* Save the given history at the uri path */
void save_hist(struct history *hist, const char *uri)
{
    hist = hist->next;
    if (!hist)
    {
        errx(1, "save_hist_xml : hist was empty\n");
    }

    int rc;
    xmlTextWriterPtr writer;

    writer = xmlNewTextWriterFilename(uri, 0);
    if (writer == NULL)
    {
        errx(1, "save_hist_xml: Error creating the xml writer\n");
    }

    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    rc_handler(rc, "save_hist_xml: Error at xmlTextWriterStartDocument");

    rc = xmlTextWriterStartElement(writer, BAD_CAST "Modules");
    rc_handler(rc, "save_hist_xml: Error at xmlTextWriterStartElement");

    while (hist)
    {
        rc = xmlTextWriterStartElement(writer, BAD_CAST get_name(hist->id));
        rc_handler(rc, "save_hist_xml: Error at xmlTextWriterStartElement");

        char _enable[2];
        char _value[30];
        sprintf(_enable, "%d", hist->enable);
        sprintf(_value, "%f", hist->value);

        rc = xmlTextWriterWriteElement(writer, BAD_CAST "Enable",
                BAD_CAST _enable);
        rc_handler(rc, "save_hist_xml: Error at xmlTextWriterWriteElement");

        rc = xmlTextWriterWriteElement(writer, BAD_CAST "Value",
                BAD_CAST _value);
        rc_handler(rc, "save_hist_xml: Error at xmlTextWriterWriteElement");

        rc = xmlTextWriterEndElement(writer);
        rc_handler(rc, "create_xml: Error at xmlTextWriterEndElement");

        hist = hist->next;
    }

    rc = xmlTextWriterEndDocument(writer);
    rc_handler(rc, "create_xml: Error at xmlTextWriterEndDocument");

    xmlFreeTextWriter(writer);
    xmlCleanupParser();
    xmlMemoryDump();
}

struct history *load_hist(const char *path)
{
    struct history *hist = hst_new();
    xmlTextReaderPtr reader;
    int ret;

    reader = xmlReaderForFile(path, NULL, 0);
    if (reader != NULL)
    {
        int _id;
        int _enable;
        float _value;
        const char *str_tmp;

        ret = xmlTextReaderRead(reader);
        ret = xmlTextReaderRead(reader);
        while (ret == 1)
        {
            ret = xmlTextReaderRead(reader);
            if (ret == 0)
            {
                break;
            }

            ret = xmlTextReaderRead(reader);

            _enable = atoi((const char *)xmlTextReaderReadOuterXml(reader));
            printf("%i\n", _enable);

            ret = xmlTextReaderRead(reader);
            ret = xmlTextReaderRead(reader);
            ret = xmlTextReaderRead(reader);

            _value = atof((const char *)xmlTextReaderReadOuterXml(reader));
            printf("%f\n", _value);

            ret = xmlTextReaderRead(reader);
            ret = xmlTextReaderRead(reader);

            //Find better conversion
            str_tmp = (const char *)xmlTextReaderReadOuterXml(reader);
            char _str_id[strlen(str_tmp)-2];
            memset(_str_id, '\0', sizeof(_str_id));
            strncpy(_str_id, str_tmp + 1, strlen(str_tmp) - 3);
            _id = get_id(_str_id);
            printf("%s\n", _str_id);
            printf("%i\n", _id);

            // Bug my come from here
            //hst_append(hist, _id, _enable, _value);

            ret = xmlTextReaderRead(reader);
        }
        if (ret != 0)
        {
            fprintf(stderr, "%s : failed to parse\n", path);
            xmlFreeTextReader(reader);
            return(NULL);
        }

        xmlFreeTextReader(reader);
    }
    else
    {
        fprintf(stderr, "Unable to open %s\n", path);
        return(NULL);
    }

    return hist;
}
