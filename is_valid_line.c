#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "commands.h"
#include "identify.h"
#include "is_valid_line.h"

struct entry_extern_node
{ /*creating a list of labels that were defined as extern or entry */
    char name[MAX_LINE_SIZE];
    struct entry_extern_node* next;
} analyze_ent_ext_head_node = {"\0", NULL};

struct label_node
{ /*creating a list of labels that were defined */
    char name[MAX_LINE_SIZE];
    struct label_node* next;
} analyze_lbalel_head_node = {"\0", NULL};

/*function decleration*/
int validate_entry_and_extern( struct entry_extern_node *ptr, char* line);
int is_new_label(struct label_node *ptr, char* line);

int validating_file(FILE *fp)
{
	int i =1, to_return=0;
	char line[MAX_LINE_SIZE];
	if (fp == NULL)
	{
		printf("Error opening file.\n");
		return 1;
	}
	fseek(fp,0,SEEK_SET);
	while (fgets(line,MAX_LINE_SIZE,fp) != NULL)
	{
		if(is_white_space_line(line) == 0 || is_note_line(line) == 0 )
		{
			i++;
			continue;
		}
		else
		{
			if( is_valid_line(line)!= 0)
			{
				printf("^ in line %d. ^\n", i);	
				to_return = 1;
			}
			i++;
		}
	}
	check_double_names();

 	return to_return;
}

int is_valid_line(char* line)
{
	char* token, *result;
	char cpy_line[MAX_LINE_SIZE] = {0},line_second[MAX_LINE_SIZE]={0};
	int to_return=0;
	struct entry_extern_node* ptr = &analyze_ent_ext_head_node; 
	if(line == NULL || line[0]==';')
		return 0;
	strcpy(cpy_line , line);
	result = exclude_first_token(line);
	if(result != NULL)
		strcpy(line_second , result );
	token = strtok(cpy_line , DELIM);
	 if(is_command(token) == 0)
		to_return = validate_command(line);
	 else  if(is_label(token) == 0)
		to_return = validate_label(line);
	 else if (is_extern(token) == 0 || is_entry(token) == 0 )
		to_return = validate_entry_and_extern( ptr , line_second);
	 else if(is_data(token) == 0)
		to_return = validate_data(result);
	 else if( is_string(token)== 0 )
		to_return = validate_string(result);
	 else
		 to_return =1;
	free(result);
	if(!to_return)
		return 0;
	printf("ERROR: line doesn't fit any valid line structure.\n");
	return 1;
}
/* function gets a line that starts with a label like 'label1:' ... */
int validate_label(char *line)
{
	struct entry_extern_node* entry_extern_ptr = &analyze_ent_ext_head_node;
	struct label_node* label_ptr = &analyze_lbalel_head_node; 
	char* token,*result,*result2;
	char line_second[MAX_LINE_SIZE] = {0},line_third[MAX_LINE_SIZE]={0},cpy_line[MAX_LINE_SIZE]={0};
	if (line == NULL)
		return 1;
	strcpy(cpy_line , line);
	if(is_new_label( label_ptr ,line) != 0)
		return 1;
	result = exclude_first_token(line);
	if(result != NULL)
		strcpy(line_second , result);
	result2 = exclude_first_two_tokens(line);
	if(result2 != NULL)
		strcpy(line_third , result2);
	free(result);
	free(result2);
	token = strtok(cpy_line , DELIM);
	if(token == NULL)
		printf("ERROR: no words.\n");

	token = strtok(NULL , DELIM);		
	if(token == NULL){
		printf("ERROR: no words after label.\n");
		return 1;
	}
	if(is_entry(token) == 0 || is_extern(token) == 0){
		printf("NOTE: line starts with LABEL: and than- .extern or .entry.\n");
		validate_entry_and_extern(entry_extern_ptr , line_third);
	}
	if(is_data(token) != 0 && is_string(token)!=0 && is_command(token)!= 0){
		printf("ERROR: first word of label should be \".string\" or \".data\"\n or a command.");
		return 1;
	}
	if( is_data(token) == 0 )
		return validate_data(line_third);
	if( is_string(token)== 0 )
		return validate_string(line_third);
	if( is_command(token) == 0)
		return validate_command(line_second);
	return 0;
}
/* function gets the part of line that starts right after .data*/
int validate_data(char* line)
{
	int count = 0, flag = 1 ;
	char cpy_line[MAX_LINE_SIZE];
	char* token;
	memset(cpy_line, 0, sizeof(cpy_line));
	if(line != NULL){
	strcpy(cpy_line , line);
	flag = alternatly_string_comma(line);
	}
	token = strtok(cpy_line , DELIM_COMMA); 
	if (token == NULL)
	{
		printf("ERROR: no tokens found in input string.\n");
		return 1;
	}
	if(token != NULL)
	{
		if( flag == 0)
		{ /* checking for invalid commas */
			while( token != NULL )
			{ /* checking if strings are numbers */
				count++;
				if(is_number(token, count)!= 0)
					return 1;
				token = strtok(NULL, DELIM_COMMA );	
			}
			return 0;
		}
	}
	return 1;
}

