/*************************************************
 * File name: line.c
 * Author: MagicalSheep
 * ID: 8208201308
 * Version: 0.1.0
 * Date: 2012/12/20
 * Description: 
 *  Page is an instance of the document.
 * History: 
 *  2020/12/21  MagicalSheep    Init the file.
 *  2020/12/23  MagicalSheep    Beautify the codes.
*************************************************/

#include <page.h>

Page page;
Line line;

void init_page(const char *name, int cols, int lines)
{
    init_line(&line);
    insert_char(&line, NEWLINE);
    for (int i = 1; i < cols; i++)
        insert_char(&line, INVALID);
    move_to(&line, 0);
    strcpy(page.doc_name, name);
    page.line_num = 1;
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

inline void add_line()
{
    page.line_num++;
}

inline void set_docname(const char *name)
{
    strcpy(page.doc_name, name);
}

inline int get_pos(int offset, int y, int x)
{
    // y begin at position 1
    return (y - 1 + offset) * page.cols + x;
}