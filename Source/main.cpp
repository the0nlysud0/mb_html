#include "../Include/lexbor/html/parser.h"
#include "../Include/lexbor/dom/dom.h"
#include "../Include/lexbor/html/html.h"
#include "../Include/lexbor/html/interface.h"
#include <../Include/lexbor/html/parser.h>
#include <../Include/lexbor/dom/interfaces/element.h>

main(int argc, const char *argv[])
{
    lxb_status_t status;
    const lxb_char_t *tag_name;
    lxb_html_document_t *document;

    static const lxb_char_t html[] = "<div>Works fine!</div>";
    size_t html_len = sizeof(html) - 1;

    document = lxb_html_document_create();
    if (document == NULL) {
        exit(EXIT_FAILURE);
    }

    status = lxb_html_document_parse(document, html, html_len);
    if (status != LXB_STATUS_OK) {
        exit(EXIT_FAILURE);
    }

    tag_name = lxb_dom_element_qualified_name(lxb_dom_interface_element(document->body),
                                              NULL);

    printf("Element tag name: %s\n", tag_name);

    lxb_html_document_destroy(document);

    return EXIT_SUCCESS;
}