int is_number(char *token, int index){
	int i;
	if(!isdigit(token[0]) && token[0] != '-' && token[0] != '+')
	{
		printf("ERROR: %d-th element of data not a number.\n" , index );
		return 1;
	}
	else
	{
		for(i = 1;i< strlen(token);i++)
		{
			if( isdigit(token[i])==0 )
			{
				printf("ERROR: %d-th element of data not a number.\n",index);
				return 1;
			}
		}
	}
	return 0 ;
}

int alternatly_string_comma(char *line){ /*varified function*/
	int last; /*last =0 if last word wasn't a comma. last =1 if comma is the last word seen.*/
	char cpy_line[MAX_LINE_SIZE*3] ;
	char* token;
	memset(cpy_line, 0, sizeof(cpy_line));
	make_cpy_line( line , cpy_line);
	token = strtok(cpy_line , DELIM);
	if( strcmp( "," , token ) == 0 )
	{ /* , before first number */
		printf("ERROR: ',' appears before first string. \n" );
		return 1;
	}
	last = 0;
	token = strtok(NULL , DELIM);
	while( token != NULL)
	{
		if(!strcmp( "," , token ) && last == 1)
		{
			printf( "ERROR: a few commas in a row.\n" );
			return 1;
		} 
		if( strcmp( "," , token ) != 0  && last == 0)
		{
			printf( "ERROR: two strings with no comma in between.\n" );
			return 1;
		}
		else
		{
			if( last == 0 )
				last = 1;
			else
				last = 0;
			token = strtok(NULL , DELIM);
		}
	}
	if( last == 1)
	{
		printf("ERROR: last string is a comma.\n");
		return 1;
	}
	return 0;
}
void make_cpy_line(char* line, char *cpy_line){/*varified function*/

	int i, j; i = j = 0;
	while (line[i] != '\0')
	{
		if( line[i] != ',')
		{
			cpy_line[j] = line[i];
			i++; j++;
		}
		else
		{
			cpy_line[j++] = ' ';
			cpy_line[j++] = ',';
			cpy_line[j++] = ' ';
			i++;
		}
	}
	cpy_line[j] = '\0';
}

/* function gets the part of line that starts right after.string
	currently assuming all chars are ascii valid.*/
int validate_string(char* line)
{
	char *token , cpy_line[MAX_LINE_SIZE]={0};
	if(line == NULL)
		return 1;
	make_cpy_string(cpy_line, line);
	token = strtok(cpy_line , "*");
	if (token == NULL){
		printf("ERROR: no word after .string\n");
		return 1;
	}
	if(strcmp(token , "\"") != 0){
		printf("ERROR: word after .string doesn't start with double quote.\n");
		return 1;
	}
	token = strtok(NULL , "*");
	if (token == NULL){
		printf("ERROR: word after .string is just double quote.\n");
		return 1;
	}
	token = strtok(NULL , "*");
	if (token == NULL){
		printf("ERROR: word after .string has no double quote at the end.\n");
		return 1;
	}
	if(strcmp(token , "\"") != 0){
		printf("ERROR: word after .string doesn't start with double quote.\n");
		return 1;
	}
	token = strtok(NULL , DELIM);
	if (token != NULL){
		printf("ERROR: line should end after the string.\n");
		return 1;
	}
	return 0;
}
void make_cpy_string(char *cpy_line , char *line)
{
	int i, j; i = j = 0;
	while (line[i] != '\0')
	{
		if( line[i] != '"')
		{
			cpy_line[j] = line[i];
			i++; j++;
		}
		else
		{
			cpy_line[j++] = '*';
			cpy_line[j++] = '"';
			cpy_line[j++] = '*';
			i++;
		}
	}
	cpy_line[j] = '\0';
}

