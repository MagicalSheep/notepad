#ifndef _LINE_H_
#define _LINE_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEWLINE '\n'
#define INVALID -2
#define BUFFER_LENGTH 10

typedef struct line
{
    char *buffer;
    int gap_length;       // current gap length
    size_t buffer_length; // current buffer length
    int content_length;   // the content length, exclude EOF, '\n' and INVALID
    int string_length;    // the content length, include EOF, '\n' and INVALID
    int cursor_pos;       // the current position of the gap(buffer position)
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
 * Move the gap left to the position of the buffer(count position), 
 * ready for inserting
 * @param line line
 * @param position the position you want to move to
 */
void move_left_at(Line *line, const int postion);

/**
 * Move the gap right to the position of the buffer(count position), 
 * ready for inserting
 * @param line line
 * @param position the position you want to move to
 */
void move_right_at(Line *line, const int postion);

/**
 * Move the gap to the position of the buffer(count position), 
 * ready for inserting
 * @param line line
 * @param position the position you want to move to
 */
void move_to(Line *line, const int position);

/**
 * Grow the gap buffer from the current cursor positon
 * @param line line
 */
void grow(Line *line);

/**
 * Insert a character to the current cursor positon(point to count position)
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
 * (point to count position)
 * @param line line
 */
void delete_char(Line *line);

/**
 * Delete a character at a specific position has been pointed to(count position)
 * @param line line
 * @param position the position of character to be deleted
 */
void delete_char_at(Line *line, int position);

/**
 * To check whether the element at the position(count position) 
 * of the buffer is able to achieve or not
 * @param line line
 * @param position element count position
 * @return is valid or not
 */
int is_valid(Line *line, int position);

/**
 * Remove an INVALID flag behind the NEWLINE flag on current line
 * @param line line
 */
void remove_invalid(Line *line);

/**
 * Get NEWLINE flag position(count position) from current line
 * @param line line
 * @return NEWLINE flag position
 */
int get_NEWLINE_pos(Line *line);

/**
 * Add INVALID flag to the end of current line(cursor point to)
 * @param line line
 */
void add_invalid(Line *line);

/**
 * Translate buffer position to the count position 
 * which is not include the cursor space and count 
 * from 1
 * @param line line
 * @param bpos buffer position
 * @return count position
 */
int bpos_to_cpos(Line *line, int bpos);

/**
 * Translate count position which is not include the 
 * cursor space and count from 1 to the buffer position
 * @param line line
 * @param cpos count position
 * @return buffer position
 */
int cpos_to_bpos(Line *line, int cpos);

#endif