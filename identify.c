#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "identify.h"
#include "commands.h"

/* all functions return 0 for true if not specified else. */

/*function checks if token is of format "word:" */
int is_label(char* token){
	int l;
	l = strlen( token);
	return !(token[l-1] == ':');
}
/*function checks if the given token is a command or not. */
int is_command(char* token)
{
	int i = 0, flag = 1 ;
		char commands[OPCODE_ARR_LENGTH][4] = { "mov" , "cmp" , "add" , "sub" , "not" , "clr" , "lea" ,
		"inc" , "dec" , "jmp" , "bne" , "red" , "prn" , "jsr" , "rts" } ; 
	while(flag!= 0 && i < OPCODE_ARR_LENGTH-1)
	{
		if ( strcmp( token , commands[i])==0)
			flag = 0;
		i++;
	}
		if (strcmp( token , "stop")==0)
			flag = 0;
	return flag;
}
int is_string(char* token){
	return strcmp(token, ".string");
}
int is_data(char* token){
	return strcmp(token, ".data"); 
}
int is_entry(char* token){
	return strcmp(token, ".entry");								
}

int is_extern(char* token){
	return strcmp(token, ".extern");								
}
/*function checks if the give string is of format: #number */
int is_immediate(char *str){ /*verified */
	if(str == NULL)
		return 1;
	if(str[0] == '#')
		if(is_num(str+1) == 0){
			return 0;
		}
return 1;
}
/*function checking if the given string represents a number */
int is_num(char *str){/*varified function*/
	int i;
	if (!isdigit(str[0]) && str[0] != '-' && str[0] != '+')
		return 1;
	else for( i = 1 ; i < strlen(str) ; i++)
		if( isdigit(str[i])==0 )
			return 1;
	return 0 ;
}
/* function checking if the given string can be a label's name, rull out it being a reserved word or irrelevant syntax.*/
int not_reserved(char *str){/*varified*/

	if(str == NULL)
		return 1;
	if( is_reg(str) != -1)
		return 1;
	if( is_command(str) == 0)
		return 1;
	if( is_immediate(str) ==0)
		return 1;
	if( no_trash(str) != 0)
		return 1;
	return 0; /*param is not a reserved word and not null. therefore it could be
			a label's name. */
}
/*function checks if the given string is representing a register's name */
int is_reg( char *str) {/*varified*/
	if(str == NULL)
		return -1;
	if (strcmp(str, "r0") == 0) {
		return R0;
	} else if (strcmp(str, "r1") == 0) {
	        return R1;
	} else if (strcmp(str, "r2") == 0) {
		return R2;
	} else if (strcmp(str, "r3") == 0) {
        	return R3;
	} else if (strcmp(str, "r4") == 0) {
	        return R4;
	} else if (strcmp(str, "r5") == 0) {
	        return R5;
	} else if (strcmp(str, "r6") == 0) {
	        return R6;
	} else if (strcmp(str, "r7") == 0) {
	        return R7;
	} else {
	        return NON_REG;
	}
}
/* function calls for all the relevant checks for the given command line */
int validate_command(char *line){

	int index;
	index = what_command(line);
	if(command_params(index ,line) != 0)
	{
		printf("ERROR: in line format\n");
		return 1;
	}
	return 0;
	if( first_param_types( index , line) != 0 )
	{
		printf("ERROR: first param does not fit\n");
		return 1;
	}

	if( second_param_types( index , line) != 0 )
	{
		printf("index is: %d\n",index);

		printf("ERROR: second param does not fit\n");

		return 1;
	}
	return 0;
}
/*function returns index of command if it's a command, -1 else*/
int what_command(char *line){ 
	char* token, cpy_line[MAX_LINE_SIZE]={0};
	int i = 0;
	if(line == NULL)
		return -1;
	strcpy(cpy_line, line);
	token = strtok(cpy_line, DELIM);
	for (i = 0; i < OPCODE_ARR_LENGTH; i++) {
		if (strcmp(table[i].cmd, token) == 0)
			return i;
	}
	return -1;
}	
/*function validates of the structure of the commands */
int command_params(int cmd_num, char *line)
{
	char line_second[MAX_LINE_SIZE]={0} ;
	char* result;
	if(line == NULL || cmd_num == -1)
		return -1;
	result =exclude_first_token(line);
	if(table[cmd_num].params_num == ONE_PARAM )
	{

		if(result != NULL)
		{
			strcpy(line_second , result);
			free(result);
			return check_one_param( line_second);
		} 
		else if(result == NULL)
		{
			printf("ERROR: no parameters.\n");
		}
	}

	if(table[cmd_num].params_num == TWO_PARAMS )
	{
		if(result != NULL)
		{
			strcpy(line_second , result);
			free(result);
			return check_two_params( line_second);
		}

	}
	if(table[cmd_num].params_num == LABEL_NON_OR_TWO_PARAMS)
	{
		if(result != NULL)
		{
			strcpy(line_second , result);
			if(strchr(line_second,',')!=NULL)
			{
				if(check_label_and_two_params(line_second)==0)
				{
					free(result);
					return 0;
				}
			}
			else
			{
				if(check_label_no_params(line_second) == 0)
				{
					free(result);
					return 0;
				}
			}
		}
	}

	if(table[cmd_num].params_num == NO_PARAMS)
	{
		if(result == NULL)
		{
			free(result);
			return 0;
		}
	}
	free(result);
	return 1;
}

