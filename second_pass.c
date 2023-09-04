#include "code_image.h"
#include "data_image.h"
#include "table.h"
#include "second_pass.h"
#include "first_pass.h"

/*This function performs the second pass of a compiler program by calling three
 functions that update and encode the symbol table, final labels, and entry list. */
void execute_second_pass()
{
	update_symbol_table();
	encode_final_labels();
	encode_entry_list();
}
/*This function updates the address of symbols in the symbol table if they are of type DATA.*/
void update_symbol_table()
{
	table_node* ptr = symbol_list_head->next;
	while(ptr!=NULL)
	{
		if(ptr->type==DATA)
			ptr->address+=IC;
		ptr = ptr->next;
	}
}
/* This function encodes the final labels by searching for them in the symbol table and setting their ARE values accordingly.
 * If a label is not found, it creates a new entry in the extern list. This is necessary to generate correct object files.*/
void encode_final_labels()
{
	int i,result;
	table_node* current_ext;
	code_image_node *ptr= code_image_head->next;
	while(ptr!=NULL)
	{
		for(i=0;i<ptr->par_counter;i++)
		{
			if(ptr->b[i].ARE==3)
			{
				result = search_table(symbol_list_head,ptr->b[i].label);
				if(result==-1)
				{
					ptr->b[i].ARE=1;
					ptr->b[i].data.normal_case.val=0;
					current_ext = create_table_node(extern_list_head);
					strcpy(current_ext->name,ptr->b[i].label);
					current_ext->address = ptr->b[i].adress;
				}
				else
				{
					ptr->b[i].ARE=2;
					ptr->b[i].data.normal_case.val=result;
				}
 			}
		}
		ptr=ptr->next;
	}
}
/*  This function encodes the entry list by searching for the entry symbol in the symbol table and setting its address
 *  if found. This is necessary to generate correct object files and to support linking with other object files.*/
void encode_entry_list()
{
	table_node *ptr = entry_list_head->next;
	int adress=0;
	while(ptr!=NULL)
	{
		adress = search_table(symbol_list_head,ptr->name);
		if(adress>0)
			ptr->address = adress;
		ptr=ptr->next;
	}
}
