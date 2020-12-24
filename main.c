#include <console.h>
#include <file.h>

int main(int argc, char *argv[])
{
    char *content;
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