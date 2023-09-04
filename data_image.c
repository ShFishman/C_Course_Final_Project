#include "data_image.h"
/*This function creates a new node and adds it to the end of a linked list of nodes.*/
data_image_node* create_new_data_image_node(data_image_node* head)
{
		data_image_node *ptr = head;
		data_image_node *new_node = (data_image_node*)malloc(sizeof(data_image_node));
		/* check if malloc worked*/
		/* Safe check for malloc */
		if (new_node == NULL)
		{
			fprintf(stderr, "Error: Unable to allocate memory\n");
			return NULL;
		}
		memset(new_node,0,sizeof(data_image_node));
		if(ptr==NULL)
			return new_node;
		while(ptr->next!=NULL)
			ptr=ptr->next;
		ptr->next = new_node;
		return new_node;
}
/*free the linked list*/
void free_data_image_list(data_image_node *head)
{
	data_image_node *current = head;
	data_image_node *next;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
}
