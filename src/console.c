/*************************************************
 * File name: line.c
 * Author: MagicalSheep
 * ID: 8208201308
 * Version: 1.0.0
 * Date: 2020/12/20
 * Description: 
 *  Main UI file
 *  include head area, editor and command area
 * History: 
 *  2020/12/20  MagicalSheep    Init the file.
 *  2020/12/21  MagicalSheep    Add multiple funtion area.
 *  2020/12/23  MagicalSheep    Beautify the codes.
 *  2020/12/23  MagicalSheep    Rebuild the control function.
 *  2020/12/24  MagicalSheep    Delete lines implementation.
 *  2020/12/24  MagicalSheep    Edit function completed.
 *  2020/12/25  MagicalSheep    Optimization of opening file.
 *  2020/12/25  MagicalSheep    Add status area.
 *  2020/12/25  MagicalSheep    Support linux.
*************************************************/

#include <console.h>
// #define _DEBUG_

int x = 0, y = 1; // position of the cursor, y->row, x->col
int offset;       // current y offset to the document head line

#if defined(_DEBUG_)
int monitor;
void debug_print(Line *line)
{
    int old_x, old_y;
    getyx(stdscr, old_y, old_x);
    clear();
    move(26, 0);
    clrtoeol();
    printw("x = %d, y = %d, pos = %d, strlen = %d, monitor = %d\n", old_x, old_y, line->cursor_pos, line->string_length, monitor);
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
    }
    move(old_y, old_x);
}
#endif

void search();
void print_page();
void mv_return();
void mv_backspace();
void mv_insert(char ch);
int mv_up();
int mv_down();
void mv_left();
void mv_right();

void loading(double progress)
{
    int t_y = y, t_x = x;
    move(LINES - 1, 0);
    attron(A_REVERSE);
    // draw
    int cnt = (int)COLS * progress;
    for (int i = 1; i <= cnt; i++)
        addch(' ');
    attroff(A_REVERSE);
    move(t_y, t_x);
    refresh();
}

void update_status()
{
    int t_x, t_y;
    getyx(stdscr, t_y, t_x);
    move(LINES - 1, 0);
    clrtoeol();
    attron(A_REVERSE);
    printw("LINE = %d, COL = %d, offset = %d, length = %d", y, x, offset, get_line()->content_length);
    attroff(A_REVERSE);
    move(t_y, t_x);
}

void update_status_with_message(char *message)
{
    int t_x, t_y;
    getyx(stdscr, t_y, t_x);
    move(LINES - 1, 0);
    clrtoeol();
    attron(A_REVERSE);
    printw(message);
    attroff(A_REVERSE);
    move(t_y, t_x);
}

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

void init_editor(Page *page)
{
    int operate;

#if defined(_DEBUG_)
    debug_print(get_line());
#endif

    while (1)
    {
        operate = getch();
        switch (operate)
        {
        case '\n': // return in linux
        case '\r': // return
            mv_return();
            print_page();
            break;
        case 127:  // delete (backspace in linux)
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
        case 'C' - 64: // ctrl+c
        case 27:       // ESC
            save(get_page());
            endwin();
            exit(0);
            break;
        case 'S' - 64: // ctrl+s
            save(get_page());
            break;
        case 'F' - 64: // ctrl+f
            // search();
            break;
        default:
            mv_insert(operate);
            print_page();
            break;
        }
        getyx(stdscr, y, x);
        update_status();

#if defined(_DEBUG_)
        debug_print(get_line());
#endif

        refresh();
    }
}

