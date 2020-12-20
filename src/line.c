/*************************************************
 * File name: line.c
 * Author: MagicalSheep
 * ID: 8208201308
 * Version: 0.1.0
 * Date: 2012/12/20
 * Description: 
 *  Implementation of the gap buffer.
 * History: 
 *  2020/12/20  MagicalSheep    Init the file.
*************************************************/

#include <line.h>
#include <stdlib.h>
#include <string.h>

void init_line(Line *line)
{
    (*line).buffer = (char *)calloc(BUFFER_LENGTH, sizeof(char));
    (*line).gap_length = BUFFER_LENGTH;
    (*line).buffer_length = BUFFER_LENGTH;
    (*line).cursor_pos = 0;
}

void move_left(Line *line)
{
    (*line).cursor_pos--;
    (*line).buffer[(*line).cursor_pos + (*line).gap_length] = (*line).buffer[(*line).cursor_pos];
    (*line).buffer[(*line).cursor_pos] = 0;
}

void move_right(Line *line)
{
    (*line).buffer[(*line).cursor_pos] = (*line).buffer[(*line).cursor_pos + (*line).gap_length];
    (*line).buffer[(*line).cursor_pos + (*line).gap_length] = 0;
    (*line).cursor_pos++;
}

void move_left_at(Line *line, const int postion)
{
    while (postion < (*line).cursor_pos)
        move_left(line);
}

void move_right_at(Line *line, const int postion)
{
    while (postion > (*line).cursor_pos)
        move_right(line);
}

void grow(Line *line)
{
    int old_length = (*line).buffer_length;
    char old_buffer[old_length];
    memcpy(old_buffer, (*line).buffer, old_length * sizeof(char));
    int gap_length = 0;
    (*line).buffer = (char *)calloc((*line).buffer_length + BUFFER_LENGTH, sizeof(char));
    (*line).buffer_length += BUFFER_LENGTH;
    (*line).gap_length = BUFFER_LENGTH;
    char gap[BUFFER_LENGTH];
    // copy origin gap data to an array
    for (int i = (*line).cursor_pos; i < BUFFER_LENGTH && i < old_length; i++, gap_length++)
        gap[i - (*line).cursor_pos] = old_buffer[i];
    // copy the first part
    for (int i = 0; i < (*line).cursor_pos; i++)
        (*line).buffer[i] = old_buffer[i];
    // copy the second part
    for (int i = (*line).cursor_pos + BUFFER_LENGTH, j = 0; j < gap_length; i++, j++)
        (*line).buffer[i] = gap[j];
    // copy the third part
    for (int i = (*line).cursor_pos + BUFFER_LENGTH * 2, j = (*line).cursor_pos + BUFFER_LENGTH; i < (*line).buffer_length; i++, j++)
        (*line).buffer[i] = old_buffer[j];
}

void insert_char(Line *line, const char ch)
{
    if ((*line).gap_length == 0)
        grow(line);
    (*line).buffer[(*line).cursor_pos] = ch;
    (*line).cursor_pos++;
    (*line).gap_length--;
}

void insert_str(Line *line, const char *str)
{
    for (int i = 0; i < (int)strlen(str); i++)
        insert_char(line, str[i]);
}

void delete_char(Line *line)
{
    (*line).cursor_pos--;
    (*line).buffer[(*line).cursor_pos] = 0;
    (*line).gap_length++;
}

void delete_char_at(Line *line, int position)
{
    if ((*line).cursor_pos - position >= 0)
        move_left_at(line, position + 1);
    else
        move_right_at(line, position + 1);
    delete_char(line);
}