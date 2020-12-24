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
 *  2020/12/23  MagicalSheep    Define the concept of position.
 *  2020/12/23  MagicalSheep    Rebuild the line control function.
 *  2020/12/24  MagicalSheep    Add get NEWLINE flag function.
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
    while (postion <= line->cursor_pos)
        move_left(line);
}

void move_right_at(Line *line, const int postion)
{
    while (postion - 1 > line->cursor_pos)
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
}

void insert_str(Line *line, const char *str)
{
    for (int i = 0; i < (int)strlen(str); i++)
        insert_char(line, str[i]);
}

void delete_char(Line *line)
{
    move_right(line);
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
    if (position <= 0 || position > line->string_length)
        return 0;
    int b_p = cpos_to_bpos(line, position);
    return line->buffer[b_p] != INVALID;
}

void remove_invalid(Line *line)
{
    int pos = bpos_to_cpos(line, line->cursor_pos);
    int endpos = get_NEWLINE_pos(line);
    int c_p = endpos + 1;
    delete_char_at(line, c_p);
    move_to(line, pos);
}

void add_invalid(Line *line)
{
    int pos = bpos_to_cpos(line, line->cursor_pos);
    int endpos = get_NEWLINE_pos(line);
    int c_p = endpos + 1;
    move_to(line, c_p);
    insert_char(line, INVALID);
    move_to(line, pos);
}

int get_NEWLINE_pos(Line *line)
{
    for (int i = line->cursor_pos;; i++)
    {
        // new line signal include NEWLINE and EOF
        if (line->buffer[i] == NEWLINE || line->buffer[i] == EOF)
            return bpos_to_cpos(line, i);
    }
}

int bpos_to_cpos(Line *line, int bpos)
{
    if (bpos <= line->cursor_pos)
        return bpos + 1;
    else
        return bpos - line->gap_length + 1;
}

int cpos_to_bpos(Line *line, int cpos)
{
    int t_p = bpos_to_cpos(line, line->cursor_pos);
    if (cpos < t_p)
        return cpos - 1;
    else
        return line->cursor_pos + line->gap_length + cpos - t_p;
}