void init(const char *name, const wchar_t *content)
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    init_page(name, COLS, LINES);
    init_head_area(get_page());
    move(1, 0);
    refresh();

    curs_set(0);
    size_t len = wcslen(content);
    if (len == 0)
    {
        insert_char(get_line(), END);
        int tmp = bpos_to_cpos(get_line(), get_line()->cursor_pos) % COLS;
        if (tmp == 0)
            tmp = COLS;
        int cnt = COLS - tmp;
        for (int j = 0; j <= cnt; j++)
            insert_char(get_line(), INVALID);
        goto load_end;
    }
    for (long long i = 0; i < len; i++)
    {
        insert_char(get_line(), content[i]);
        if (content[i] == NEWLINE)
        {
            int tmp = bpos_to_cpos(get_line(), get_line()->cursor_pos) % COLS;
            if (tmp == 0)
                tmp = COLS;
            int cnt = COLS - tmp;
            // I don't know why it has to begin at 0, but it works well
            for (int j = 0; j <= cnt; j++)
                insert_char(get_line(), INVALID);
        }
        if (i == len - 1)
        {
            insert_char(get_line(), END);
            int tmp = bpos_to_cpos(get_line(), get_line()->cursor_pos) % COLS;
            if (tmp == 0)
                tmp = COLS;
            int cnt = COLS - tmp;
            for (int j = 0; j <= cnt; j++)
                insert_char(get_line(), INVALID);
        }
        loading((double)i / (double)len);
    }
load_end:
    curs_set(1);
    update_status();
    offset = 0;
    move_to(get_line(), 1); // move to the front of the first character(NEWLINE flag)
    move(1, 0);
    print_page();
    free_read_buffer();

    init_editor(get_page());
    refresh();
}

inline void search()
{
    // TODO: wait to be developed
    int t_y = y, t_x = x;
    move(LINES - 1, 0);
    clrtoeol();
    char in = 0;
    char words[COLS * 2];
    int index = 0;
    for (int i = 0;; i++)
    {
        in = getch();
        if (in == '\r')
        {
            words[i] = '\0';
            break;
        }
        addch(in);
        refresh();
        words[i] = in;
    }
    int pos = search_string(get_line(), words);
    if (pos == -1)
    {
        update_status_with_message("Nothing is founded");
        move(t_y, t_x);
    }
    else
    {
        // logical cursor move
        move_to(get_line(), pos);
        int lines = pos / COLS + 1;
        if (pos % COLS == 0)
            lines -= 1;
        if (lines <= EDITOR_HEIGHT)
            offset = 0;
        else
            offset = lines - EDITOR_HEIGHT;
        get_cursor_pos(offset, pos, &t_y, &t_x);
        move(t_y, t_x);
        getyx(stdscr, y, x);
        update_status();
    }
}

inline void print_page()
{
    int old_x, old_y;
    getyx(stdscr, old_y, old_x);
    clear();
    init_head_area(get_page());
    update_status();
    move(1, 0);
    Line *line = get_line();
    for (int i = offset * COLS; i < line->buffer_length; i++)
    {
        if (getcury(stdscr) == LINES - 1)
            break;
        if (line->buffer[i] == INVALID || line->buffer[i] == 0)
            continue;
        if (line->buffer[i] == END)
            break;
        if (line->buffer[i] == NEWLINE)
        {
            printw("\n");
            continue;
        }
        if (line->buffer[i])
            addnwstr(line->buffer + i, 1);
    }
    move(old_y, old_x);
}

inline void mv_return()
{
    // logical new line
    int cnt = get_pos(offset, y, x) - get_pos(offset, y, 0);
    insert_char(get_line(), NEWLINE);
    int begin = bpos_to_cpos(get_line(), get_line()->cursor_pos);
    int end = get_pos(offset, y, COLS - 1);
    for (int i = begin; i <= end; i++)
        insert_char(get_line(), INVALID);
    for (int i = 1; i <= cnt; i++)
        add_invalid(get_line());
    // physical new line
    if (y == LINES - 2)
    {
        offset++;
        move(y, 0);
    }
    else
    {
        move(y + 1, 0);
    }
}

