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

/**
 * Init page
 * @param name document name
 * @param cols max columns of the console
 * @param lines max lines of the console
 */
void init_page(const char *name, int cols, int lines);

/**
 * Return the pointer of the instance of page
 * @return page pointer
 */
Page *get_page();

/**
 * Return the pointer of the instance of line
 * @return line pointer
 */
Line *get_line();

/**
 * Set document name
 * @param name document name
 */
void set_docname(const char *name);

/**
 * Get the element position(count position) in the line 
 * by physical cursor position
 * @param offset offset
 * @param y y
 * @param x x
 * @return position in line
 */
int get_pos(int offset, int y, int x);

/**
 * Get the physical cursor position by element position 
 * (count position) in the line
 * @param offset offset
 * @param pos element position in line
 * @param y variable to save y position
 * @param x variable to save x position
 */
void get_cursor_pos(int offset, int pos, int *y, int *x);

#endif