#ifndef _LIST_H_
#define _LIST_H_

#include <line.h>

typedef struct node
{
    Line line;
    struct node *next;
    struct node *prev;
} Node;

/**
 * Init the list of line, set an empty node as the head node
 * @return head node
 */
Node *init_text();

/**
 * Add a new line node to the list tail
 * @param line new line
 * @param head_node head node
 */
void add_line_tail(Line line, Node *head_node);

/**
 * Delete the tail line from the list
 * @param head_node head node
 */
void del_line_tail(Node *head_node);

/**
 * Add a new line node to the specific positon of the list
 * @param line new line
 * @param position position to insert the new line
 * @param head_node head node
 */
void add_line(Line line, int position, Node *head_node);

/**
 * Delete the specific line from the list
 * @param line_num line number
 * @param head_node head node
 */
void del_line(int line_num, Node *head_node);

/**
 * Test the list empty
 * @param head_node head node
 * @return whether is empty or not
 */
int empty(Node *head_node);

#endif