inline void mv_backspace()
{
    if (offset == 0 && x == 0 && y == 1)
        // head can't be deleted
        return;
    // logical delete
    for (int i = get_pos(offset, y, x) - 1; i > 0; i--)
    {
        int flag = is_valid(get_line(), i);
        delete_char_at(get_line(), i);
        add_invalid(get_line());
        if (flag)
            break;
    }
    int pos = bpos_to_cpos(get_line(), get_line()->cursor_pos);
    int t_y = 0, t_x = 0;
    get_cursor_pos(offset, get_line()->cursor_pos, &t_y, &t_x);
    int t_pos = get_pos(offset, t_y, COLS - 1) + 1;
    int t_len = get_line()->string_length;
    while (t_pos <= get_line()->string_length)
    {
        if (!is_valid(get_line(), t_pos))
            delete_char_at(get_line(), t_pos);
        else
            break;
    }
    move_to(get_line(), pos);
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
            move(t_y, t_x);
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
    int before = get_NEWLINE_pos(get_line()) - 1;
    int t1_y = 0, t1_x = 0;
    get_cursor_pos(offset, before, &t1_y, &t1_x);
    insert_char(get_line(), ch);
    int after = get_NEWLINE_pos(get_line()) - 1;
    int t2_y = 0, t2_x = 0;
    get_cursor_pos(offset, after, &t2_y, &t2_x);
    if (t2_y != t1_y)
    {
        // update the INVALID flag
        // history remain codes, but it works well.
        int tmp_pos = bpos_to_cpos(get_line(), get_line()->cursor_pos);
        move_to(get_line(), after + 2);
        int begin = bpos_to_cpos(get_line(), get_line()->cursor_pos);
        int t_y = 0, t_x = 0;
        get_cursor_pos(offset, get_line()->cursor_pos, &t_y, &t_x);
        int end = get_pos(offset, t_y, COLS - 1);
        for (int i = begin; i <= end; i++)
            insert_char(get_line(), INVALID);
        move_to(get_line(), tmp_pos);
    }
    else
    {
        remove_invalid(get_line());
    }
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
    }
    else
    {
        move(y, x + 1);
    }
}

inline int mv_up()
{
    if (offset == 0 && y == 1)
        return 0;
    // move logical cursor
    int pos = get_pos(offset, y - 1, x);
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
        for (int i = pos; i >= pos - x; i--)
        {
            if (is_valid(get_line(), i))
            {
                // move logical cursor
                move_to(get_line(), i);
                // move physical cursor
                if (y == 1)
                    offset--;
                int t_y = 0, t_x = 0;
                get_cursor_pos(offset, get_line()->cursor_pos, &t_y, &t_x);
                move(t_y, t_x);
                print_page();
                return 1;
            }
        }
    }
    return 0;
}

inline int mv_down()
{
    int pos = get_pos(offset, y + 1, x);
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
        int end = get_pos(offset, y + 1, 0);
        for (int i = pos; i >= end; i--)
        {
            if (is_valid(get_line(), i))
            {
                // logical cursor move
                move_to(get_line(), i);
                // move physical cursor
                if (y == LINES - 2)
                    offset++;
                int t_y = 0, t_x = 0;
                get_cursor_pos(offset, get_line()->cursor_pos, &t_y, &t_x);
                move(t_y, t_x);
                print_page();
                return 1;
            }
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
            {
                offset--;
                int begin = get_pos(offset, y, x);
                int end = get_pos(offset, y, COLS - 1);
                for (int i = end; i >= begin; i--)
                {
                    if (is_valid(get_line(), i))
                    {
                        // logical cursor move
                        move_to(get_line(), i);
                        // physical cursor move
                        int t_y = 0, t_x = 0;
                        get_cursor_pos(offset, get_line()->cursor_pos, &t_y, &t_x);
                        move(t_y, t_x);
                        break;
                    }
                }
                print_page();
            }
        }
        else
        {
            int begin = get_pos(offset, y, x) - 1;
            int end = get_pos(offset, y - 1, 0);
            for (int i = begin; i >= end; i--)
            {
                if (is_valid(get_line(), i))
                {
                    // logical cursor move
                    move_to(get_line(), i);
                    // physical cursor move
                    int t_y = 0, t_x = 0;
                    get_cursor_pos(offset, get_line()->cursor_pos, &t_y, &t_x);
                    move(t_y, t_x);
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
        move_right(get_line()); // cross the NEWLINE flag
        int flag = 0;
        int begin = bpos_to_cpos(get_line(), get_line()->cursor_pos);
        for (int i = begin; i <= get_line()->string_length; i++)
        {
            if (is_valid(get_line(), i))
            {
                // logical cursor move
                move_to(get_line(), i);
                // physical cursor move
                if (y == LINES - 2)
                    offset++;
                int t_y = 0, t_x = 0;
                get_cursor_pos(offset, get_line()->cursor_pos, &t_y, &t_x);
                move(t_y, t_x);
                print_page();
                flag = 1;
                break;
            }
        }
        if (!flag)
            move_left(get_line());
    }
}