int validate_entry_and_extern(struct entry_extern_node *ptr, char* line)
{
	int flag = 0;
	char* token , cpy_line[MAX_LINE_SIZE]={0};
	struct entry_extern_node *prev = NULL;
	struct entry_extern_node *new_node =NULL;
	if(line == NULL)
		return 1;
	strcpy(cpy_line, line);
	token = strtok(cpy_line , DELIM);
	if(token == NULL)
	{
		return 1;
	}
	while(  ptr != NULL && flag == 0 )
	{
		if( strcmp( ptr->name , token) ==0)
		{
			printf("ERROR: label already defined as entry / extern.\n");
			flag = 1;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	new_node = malloc(sizeof(struct entry_extern_node));
	strcpy(new_node->name, token);
	new_node->next = NULL;
	if (flag == 0)
	{
		if (prev == NULL)
			ptr = new_node;
		else
			prev->next = new_node;
	}
	else
		free(new_node);
	token = strtok(NULL , DELIM);
	if(token != NULL)
	{
		printf("ERROR: line has too many tokens.\n");
		return 1;
	}
	return flag;
}
int is_new_label(struct label_node *ptr, char* line)
{
	int flag = 0;
	struct label_node *prev = NULL;
	struct label_node *new_node;
	char* token , cpy_line[MAX_LINE_SIZE]={0};
	if(line == NULL)
		return 1;
	strcpy(cpy_line, line);
	token = strtok(cpy_line , DELIM);
	if(token == NULL)
		return 1;
	while(  ptr != NULL && flag == 0 )
	{
		if( strcmp( ptr->name , token) ==0)
		{
			printf("ERROR: label already defined.\n");
			flag = 1;
		}
		prev = ptr;
		ptr = ptr->next;
	}

	new_node = malloc(sizeof(struct label_node));
	strcpy(new_node->name, token);
	new_node->next = NULL;
	if (flag == 0)
	{
		if (prev == NULL)
			ptr = new_node;
		else
			prev->next = new_node;
	}
	else
		free(new_node);
	return flag;
}
char* exclude_first_two_tokens(char* s) { /*varified function*/
    char* copy = NULL;
    char* token,cpy_line[MAX_LINE_SIZE]={0};
    int i = 0;
        strcpy(cpy_line , s);
    /* Get first two tokens*/
    for (i = 0; i < 2; i++)
    {
        token = strtok(i == 0?(char*)cpy_line : NULL, DELIM);
        if (token == NULL) {
            /* Less than two words in the string*/
            return NULL;
        }
    }
    /* Copy the rest of the string*/
    while ((token = strtok(NULL, DELIM)) != NULL) {
        if (copy == NULL) {
            copy = (char*)malloc(strlen(token) + 1);
            strcpy(copy, token);
        } else {
            copy = (char*)realloc(copy, strlen(copy) + strlen(token) + 2);
            strcat(copy, " ");
            strcat(copy, token);
        }
    }
    return copy;
}
char* exclude_first_token(char* s)
{/*varified function*/
	char* copy = NULL;
	char* token,cpy_line[MAX_LINE_SIZE]={0};
	strcpy(cpy_line , s);
    /* Get first token*/
	token = strtok(cpy_line, DELIM);
	if (token == NULL)
	{
		return NULL;
	}
    /* Copy the rest of the string*/
	while ((token = strtok(NULL, DELIM)) != NULL)
	{
		if (copy == NULL)
		{
			copy = (char*)malloc(strlen(token) + 1);
			strcpy(copy, token);
		}
		else
		{
			copy = (char*)realloc(copy, strlen(copy) + strlen(token) + 2);
			strcat(copy, " ");
			strcat(copy, token);
		}
	}
	return copy;
}
char* exclude_first_two_tokens_and_commas(char* s) { /*varified function*/
    char* copy = NULL;
    char* token,cpy_line[MAX_LINE_SIZE]={0};
    int i = 0;
	strcpy(cpy_line , s);
    /* Get first two tokens*/
    for (i = 0; i < 2; i++)
    {
        token = strtok(i == 0 ? (char*)cpy_line : NULL, DELIM_COMMA);
        if (token == NULL)
        {
            /* Less than two words in the string*/
            return NULL;
        }
    }
    /* Copy the rest of the string*/
    while ((token = strtok(NULL, DELIM_COMMA)) != NULL)
    {
        if (copy == NULL)
        {
            copy = (char*)malloc(strlen(token) + 1);
            strcpy(copy, token);
        }
        else
        {
            copy = (char*)realloc(copy, strlen(copy) + strlen(token) + 2);
            strcat(copy, " ");
            strcat(copy, token);
        }
    }
    return copy;
}
char* exclude_first_token_and_commas(char* s)
{/*varified function*/
	char* copy = NULL;
	char* token,cpy_line[MAX_LINE_SIZE]={0};
	strcpy(cpy_line , s);
    /* Get first token*/
	token = strtok(cpy_line, DELIM_COMMA);
	if (token == NULL)
	{
		return NULL;
	}
    /* Copy the rest of the string*/
	while ((token = strtok(NULL, DELIM_COMMA)) != NULL)
	{
		if (copy == NULL)
		{
			copy = (char*)malloc(strlen(token) + 1);
			strcpy(copy, token);
		}
		else
		{
			copy = (char*)realloc(copy, strlen(copy) + strlen(token) + 2);
			strcat(copy, " ");
			strcat(copy, token);
		}
	}
	return copy;
}
int is_note_line(char *line)
{
	if(line[0] == '#')
		return 0;
	return 1; 
}
int is_white_space_line(char *line){
	char *token ,cpy_line[MAX_LINE_SIZE]={0};
	if(line == NULL)
		return 1;

	strcpy(cpy_line , line);
	token = strtok(cpy_line, DELIM); /*DELIM includes all white spaces, if line is only
					white spaces then token would be null. */
	if(token == NULL)
		return 0;
	return 1;
}
void clear_labels_list()
{
    struct label_node* current = analyze_lbalel_head_node.next;
    struct label_node* temp = NULL;
    strcpy(analyze_lbalel_head_node.name,"\0");
    while(current != NULL)
    {
        temp = current;
        current = current->next;

        free(temp);
    }
    analyze_lbalel_head_node.next =NULL;
}
void clear_entry_extern_list()
{
    struct entry_extern_node* current = analyze_ent_ext_head_node.next;
    struct entry_extern_node* temp = NULL;
    strcpy(analyze_ent_ext_head_node.name,"\0");
    while(current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
    analyze_ent_ext_head_node.next =NULL;
}
/* Function to check for double names in the entry_extern_node and label_node linked lists */
int check_double_names()
{
    /* Initialize variables */
    struct entry_extern_node* ent_ext_ptr = analyze_ent_ext_head_node.next;
    struct label_node* label_ptr = analyze_lbalel_head_node.next;
    int found_duplicate = 0;
    /* Loop through the entry_extern_node linked list */
    while (ent_ext_ptr != NULL)
    {
        /* Loop through the label_node linked list */
        while (label_ptr != NULL)
        {
            /* Compare the names in the two nodes */
            if (strcmp(ent_ext_ptr->name, label_ptr->name) == 0) {
                /* If a duplicate name is found, set found_duplicate to 1 and print an error message */
                found_duplicate = 1;
                printf("Error: The name \"%s\" already defined.\n", ent_ext_ptr->name);
            }
            label_ptr = label_ptr->next;
        }
        ent_ext_ptr = ent_ext_ptr->next;
    }
    /* Return 1 if a duplicate name was found, 0 otherwise */
    return found_duplicate;
}
