#ifndef TABLE_H_
#define TABLE_H_

#include <stdlib.h>
#include <string.h>
#include "global_helpers.h"

typedef struct table_node
{
    char name[MAX_LABEL_SIZE];
    unsigned int address;
    int type;
    struct table_node* next;
} table_node;

/* Creates a new table node and returns a pointer to it. */
table_node* create_table_node();

/* Frees the memory allocated for the given table. */
void free_table(table_node* table_head);

/* Searches the given table for a node with the given name.
 * Returns the node's address if found, or -1 otherwise.
 */
int search_table(table_node* head, char* name);

#endif /* TABLE_H_ */
