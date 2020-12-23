/*************************************************
 * File name: line.c
 * Author: MagicalSheep
 * ID: 8208201308
 * Version: 0.1.0
 * Date: 2012/12/20
 * Description: 
 *  Main UI file
 *  include head area, editor and command area
 * History: 
 *  2020/12/20  MagicalSheep    Init the file.
 *  2020/12/21  MagicalSheep    Add multiple funtion area.
 *  2020/12/23  MagicalSheep    Beautify the codes.
*************************************************/

#include <console.h>

int x = 0, y = 1; // position of the cursor, y->row, x->col
int offset;       // current y offset to the document head line

void debug_print(Line *line);

void print_page();
void mv_return();
void mv_backspace();
void mv_insert(char ch);
int mv_up();
int mv_down();
void mv_left();
void mv_right();

void init_head_area(Page *page)
{
    int t_x, t_y;
    getyx(stdscr, t_y, t_x);
    int len = strlen(page->doc_name);
    int title_pos = (COLS - len) / 2;
    move(0, title_pos);
    attron(A_REVERSE);
    printw(page->doc_name);
    move(t_y, t_x);
    attroff(A_REVERSE);
}

void init_command_area()
{
    int t_x, t_y;
    getyx(stdscr, t_y, t_x);
    move(LINES - 1, 0);
    attron(A_REVERSE);
    printw("-- INSERT --");
    move(t_y, t_x);
    attroff(A_REVERSE);
}

void init_editor(Page *page)
{
    offset = 0;
    move_to(get_line(), 0);
    move(1, 0);
    int operate;
    while (1)
    {
        operate = getch();
        switch (operate)
        {
        case '\r': // return
            mv_return();
            break;
        case '\b': // backspace
            mv_backspace();
            break;
        case KEY_LEFT:
            mv_left();
            break;
        case KEY_RIGHT:
            mv_right();
            break;
        case KEY_UP:
            mv_up();
            break;
        case KEY_DOWN:
            mv_down();
            break;
        default:
            mv_insert(operate);
            break;
        }
        getyx(stdscr, y, x);
        debug_print(get_line());
        refresh();
    }
}

void init(const char *name)
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    init_page(name, COLS, LINES);

    init_head_area(get_page());
    init_command_area();
    init_editor(get_page());
    refresh();
}

inline void print_page()
{
    return;
    int old_x, old_y;
    getyx(stdscr, old_y, old_x);
    clear();
    init_head_area(get_page());
    init_command_area();
    move(1, 0);
    Line *line = get_line();
    for (int i = offset * COLS; i < line->buffer_length; i++)
    {
        if (getcury(stdscr) == LINES - 1)
            break;
        if (line->buffer[i] == INVALID || line->buffer[i] == 0)
            continue;
        if (line->buffer[i] == EOF)
            break;
        if (line->buffer[i] == NEWLINE)
        {
            printw("\n");
            continue;
        }
        printw("%c", line->buffer[i]);
    }
    move(old_y, old_x);
}

inline void mv_return()
{
    // logical new line
    insert_char(get_line(), NEWLINE);
    get_page()->line_num++;
    for (int i = get_line()->cursor_pos; i <= (y + offset) * COLS - 1; i++)
        insert_char(get_line(), INVALID);
    // physical new line
    if (y == LINES - 2)
        move(y, 0);
    else
        move(y + 1, 0);
    print_page();
}

inline void mv_backspace()
{
    if (offset == 0 && x == 0 && y == 1)
        // head can't be deleted
        return;
    // logical delete
    if (x == 0)
    {
    }
    delete_char(get_line());
    add_invalid(get_line());
    // physical delete
    if (x == 0)
    {
        if (y == 1)
        {
            offset--;
            move(y, COLS - 1);
        }
        else
        {
            move(y - 1, COLS - 1);
        }
    }
    else
    {
        move(y, x - 1);
    }
    print_page();
}

inline void mv_insert(char ch)
{
    // logical insert
    insert_char(get_line(), ch);
    // physical insert
    if (x == COLS - 1)
    {
        if (y == LINES - 2)
        {
            offset++;
            move(y, 0);
            clrtoeol();
        }
        else
        {
            move(y + 1, 0);
        }
        getyx(stdscr, y, x);
        // INVALID flag update
        int tmp_pos = get_line()->cursor_pos;
        move_right(get_line()); // cross NEWLINE flag
        for (int i = tmp_pos; i <= (y + offset) * COLS - 1; i++)
            insert_char(get_line(), INVALID);
        move_to(get_line(), tmp_pos);
    }
    else
    {
        move(y, x + 1);
    }
    print_page();
}

