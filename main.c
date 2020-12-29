#include <console.h>
#include <file.h>
#include <locale.h>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    wchar_t *content;
    char *name;
    if (argc == 1)
    {
        content = open("./untitled.txt");
        name = "./untitled.txt";
    }
    else
    {
        content = open(argv[1]);
        name = argv[1];
    }
    init(name, content);
    return 0;
}