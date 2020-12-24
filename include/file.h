#ifndef _FILE_H_
#define _FILE_H

#include <page.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_BUFFER_LENGTH 1000000

/**
 * Open page from the file
 * @param path file path
 */
char *open(const char *path);

/**
 * Save page to the file
 * @param page page to be saved
 */
int save(Page *page);

/**
 * Free the read buffer
 */
void free_read_buffer();

#endif