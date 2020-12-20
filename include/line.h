#ifndef _LINE_H_
#define _LINE_H_

#include <stddef.h>
#define BUFFER_LENGTH 10

typedef struct line
{
    char *buffer;
    int gap_length;       // current gap length
    size_t buffer_length; // current buffer length
    int cursor_pos;       // the current position of the gap
} Line;

/**
 * Create an instance of line
 * @param line line to be init
 */
void init_line(Line *line);

/**
 * Move the gap left to left at a step
 */
void move_left(Line *line);

/**
 * Move the gap right to right at a step
 */
void move_right(Line *line);

/**
 * Move the gap left to the position of the buffer
 * @param line line
 * @param position the position you want to move to
 */
void move_left_at(Line *line, const int postion);

/**
 * Move the gap right to the position of the buffer
 * @param line line
 * @param position the position you want to move to
 */
void move_right_at(Line *line, const int postion);

/**
 * Grow the gap buffer from the current cursor positon
 * @param line line
 */
void grow(Line *line);

/**
 * Insert a character to the current cursor positon
 * @param line line
 * @param ch character to be inserted
 */
void insert_char(Line *line, const char ch);

/**
 * Insert a string to the current cursor position
 * @param line line
 * @param str string to be inserted
 */
void insert_str(Line *line, const char *str);

/**
 * Delete a character at front of the positon of current cursor position
 * @param line line
 */
void delete_char(Line *line);

/**
 * Delete a character at a specific position has been pointed to
 * @param line line
 * @param position the position of character to be deleted
 */
void delete_char_at(Line *line, int position);

#endif