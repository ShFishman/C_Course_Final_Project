#include "global_helpers.h"
#include "code_image.h"
#include "data_image.h"
#include "table.h"
#include "second_pass.h"
#include "first_pass.h"
int IC,DC;
/* Executes the first pass on the input file, reading and processing each line
 *,and updates the IC and DC counters accordingly.*/
int execute_first_pass(FILE *post_macro_file_ptr)
{
	char c;
	char line[MAX_LINE_SIZE] = {0};
	int i =0 ;
	IC=LINE_NUM_START-1;DC=0;
	code_image_head = create_new_code_image_node(NULL) ;
	data_image_head = create_new_data_image_node(NULL) ;
	extern_list_head = create_table_node(NULL);
	entry_list_head = create_table_node(NULL);
	symbol_list_head = create_table_node(NULL);
	fseek(post_macro_file_ptr,0,SEEK_SET);
	while((c=fgetc(post_macro_file_ptr))!=EOF)
	{
		if(c=='\n')
		{
			line[i]='\0';
			i=0;
			if(!is_empty_or_whitespace(line)&& !is_comment(line))
			{

				encode_line(line);
			}
		}
		else
		{
			line[i]=c;
			i++;
		}
	}
	return EXIT_SUCCESS;
}
/* Encodes a single line of the input file,
 * parsing the labels and commands, and then encoding the code, data,
 * or entry based on the command type. */
