#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#define EDITOR_HEIGHT (LINES - 2)

#include <curses.h>
#include <file.h>
#include <line.h>
#include <page.h>
#include <stdlib.h>
#include <string.h>

/**
 * Init console
 * @param name document name
 * @param content document content
 */
void init(const char *name, const char *content);

#endif