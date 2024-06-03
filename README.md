# 文本编辑器

C语言课程设计作业

实验设计报告见[个人博客](https://magicalsheep.cn/2432231332/)

## 安装

### 二进制文件安装

你可以从Release页面下载对应平台的可执行文件。

### 编译安装

开始前，你需要安装cmake，且至少拥有一个编译器。

- Windows：
  
  下载源代码，依次执行以下命令(<download_dir>替换为你下载的源代码目录）：

  ```bash
  cd <download_dir>
  mkdir build
  cd build
  cmake .. -G "Unix Makefiles"
  make
  ```

- Unix / Linux：
  
  你需要从[ncurses](https://invisible-island.net/ncurses/#downloads "ncurses")的官方网站上下载源代码，并编译安装ncurses，然后依次执行以下命令：

  ```bash
  cd <download_dir>
  mkdir build && cd build
  cmake ..
  make
  ```

接下来，你就可以在build文件夹中找到该程序的可执行文件了。

## 运行

你可以在命令行窗口执行`./Notepad`命令运行该程序，这将会在当前目录下创建一个名为`untitled`的空白文本文件。

如果你希望打开指定文本文件，则以`./Notepad <doc_path>`命令运行程序，这将尝试打开并编辑路径名为`<doc_path>`的文本文件。如果它不存在，程序将会创建它。

接下来就可以愉快地编辑文本啦！

程序支持的快捷键：

- Ctrl+S：保存文件
- Ctrl+C：保存并退出程序
- Esc：保存并退出程序

## 运行截图

![](https://magicalsheep.cn/assets/imgs/2020-12-25/notepad.png "运行截图")

## 原理

### 文本存储数据结构

整个文本采用带有Gap Buffer的动态数组顺序存储。同时，为了便于物理光标定位，每一行的末尾插入了n个值为-2的INVALID标记，直到窗口最大宽度。这些标记在读入文件与编辑文本时被创建，在写出文件时将被忽略。

![](https://media.geeksforgeeks.org/wp-content/uploads/20190605114709/Gap-Buffer-1.png "Gap Buffer原理图")

上图所示为Gap Buffer的工作原理。无论是插入还是删除，其时间复杂度都能控制在常数级别，效率十分可观。在实际应用中，适当增大Gap Buffer的长度可以大量减少因动态数组扩容而导致的复制操作，从而有效提高文件的打开速度。

![](https://media.geeksforgeeks.org/wp-content/uploads/20190606184341/GAP-BUFFER-LEFT-11.png "Gap Buffer的移动")

Gap Buffer的左右移动只需要交换其首尾两个元素，从头移动到尾进行一次遍历需要线性时间复杂度。而在实际应用中，光标的移动总是发生在相邻几个元素或相邻几行之间，使得Gap Buffer在大部分应用场景下是十分可靠的。

### 光标移动控制

由于字符在数组中进行存储，因此我们可以通过下标访问他们。但需要注意的是，由于Gap Buffer的存在，每一个字符的下标实际上是动态变化的。Gap Buffer长度的变化、位置的移动都会影响每一个字符的数组下标。注意到字符在文本中的计数位置是固定的（第一个字符、第二个字符……第n个字符），我们考虑以计数位置来定位字符，并建立计数位置与物理光标坐标之间的关系。

在该程序中，数组下标被称为`Buffer Position`，计数位置被称为`Count Position`。对文本的插入、删除等编辑操作，都是基于计数位置进行的，这是由于我们能够建立计数位置与数组下标之间的对应关系。

```c
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
```

我们将整个窗口视为一张地图，其中不含有效字符的坐标填充为`INVALID`标记。现在，我们可以建立起该地图上点的坐标与文本计数位置之间的转换关系了。

```c
void get_cursor_pos(int offset, int pos, int *y, int *x)
{
    // y begin at position 1
    *y = pos / page.cols + 1 - offset;
    *x = pos - (*y - 1 + offset) * page.cols;
}
```

移动物理光标到应有的位置也变得轻松起来。