void encode_line(char* line)
{
	char* token,*label,*cmd;
	cmd_tpye current_cmd_type;
	char line_cpy[MAX_LINE_SIZE];
	token=label=cmd=NULL;
	strcpy(line_cpy,line);
	token = strtok(line_cpy,DELIM);
	/* if label*/
	if(token)
	{
		if(token[strlen(token)-1]==':')
		{
			token[strlen(token)-1] = '\0';
			label = token;
			line = get_next_word(line,token);
			token = strtok(NULL,DELIM);
		}
		cmd = token;
	}
	current_cmd_type = find_line_type(cmd);
	if(label!=NULL)
		encode_label(label,current_cmd_type);
	if(current_cmd_type == CODE)
		encode_code(line);
	else if(current_cmd_type ==DATA)
		encode_data(line);
	else  if(current_cmd_type ==ENTERY)
		encode_ent(line);
}
/*  Encodes a line of code, handling the parameters and creating new code image nodes accordingly. */
void encode_code(char *line)
{
	char line_cpy[MAX_LINE_SIZE], *token, parameters[4][MAX_LINE_SIZE];
    code_image_node *current_node = create_new_code_image_node(code_image_head);
    strcpy(line_cpy, line);
    token = strtok(line_cpy, DELIM);
    current_node->a.opcode = get_opcode_num(token); /* encode opcode */
    token = strtok(NULL, DELIM);
    /* get parameters */
    while (token != NULL)
    {
        strcpy(parameters[current_node->par_counter], token);
        current_node->par_counter++;
        token = strtok(NULL, DELIM);
    }
    /* handle parameters */
    if (current_node->par_counter == 3)
    {
        current_node->a.target_adressing = JMP_W_PAR_ADRESS;
        current_node->a.par1 = get_parameter_type(parameters[1]);
        current_node->a.par2 = get_parameter_type(parameters[2]);
        current_node->b[2] = encode_other_word(parameters[2], REG_PAR_DEST_STATE);
        current_node->b[1] = encode_other_word(parameters[1], REG_PAR_SOURCE_STATE);
    }
	else if(current_node->par_counter == 2)
    {
        current_node->a.source_adressing = get_parameter_type(parameters[0]);
		current_node->a.target_adressing = get_parameter_type(parameters[1]);
		current_node->b[1] = encode_other_word(parameters[1], REG_PAR_DEST_STATE);
    }
	else if(current_node->par_counter == 1)
		current_node->a.target_adressing = get_parameter_type(parameters[0]);

    if(current_node->par_counter>0)
    {
    	 /* encode the first word */
    	current_node->b[0] = encode_other_word(parameters[0], REG_PAR_SOURCE_STATE);
		/* check if it's a case of two registers parameters */
		special_register_encoding(current_node);
		set_second_words_label_ic(current_node);
    }
    IC+=(1+current_node->par_counter);
}
/*Sets the address of the second word for each parameter in the code image node.*/
void set_second_words_label_ic(code_image_node* current_node)
{
	int i;
	for(i=0;i<current_node->par_counter;i++)
		current_node->b[i].adress+=(i+1);
}
/*Handles special cases for register encoding in a code image node.*/
void special_register_encoding(code_image_node* current_node)
{
	if(current_node->a.par1==REGISTER_ADRESS && current_node->a.par2==REGISTER_ADRESS)
	{
		current_node->b[1].data.split_case.dest_reg = current_node->b[2].data.split_case.dest_reg;
		current_node->par_counter--;
	}
	else if(current_node->a.source_adressing==REGISTER_ADRESS && current_node->a.target_adressing==REGISTER_ADRESS)
	{
		current_node->b[0].data.split_case.dest_reg=current_node->b[1].data.split_case.dest_reg;
		current_node->par_counter--;
	}
}
/*Encodes a single parameter of a code line based on its type and returns an other_word struct.*/
other_word encode_other_word(char* par,DEST_OR_SOURCE_REG_FLAG par_state)
{
	int sign = 1;
	other_word to_return;
	if(par[0]=='#')
	{
		par[0]='0';
		if(par[1]=='-') /* handle negetive case*/
		{
			par[1]='0';
			sign =-1;
		}
		to_return.data.normal_case.val= atoi(par)*sign;
		to_return.ARE=0;
	}
	else if(par[0]=='r')
	{
		par[0]='0';
		if(par_state==REG_PAR_SOURCE_STATE)
			to_return.data.split_case.source_reg=atoi(par);
		else if(par_state==REG_PAR_DEST_STATE)
			to_return.data.split_case.dest_reg=atoi(par);
		to_return.ARE=0;
	}
	else
	{
		to_return.adress =IC+1;
		strcpy(to_return.label,par);
		to_return.ARE=3;
	}

	return to_return;
}
/*Encodes a label by creating a new table node and assigning the label name, address, and type.*/
void encode_label(char* label,cmd_tpye type)
{
	table_node *ptr = create_table_node(symbol_list_head);
	strcpy(ptr->name,label);
	ptr->type = type;
	if(type ==CODE)
		ptr->address =IC+1;
	else if(type ==DATA)
		ptr->address =DC+1;
}
/*Checks if the current code image node represents a jump command with parameters.*/
int is_jump_with_par(code_image_node *current_node)
{
	if(current_node->a.opcode==9||current_node->a.opcode==10||current_node->a.opcode==13)
		return 1;
	return 0;
}
/* Encodes a line of data by creating new data image nodes and updating the DC counter.*/
void encode_data(char* line)
{
	char linecpy[MAX_LINE_SIZE],*token,*char_ptr;
	data_image_node* ptr;
	strcpy(linecpy,line);
	token = strtok(linecpy,DELIM);
	if(!strcmp(token,".data"))
	{
		token = strtok(NULL,DELIM);
		while(token!=NULL)
		{
			ptr= create_new_data_image_node(data_image_head);
			ptr->data = atoi(token);
			DC++;
			token = strtok(NULL,DELIM);
		}
 	}
	else
	{
		token = strtok(NULL,DELIM);
		char_ptr =token;
		while(*char_ptr!='\0')
		{
			ptr= create_new_data_image_node(data_image_head);
			ptr->data = *char_ptr;
			DC++;
			char_ptr++;
		}
		ptr= create_new_data_image_node(data_image_head);
		ptr->data ='\0';
		DC++;
	}
}
/* Encodes an entry line by creating a new table node and assigning the entry name.*/
void encode_ent(char* line)
{
	table_node *ptr = create_table_node(entry_list_head);
	line = get_next_word(line,".entry");
	strcpy(ptr->name,line);
}
