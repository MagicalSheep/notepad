#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#define EDITOR_HEIGHT (LINES - 2)

#include <curses.h>
#include <line.h>
#include <page.h>
#include <stdlib.h>
#include <string.h>

/**
 * Init console
 * @param page Page to be loaded
 */
void init();

void init_editor(Page *page);

void print_page();

int mv_up();

int mv_down();

void mv_left();

void mv_right();

void init_head_area(Page *page);

void init_command_area();

#endif