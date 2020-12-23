#ifndef _PAGE_H_
#define _PAGE_H_

#include <line.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DOC_NAME_LENGTH 200

typedef struct page
{
    char doc_name[MAX_DOC_NAME_LENGTH];
    Line *text;
    int line_num;
    int cols;
    int lines;
} Page;

void init_page(const char *name, int cols, int lines);

Page *get_page();

Line *get_line();

void add_line();

void set_docname(const char *name);

/**
 * Get the element position in the line by physical cursor position
 * @param offset offset
 * @param y y
 * @param x x
 * @return position in line
 */
int get_pos(int offset, int y, int x);

#endif