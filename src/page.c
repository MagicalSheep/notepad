/*************************************************
 * File name: line.c
 * Author: MagicalSheep
 * ID: 8208201308
 * Version: 1.0.0
 * Date: 2020/12/21
 * Description: 
 *  Page is an instance of the document.
 * History: 
 *  2020/12/21  MagicalSheep    Init the file.
 *  2020/12/23  MagicalSheep    Beautify the codes.
 *  2020/12/23  MagicalSheep    Offer traslation between logic and physic. 
*************************************************/

#include <page.h>

Page page;
Line line;

void init_page(const char *name, int cols, int lines)
{
    init_line(&line);
    move_to(&line, 1);
    strcpy(page.doc_name, name);
    page.text = &line;
    page.cols = cols;
    page.lines = lines;
}

inline Page *get_page()
{
    return &page;
}

inline Line *get_line()
{
    return &line;
}

inline void set_docname(const char *name)
{
    strcpy(page.doc_name, name);
}

inline int get_pos(int offset, int y, int x)
{
    // y begin at position 1
    return (y - 1 + offset) * page.cols + x + 1;
}

void get_cursor_pos(int offset, int pos, int *y, int *x)
{
    // y begin at position 1
    *y = pos / page.cols + 1 - offset;
    *x = pos - (*y - 1 + offset) * page.cols;
}