/*function checks if the addressing type of the first parameter is suitable for its command. */
int first_param_types( int cmd_num , char *line){/*verified*/
	char *token,*result, cpy_line[MAX_LINE_SIZE]={0}, line_second[MAX_LINE_SIZE]={0};
	if(line == NULL)
		return 1;
	if(table[cmd_num].params_num == LABEL_NON_OR_TWO_PARAMS) /*specific check was already made in "check_label_and_two_params" and "check_label_no_params" */
		return 0;
	strcpy(cpy_line, line);
	result = exclude_first_token_and_commas(line);
	if(result == NULL)
	{
		if(table[cmd_num].source ==  NO_ADDRESSINGS_TYPES)
		{
			free(result);
			return 0;
		}
		else
		{
			free(result);
			printf("ERROR: no parameters.\n");
			return 1;
		}	
	}
	strcpy( line_second , result);
	if(table[cmd_num].source == ALL_BUT_TWO)
	{
		token = strtok(line_second , DELIM_COMMA);
		free(result);
		return is_param(token);
	}
	if(table[cmd_num].source== ONLY_ONE)
	{
		token = strtok(line_second , DELIM_COMMA);
		free(result);
		return not_reserved(token);
	}
	if(table[cmd_num].source== NO_ADDRESSINGS_TYPES)
	{
		free(result);
		return 0;
	}
	return 1;
}
/*function checks if the addressing type of the second parameter is suitable for its command. */ 
int second_param_types(int cmd_num , char *line)
{ /*verified*/
	char *token,*result,*result2,*result3, cpy_line[81]={0},line_second[MAX_LINE_SIZE]={0},line_third[MAX_LINE_SIZE]={0};
	if(table[cmd_num].params_num == LABEL_NON_OR_TWO_PARAMS) /*specific check was already made in "check_label_and_two_params" and "check_label_no_params" */
		return 0;	
	result = exclude_first_two_tokens_and_commas(line);
	if(result == NULL)
	{
		if(table[cmd_num].dest ==  NO_ADDRESSINGS_TYPES || table[cmd_num].source ==  NO_ADDRESSINGS_TYPES)
		{
			free(result);
			return 0;
		}
	}
	result2 = exclude_first_token_and_commas(line);
	if( result2!= NULL)
	{
		free(result2);
		strcpy(line_second , result2);
	}
	result3 = exclude_first_two_tokens_and_commas(line);
	if(result3 != NULL)
	{
		free(result3);
		strcpy(line_third ,result3);
	}
	strcpy(cpy_line, line);
	free(result);
	if(table[cmd_num].dest == ONE_AND_THREE && table[cmd_num].source == NO_ADDRESSINGS_TYPES)
	{
		token = strtok(line_second, DELIM_COMMA);
		if(token ==  NULL)
			return 1;
		if(not_reserved(token) == 0 || is_reg(token) != -1){
			token = strtok(NULL , DELIM_COMMA);
			if(token != NULL){
				printf("ERROR: too many parmeters.\n");
				return 1;
			}
			return 0;
		}
		else {
			printf("ERROR: wrong parameter type.\n");
			return 1;
		}
	}
	if(table[cmd_num].dest == ONE_AND_THREE && table[cmd_num].source != NO_ADDRESSINGS_TYPES){
		token = strtok(line_third, DELIM_COMMA);
		if(token ==  NULL)
			return 1;
		if(not_reserved(token) == 0 || is_reg(token) != -1){
			token = strtok(NULL , DELIM_COMMA);
			if(token != NULL){
				printf("ERROR: too many parmeters.\n");
				return 1;
			}
			return 0;
		}
		else {
			printf("ERROR: wrong parameter type.\n");
			return 1;
		}
	}
	if(table[cmd_num].dest == ALL_BUT_TWO){
		token = strtok(line_third, DELIM_COMMA);
		if(token ==  NULL)
			return 1;
		return is_param(token);	
	} 
	if(table[cmd_num].dest == ONE_AND_TWO){
		if(check_label_and_two_params( line) == 0  || check_label_no_params(line) == 0 )
			return 0;
		token = strtok(line_third, DELIM_COMMA);
		if(token ==  NULL)
			return 1;
		if(not_reserved(token) == 0)
			return 0;
		else return 1;
	}

	return 1;
}
/*function checks if line is of format: command dest_param */ 
int check_one_param(char* line){
	char* token,cpy_line[MAX_LINE_SIZE]={0};
	if(line == NULL)
		return 1;
	strcpy(cpy_line, line);
	token = strtok(cpy_line, DELIM);
	if( token == NULL){
		printf("ERROR: no parameters.\n");
		return 1;
	}
	token = strtok(NULL, DELIM_COMMA);
	if(token != NULL){
		printf("ERROR: shouldn't be more than one parameter.\n");
		return 1;
	}
	return 0;

}
/*function checks if line is of format: command source_param , dest_param */ 
int check_two_params(char* line)
{
	char* token,cpy_line[MAX_LINE_SIZE]={0};
	if(line == NULL)
		return -1;

	make_cpy_line( line, cpy_line);
	if( alternatly_string_comma(line) != 0)
	{
		return 1;
	}

	token = strtok(cpy_line, DELIM_COMMA);
	if( token == NULL){
		printf("ERROR: no parameters.\n");
		return 1;
	}
	if( is_param(token) != 0){
		printf("ERROR: invalid first parameter.\n");
		return 1;
	}
	token = strtok(NULL, DELIM_COMMA);
	if(token == NULL){
		printf("ERROR: should be more than one parameter.\n");
		return 1;
	}
	if( is_param(token) != 0){
		printf("ERROR: invalid second parameter.\n");
		return 1;
	}
	token = strtok(NULL, DELIM_COMMA);
	if(token != NULL){
		printf("ERROR: shouldn't be more than two parameters.\n");
		return 1;
	}
	return 0;
}
/*function checks if line is of format: command LABEL_NAME (source_param,dest_param) */ 
int check_label_and_two_params(char* line)
{
	char* token,cpy_line[MAX_LINE_SIZE]={0};
	if(line == NULL)
		return -1;
	strcpy(cpy_line, line);
	if(check_prn_comma(line) != 0)
	{
		printf("ERROR: format does not suite the command.\n");
		return 1;
	} /*if dislocated comma or parenthesies function return 1 */
	token = strtok(cpy_line, DELIM_LABEL);
	if(token == NULL){
		printf("ERROR: format does not suite the command.\n");
		return 1;
	}
	if(not_reserved(token) == 0){
		token = strtok(NULL, DELIM_LABEL);
		if( token == NULL){
			printf("ERROR: format does not suite the command.\n");
			return 1;
		}
		if(is_param(token) == 0){
			token = strtok(NULL, DELIM_LABEL);
			if( token == NULL){
				printf("ERROR: format does not suite the command.\n");
				return 1;
			}
			if( is_param(token) == 0){
				token = strtok(NULL, DELIM_LABEL);
				if(token != NULL){
					printf("ERROR: format does not suite the command.\n");
					return 1;
				}
			}
		}	
	}
	return 0;
}
/*function checks if line is of format: command LABEL_NAME */ 
int check_label_no_params( char *line)
{
	char* token, cpy_line[MAX_LINE_SIZE]={0};
	if(line == NULL)
		return -1;
	strcpy(cpy_line, line);
	token = strtok(cpy_line , DELIM_LABEL);
	if( token == NULL){
		printf("ERROR: format does not suite the command.\n");
		return 1;
	}
	if (not_reserved(token) != 0){
		printf("ERROR: \"%s\" can't be a label.\n", token);
		return 1;
	}
	token = strtok(NULL, DELIM_LABEL);
	if(token == NULL)
		return 0;
	printf("ERROR: format does not suite the command.\n");
	return 1;
}
/*function checks if the string is all digits and letters (or starts with '#').*/
int no_trash(char *str){ /*varified*/
	int i =0;
	if(str==NULL)
		return 1;
	if( isalpha(str[i]) == 0 && isdigit(str[i]) == 0 && str[i] != '#')
		return 1;
	if(str[i] == '#')
		for(i=1; str[i] != '\0'; i++) /*token should only be a number after #*/
			if(isdigit(str[i]) == 0 )
				return 1;
	for(i=1; str[i] != '\0'; i++)
		if(isalpha(str[i]) == 0 && isdigit(str[i]) == 0 )
			return 1;
	return 0; 
}
/*function checks if the given string can be a parameter or not*/
int is_param( char* str){ 
	if( is_reg(str) != -1 || is_immediate(str) == 0 || not_reserved(str) == 0)
		if( is_command(str) != 0)
			return 0;
	return 1;
}
/*function checks if the the given line (excluding its first token) is of format: LABEL (source,dest)*/
int check_prn_comma( char* line)
{  /*varified*/
	char *token, cpy_line[MAX_LINE_SIZE]={0}, *toktoken;
	int a, b, c;
	if(line == NULL)
	{
		return 1;
	}


	strcpy(cpy_line, line);

	token = strtok(cpy_line, DELIM_LABEL);
	if(token == NULL)
	{


		return 1;
	}
	if(not_reserved(token) != 0)
	{ /*first token- implied label's name */

		return 1;
	}
	token = strtok(NULL, "\n");
	if(token == NULL)
	{
		return 1;
	}
	c = strlen(token);
	if(token[strlen(token)-1] != ')' || strlen(token) < 4 )
	{
		printf("%s",token);
		return 1;
	}
	toktoken = strtok( token , "(,");
	a = strlen(toktoken);
	if( toktoken == NULL)
	{

		printf("ERROR: no parameters.\n");
		return 1;
	}
	if(is_param(toktoken) == 0){
		toktoken = strtok(NULL, " )");
		if(toktoken == NULL)
		{

			printf("ERROR: missing parameter.\n");
			return 1;
		}
		b = strlen(toktoken);
		if( a + b + 3 <  c ){
			printf("ERROR: white space or unnecessery parenthesis or comma.\n");
			return 1;	
		}
		if(is_param(toktoken) != 0){
			printf("ERROR: param is invalid.\n");
			return 1;
		}
		token = strtok(NULL, DELIM); /*check if line ends after relevant token */
		if(token == NULL)
			return 0;
	}
	printf("ERROR: format in not: command label (source_op,dest_op)\n");
	return 1;
}
