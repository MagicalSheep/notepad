/*************************************************
 * File name: line.c
 * Author: MagicalSheep
 * ID: 8208201308
 * Version: 1.0.0
 * Date: 2020/12/24
 * Description: 
 *  File util
 * History: 
 *  2020/12/24  MagicalSheep    Init the file.
*************************************************/

#include <file.h>
#include <wchar.h>

wchar_t *read_buffer;
wchar_t *old_buffer;
long long string_length;
long long buffer_length;
int _index;
char *directory;

void add_to_read_buffer(wchar_t ch);
void grow_read_buffer();

wchar_t *open(const char *path)
{
    read_buffer = (wchar_t *)calloc(READ_BUFFER_LENGTH, sizeof(wchar_t));
    buffer_length = READ_BUFFER_LENGTH;
    string_length = 0;
    _index = 0;
    directory = (char *)calloc(strlen(path) + 1, sizeof(char));
    strcpy(directory, path);
    FILE *fp;
    fp = fopen(path, "r,ccs=utf-8");
    if (fp == NULL)
    {
        fp = fopen(path, "w,ccs=utf-8");
    }
    else
    {
        wchar_t ch;
        while (1)
        {
            ch = fgetwc(fp);
            if (ch == WEOF)
                break;
            add_to_read_buffer(ch);
        }
    }
    fclose(fp);
    return read_buffer;
}

int save(Page *page)
{
    FILE *fp;
    fp = fopen(directory, "w,ccs=utf-8");
    if (fp == NULL)
        return 0;
    wchar_t *b = page->text->buffer;
    int length = page->text->buffer_length;
    for (int i = 0; i < length; i++)
    {
        if (b[i] == INVALID || b[i] == 0 || b[i] == END)
            continue;
        fputwc(b[i], fp);
    }
    fclose(fp);
    return 1;
}

void grow_read_buffer()
{
    int old_length = buffer_length;
    old_buffer = (wchar_t *)calloc(old_length, sizeof(wchar_t));
    memcpy(old_buffer, read_buffer, old_length * sizeof(char));
    free(read_buffer);
    read_buffer = (wchar_t *)calloc(buffer_length * 2, sizeof(wchar_t));
    buffer_length *= 2;
    for (int i = 0; i < old_length; i++)
        read_buffer[i] = old_buffer[i];
    free(old_buffer);
}

void add_to_read_buffer(wchar_t ch)
{
    if (string_length == buffer_length)
        grow_read_buffer();
    read_buffer[_index++] = ch;
    string_length++;
}

inline void free_read_buffer()
{
    free(read_buffer);
}