inline int mv_up()
{
    if (offset == 0 && y == 1)
        return 0;
    // move logical cursor
    int pos = get_pos(offset, y, x) - COLS;
    if (is_valid(get_line(), pos))
    {
        move_to(get_line(), pos);
        // move physical cursor
        if (y == 1)
        {
            offset--;
            print_page();
        }
        else
        {
            move(y - 1, x);
        }
        return 1;
    }
    else
    {
        int flag = 0;
        int dpos = 0;
        for (int i = pos; i >= pos - x; i--)
        {
            if (is_valid(get_line(), i))
            {
                move_to(get_line(), i);
                flag = 1;
                dpos = i;
                break;
            }
        }
        if (flag)
        {
            // move physical cursor
            if (y == 1)
            {
                offset--;
                print_page();
                move(y, x - (pos - dpos));
            }
            else
            {
                move(y - 1, x - (pos - dpos));
            }
            return 1;
        }
    }
    return 0;
}

inline int mv_down()
{
    int pos = get_pos(offset, y, x) + COLS;
    // move logical cursor
    if (is_valid(get_line(), pos))
    {
        move_to(get_line(), pos);
        // move physical cursor
        if (y == LINES - 2)
        {
            offset++;
            print_page();
        }
        else
        {
            move(y + 1, x);
        }
        return 1;
    }
    else
    {
        int flag = 0;
        int dpos = 0;
        for (int i = pos; i >= pos - x; i--)
        {
            if (is_valid(get_line(), i))
            {
                move_to(get_line(), i);
                flag = 1;
                dpos = i - (y + offset) * COLS;
                break;
            }
        }
        if (flag)
        {
            // move physical cursor
            if (y == LINES - 2)
            {
                offset++;
                print_page();
                move(y, dpos);
            }
            else
            {
                move(y + 1, dpos);
            }
            return 1;
        }
    }
    return 0;
}

inline void mv_left()
{
    int pos = get_pos(offset, y, x) - 1; // the position to move to
    // move logical cursor
    if (is_valid(get_line(), pos))
    {
        move_to(get_line(), pos);
        // move physical cursor
        if (x == 0)
        {
            if (y == 1)
            {
                offset--;
                print_page();
                move(y, COLS - 1);
            }
            else
            {
                move(y - 1, COLS - 1);
            }
        }
        else
        {
            move(y, x - 1);
        }
    }
    else
    {
        if (y == 1 && x == 0)
        {
            if (offset != 0)
                mv_up(); // temp; both logical and physical cursor move
        }
        else
        {
            int begin = (y - 1 + offset) * COLS - 1;
            int end = (y - 2 + offset) * COLS;
            for (int i = begin; i >= end; i--)
            {
                if (is_valid(get_line(), i))
                {
                    // logical cursor move
                    move_to(get_line(), i);
                    // physical cursor move
                    move(y - 1, i - end);
                    break;
                }
            }
        }
    }
}

inline void mv_right()
{
    int pos = get_pos(offset, y, x) + 1; // the position to move to
    // move logical cursor
    if (is_valid(get_line(), pos))
    {
        move_to(get_line(), pos);
        // move physical cursor
        if (x == COLS - 1)
        {
            if (y == LINES - 2)
            {
                offset++;
                print_page();
                move(y, 0);
            }
            else
            {
                move(y + 1, 0);
            }
        }
        else
        {
            move(y, x + 1);
        }
    }
    else
    {
        mv_down(); // temp
        // for (int i = get_line()->cursor_pos; i < get_line()->string_length; i++)
        // {
        //     if (is_valid(get_line(), i))
        //     {
        //         // logical cursor move
        //         move_to(get_line(), i);
        //         // physical cursor move
        //         int pos = get_line()->cursor_pos - (y - 1 + offset) * COLS;
        //         if (y == LINES - 2)
        //         {
        //             offset++;
        //             move(y, pos);
        //         }
        //         else
        //         {
        //             move(y + 1, pos);
        //         }
        //         break;
        //     }
        // }
    }
}

inline void debug_print(Line *line)
{
    int old_x, old_y;
    getyx(stdscr, old_y, old_x);
    clear();
    move(26, 0);
    clrtoeol();
    printw("x = %d, y = %d, pos = %d, strlen = %d\n", old_x, old_y, line->cursor_pos, line->string_length);
    clrtoeol();
    printw("COLS = %d, LINES = %d, LEFT_POS = %d, RIGHT_POS = %d", COLS, LINES, ((y - 1) + offset) * COLS + x - 1, ((y - 1) + offset) * COLS + x + 1);
    move(1, 0);
    for (int i = 0; i < line->buffer_length; i++)
    {
        if (line->buffer[i] == 0)
        {
            attron(A_BOLD);
            printw("%d ", 0);
            attroff(A_BOLD);
        }
        else
        {
            printw("%d ", line->buffer[i]);
        }
        // if (i == line->cursor_pos - 1 || i == line->cursor_pos + line->gap_length)
        // {
        //     attron(A_BOLD);
        //     printw("%c", line->buffer[i]);
        //     attroff(A_BOLD);
        // }
        // else if (!(i >= line->cursor_pos && i < line->cursor_pos + line->gap_length))
        // {
        //     printw("%c", line->buffer[i]);
        // }
    }
    move(old_y, old_x);
}