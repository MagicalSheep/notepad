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
 *  2020/12/23  MagicalSheep    Fix bugs.
 *  2020/12/23  MagicalSheep    Beautify the codes.
*************************************************/

#include <line.h>

void init_line(Line *line)
{
    line->buffer = (char *)calloc(BUFFER_LENGTH, sizeof(char));
    line->gap_length = BUFFER_LENGTH;
    line->buffer_length = BUFFER_LENGTH;
    line->cursor_pos = 0;
    line->string_length = 0;
}

void move_left(Line *line)
{
    line->cursor_pos--;
    line->buffer[line->cursor_pos + line->gap_length] = line->buffer[line->cursor_pos];
    line->buffer[line->cursor_pos] = 0;
}

void move_right(Line *line)
{
    line->buffer[line->cursor_pos] = line->buffer[line->cursor_pos + line->gap_length];
    line->buffer[line->cursor_pos + line->gap_length] = 0;
    line->cursor_pos++;
}

void move_left_at(Line *line, const int postion)
{
    while (postion < line->cursor_pos)
        move_left(line);
}

void move_right_at(Line *line, const int postion)
{
    while (postion > line->cursor_pos)
        move_right(line);
}

void move_to(Line *line, const int position)
{
    if (line->cursor_pos - position >= 0)
        move_left_at(line, position);
    else
        move_right_at(line, position);
}

void grow(Line *line)
{
    int old_length = line->buffer_length;
    char old_buffer[old_length];
    memcpy(old_buffer, line->buffer, old_length * sizeof(char));
    line->buffer = (char *)calloc(line->buffer_length + BUFFER_LENGTH, sizeof(char));
    line->buffer_length += BUFFER_LENGTH;
    line->gap_length = BUFFER_LENGTH;
    // copy the first part
    for (int i = 0; i < line->cursor_pos; i++)
        line->buffer[i] = old_buffer[i];
    // copy the second part
    for (int i = line->cursor_pos + BUFFER_LENGTH, j = line->cursor_pos; i < line->buffer_length; i++, j++)
        line->buffer[i] = old_buffer[j];
}

void insert_char(Line *line, const char ch)
{
    line->buffer[line->cursor_pos] = ch;
    line->cursor_pos++;
    line->gap_length--;
    line->string_length++;
    if (line->gap_length == 0)
        grow(line);
    if (ch != INVALID && ch != NEWLINE && ch != EOF)
        remove_invalid(line);
}

void insert_str(Line *line, const char *str)
{
    for (int i = 0; i < (int)strlen(str); i++)
        insert_char(line, str[i]);
}

void delete_char(Line *line)
{
    line->cursor_pos--;
    line->buffer[line->cursor_pos] = 0;
    line->gap_length++;
    line->string_length--;
}

void delete_char_at(Line *line, int position)
{
    move_to(line, position);
    delete_char(line);
}

int is_valid(Line *line, int position)
{
    if (position < 0 || position > line->string_length)
        return 0;
    if (position < line->cursor_pos)
    {
        return line->buffer[position] != INVALID;
    }
    else
    {
        int pos = line->gap_length + position;
        return line->buffer[pos] != INVALID;
    }
}

void remove_invalid(Line *line)
{
    int pos = line->cursor_pos;
    for (int i = line->cursor_pos; i < line->buffer_length; i++)
    {
        if (line->buffer[i] == NEWLINE)
        {
            if (i + 1 < line->buffer_length && line->buffer[i + 1] == INVALID)
            {
                delete_char_at(line, i - line->gap_length + 2);
            }
            break;
        }
    }
    move_to(line, pos);
}

void add_invalid(Line *line)
{
    int pos = line->cursor_pos;
    for (int i = line->cursor_pos; i < line->buffer_length; i++)
    {
        if (line->buffer[i] == NEWLINE)
        {
            if (i + 1 < line->buffer_length && line->buffer[i + 1] == INVALID)
            {
                move_to(line, i + 1);
                insert_char(line, INVALID);
            }
            break;
        }
    }
    move_to(line, pos);
}