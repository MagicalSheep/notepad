/*************************************************
 * File name: line.c
 * Author: MagicalSheep
 * ID: 8208201308
 * Version: 0.1.0
 * Date: 2012/12/24
 * Description: 
 *  File util
 * History: 
 *  2020/12/24  MagicalSheep    Init the file.
*************************************************/

#include <file.h>

char *read_buffer;
long long string_length;
long long buffer_length;
int _index;
char *directory;

void add_to_read_buffer(char ch);
void grow_read_buffer();

char *open(const char *path)
{
    read_buffer = (char *)calloc(READ_BUFFER_LENGTH, sizeof(char));
    buffer_length = READ_BUFFER_LENGTH;
    string_length = 0;
    _index = 0;
    directory = (char *)calloc(strlen(path) + 1, sizeof(char));
    strcpy(directory, path);
    FILE *fp;
    fp = fopen(path, "r");
    if (fp == NULL)
    {
        fp = fopen(path, "w");
    }
    else
    {
        char ch;
        while (1)
        {
            ch = fgetc(fp);
            if (feof(fp))
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
    fp = fopen(directory, "w");
    if (fp == NULL)
        return 0;
    char *b = page->text->buffer;
    int length = page->text->buffer_length;
    for (int i = 0; i < length; i++)
    {
        if(b[i] == INVALID || b[i] == 0 || b[i] == EOF)
            continue;
        fputc(b[i], fp);
    }
    fclose(fp);
    return 1;
}

void grow_read_buffer()
{
    char old_buffer[buffer_length];
    memcpy(old_buffer, read_buffer, sizeof(read_buffer));
    read_buffer = (char *)calloc(buffer_length * 2, sizeof(char));
    for (int i = 0; i < buffer_length; i++)
        read_buffer[i] = old_buffer[i];
    buffer_length *= 2;
}

void add_to_read_buffer(char ch)
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