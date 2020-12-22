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
*************************************************/

#include <page.h>
#include <stdlib.h>
#include <string.h>

Page page;
Line line;

void init_page(const char *name)
{
    init_line(&line);
    strcpy(page.doc_name, name);
    page.line_num = 1;
    page.text = &line;
}

Page *get_page()
{
    return &page;
}

Line *get_line()
{
    return &line;
}

void add_line()
{
    page.line_num++;
}

void set_docname(const char *name)
{
    strcpy(page.doc_name, name);
}