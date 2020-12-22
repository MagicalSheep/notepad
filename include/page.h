#ifndef _PAGE_H_
#define _PAGE_H_

#include <line.h>

#define MAX_DOC_NAME_LENGTH 200

typedef struct page
{
    char doc_name[MAX_DOC_NAME_LENGTH];
    Line *text;
    int line_num;
} Page;

void init_page(const char *name);

Page *get_page();

Line *get_line();

void add_line();

void set_docname(const char *name);

#endif