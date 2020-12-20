#include <console.h>
#include <curses.h>
#include <line.h>
#include <string.h>
#include <text.h>

int x = 0, y = 0; // position of the cursor, y->row, x->col
int width, height;

// void print(Line *line)
// {
//     for (int i = 0; i < (*line).buffer_length; i++)
//     {
//         if (i == (*line).cursor_pos)
//             attron(A_BOLD);
//         printw("%d ", (int)(*line).buffer[i]);
//         if (i == (*line).cursor_pos)
//             attroff(A_BOLD);
//     }
//     printw("\n");
// }

void init()
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    width = getmaxx(stdscr);
    height = getmaxy(stdscr);
}

void open_editor()
{
    int operate;
    Line line;
    init_line(&line);
    while (1)
    {
        operate = getch();
        switch (operate)
        {
        case '\r': // return
            move(y + 1, 0);
            // new line begin
            break;
        case '\b': // backspace
            move(y, x - 1);
            delch();
            delete_char(&line);
            break;
        case KEY_LEFT:
            move(y, x - 1);
            move_left(&line);
            break;
        case KEY_RIGHT:
            move(y, x + 1);
            move_right(&line);
            break;
        case KEY_UP:
            // move(y - 1, x);
            break;
        case KEY_DOWN:
            // move(y + 1, x);
            break;
        default:
            insch(operate);
            move(y, x + 1);
            insert_char(&line, operate);
            break;
        }
        x = getcurx(stdscr);
        y = getcury(stdscr);
        refresh();
    }
}
