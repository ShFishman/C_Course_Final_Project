#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pre_processor.h"

macro *macro_list_head;
macro* create_macro_node(macro* head);
void execute_pre_processor(FILE *fp,FILE *fpn)
{
	macro_list_head =create_macro_node(NULL);
	analyze_file(fp,fpn);
}
/* This function reads characters from a file fp and analyzes each line by calling the analyze_line function.
 * The analyzed lines are then written to another file fpn.
 * The current_state variable tracks the state of the analysis as it processes each line.
 *  The function continues to read and analyze lines until it reaches the end of the file (EOF).*/
void analyze_file(FILE* fp,FILE* fpn)
{
	char c;
	char line[MAX_LINE_SIZE] = {0};
	int i =0;
	state current_state = normal;
	while((c=fgetc(fp))!=EOF)
	{
		if(c=='\n'||i==MAX_LINE_SIZE)
		{
			line[i]='\0';
			i=0;
			analyze_line(line,&current_state,fpn);
		}
		else
		{
			line[i]=c;
			i++;
		}
	}

}
/*The function "analyze_line" takes a line of text, the current state and a file pointer,
 * and either copies the line to the file or treats it as a macro definition or a macro line.
 * If the state is normal, it checks if the line starts with a macro identifier or a macro definition command,
 *  and acts accordingly. If the state is in macro definition mode,
 it adds the line as a macro line or terminates the definition mode based on "endmcr".*/
void analyze_line(char* line,state *current_state,FILE* fpn)
{
	char line_cpy[MAX_LINE_SIZE]={0};
	char* token;
	macro* tmp_link =NULL;
	strcpy(line_cpy,line);
	token = strtok(line," \t\n");

	if(*current_state==normal)
	{
		if(token==NULL)
		{
			fprintf(fpn,"%s\n",line_cpy);
			return;
		}
		if((tmp_link=get_link(token))!=NULL)
		{
			fprintf(fpn,"%s\n",tmp_link->macro_line);
			return;
		}
		if(!strcmp(token,"mcr"))
		{
 			token= strtok(NULL," \t\n");

			insert_link(token);
			*current_state = read_macro;
			return;
		}
		fprintf(fpn,"%s\n",line_cpy);
	}
	else
	{
		if(token ==NULL)
			macro_line_to_last_link(line_cpy);
		else if(!strcmp(token,"endmcr"))
			*current_state =normal;
		else
			macro_line_to_last_link(line_cpy);
	}
}
/*function creates a new linked list node and inserts it at the end of the linked list, representing a macro with the given name.*/
void insert_link(const char* name)
{
	macro* new = create_macro_node(macro_list_head);
	strcpy(new->name,name);
}
/*This function returns the linked list node representing the macro with the given name.
 * If there is no such node, the function returns NULL.*/
struct macro* get_link(const char* name)
{

	macro* ptr = macro_list_head;
	while(ptr!=NULL)
	{
		if(strcmp(name,ptr->name)==0)
			return ptr;
		ptr = ptr->next;
	}
	return NULL;
}
/*This function add a line to the "macro_line" of the last node in the linked list.*/
void macro_line_to_last_link(char* current_macro_line)
{
	macro* ptr = macro_list_head;
	char* new_macro_line = NULL;
	while(ptr->next!=NULL)
		ptr = ptr->next;
	if(ptr->macro_line==NULL)
	{
		new_macro_line = (char*)malloc(sizeof(char)*strlen(current_macro_line)+1);
		strcpy(new_macro_line,current_macro_line);
		ptr->macro_line = new_macro_line;
	}
	else
	{
		ptr->macro_line = (char*)realloc(ptr->macro_line,strlen(ptr->macro_line)+strlen(current_macro_line)+2);
		strcat(ptr->macro_line,"\n");
		strcat(ptr->macro_line,current_macro_line);
		ptr->macro_line[strlen(ptr->macro_line)]='\0';
	}
}
/*This frees all memory used by the linked list and its nodes, releasing the memory back to the system.*/
void free_macro_list()
{
	macro* current = macro_list_head;
	macro* next = NULL;
	while (current != NULL)
	{
		next = current->next;
		if(current->macro_line!=NULL)
			free(current->macro_line);
		free(current);
		current = next;
	}
}
macro* create_macro_node(macro* head)
{
	macro *ptr = head;
	macro *new_node = (macro*)malloc(sizeof(macro));
	if (new_node == NULL)
	{
		return NULL;
	}
	memset(new_node, 0, sizeof(macro));
	if (ptr == NULL)
	{
		head = new_node;
	}
	else
	{
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = new_node;
	}
	return new_node;
}
