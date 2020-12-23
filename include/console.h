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
 * @param name document name
 */
void init(const char *name);

#endif