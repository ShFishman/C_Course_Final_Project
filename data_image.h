#ifndef DATA_IMAGE_H_
#define DATA_IMAGE_H_
#include <stdlib.h>
#include <string.h>
#include "global_helpers.h"

/**
Struct that represents a node in the linked list that holds a 14-bit data value.
The struct contains a data field that represents the compiled data value of the node, and a pointer to the next node in the
linked list.
*/
typedef struct data_image_node
{
	unsigned int data:14;
	struct data_image_node* next;
}data_image_node;
/**
Creates a new data image node and returns a pointer to the newly created node.
gets head The head of the linked list to which the new node will be added.
return A pointer to the newly created node.
*/
data_image_node* create_new_data_image_node(data_image_node* head);
/**
 Frees a linked list of data image nodes.
gets head The head of the linked list to be freed.
*/
void free_data_image_list(data_image_node *head);
#endif /* DATA_IMAGE_H_ */
