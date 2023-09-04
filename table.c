#include "table.h"

table_node* create_table_node(table_node* table_head)
{
	table_node *ptr = table_head;
	table_node *new_node = (table_node*)malloc(sizeof(table_node));
	new_node->next =NULL;
	/* check if malloc worked*/
	if(table_head==NULL)
		return new_node;
	memset(new_node, 0, sizeof(table_node));
	while(ptr->next!=NULL)
		ptr=ptr->next;
	ptr->next = new_node;
	return new_node;
}
void free_table(table_node *head)
{
	 table_node *current = head;
	    table_node *next=NULL;
	    while (current != NULL)
	    {
	        next = current->next;
	        free(current);
	        current = next;
	    }
	    head = NULL;
}
int search_table(table_node *head,char* name)
{
	table_node *ptr = head->next;
	int to_return =-1;
	while(ptr!=NULL)
	{
		if(strcmp(name,ptr->name)==0)
		{
			to_return=(int)(ptr->address);
			return to_return;
		}
		ptr=ptr->next;
	}
	return to_return;
}
