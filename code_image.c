#include "code_image.h"
/*This function creates a new node and adds it to the end of a linked list of nodes.*/
code_image_node* create_new_code_image_node(code_image_node* head)
{
	code_image_node *ptr = head;
	code_image_node *new_node = (code_image_node*)malloc(sizeof(code_image_node));

	/* Safe check for malloc */
	if (new_node == NULL) {
		fprintf(stderr, "Error: Unable to allocate memory\n");
		return NULL;
	}

	new_node->next = NULL;
	memset(new_node, 0, sizeof(code_image_node));

	if (head == NULL) {
		head = new_node;
	} else {
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new_node;
	}

	return new_node;
}
/*free the linked list*/
void free_code_image_list(code_image_node *head)
{
    code_image_node *current = head, *next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    /* Reset head pointer to NULL */
    head = NULL;
}
