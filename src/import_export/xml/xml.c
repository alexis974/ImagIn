#include <stdio.h>
#include <err.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "xml.h"

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
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterStartDocument\n");
    }

    /*
    ** Start an element named "IMAGE". Since thist is the first
    ** element, this will be the root element of the document.
    */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "IMAGE");
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterStartElement\n");
    }

    /* Add an attribute with name "xml:lang" and value "en" to IMAGE. */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xml:lang",
            BAD_CAST "en");
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterWriteAttribute\n");
    }

    /* Write a comment as child of IMAGE. */
    rc = xmlTextWriterWriteFormatComment(writer, "This is a comment");
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterWriteComment\n");
    }

    /* Write a comment as child of IMAGE */
    rc = xmlTextWriterWriteFormatComment(writer, "This is another comment");
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterWriteFormatComment\n");
    }

    /* Start an element named "DESCRIPTION" as child of IMAGE. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "DESCRIPTION");
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterStartElement\n");
    }

    /* Write an element named "NAME" as child of DESCRIPTION. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "NAME",
            BAD_CAST "This is a text.");
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterWriteElement\n");
    }

    /* Start an element named "EXIF" as child of DEXCRIPTION. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "EXIF");
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterStartElement\n");
    }
    /* Write an element named "Date" as child of EXIF. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "Date",
            BAD_CAST "This is a text.");
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterWriteElement\n");
    }

    /* Close the element named EXIF. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterEndElement\n");
    }

    /* Close the element named DESCRIPTION. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterEndElement\n");
    }

    /* Start an element named "HISTORY" as child of IMAGE. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "HISTORY");
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterStartElement\n");
    }

    /* Write an element named "TEXT" as child of HISTORY. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "TEXT",
            BAD_CAST "This is a text.");
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterWriteElement\n");
    }

    /* Close the element named HISTORY. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterEndElement\n");
    }

    /*
    ** Since we do not want to write any other elements, we simply call
    ** xmlTextWriterEndDocument, which will do all the work.
    */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0)
    {
        errx(1, "create_xml: Error at xmlTextWriterEndDocument\n");
    }

    xmlFreeTextWriter(writer);

    //Cleanup function for the XML library.
    xmlCleanupParser();

    // This is to debug memory for regression tests
    xmlMemoryDump();
}
