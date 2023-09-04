#include "global_helpers.h"
char opcodes_table[OPCODE_ARR_LENGTH][5] = {
    "mov",
    "cmp",
    "add",
    "sub",
    "not",
    "clr",
    "lea",
    "inc",
    "dec",
    "jmp",
    "bne",
    "red",
    "prn",
    "jsr",
    "rts",
    "stop",
};

char *get_next_word(char *string1, char *string2)
{
    char *result = strstr(string1, string2);
    if (result)
    {
        result += strlen(string2);
        while (*result == ' ' || *result == '\t' ||*result == ':' || *result == '\n' || *result == '\r')
        	result++;
    }
    return result;
}

adressing_tpye get_parameter_type(char *par)
{
	if(par[0]=='#')
		return IMMEDIATE_ADRESS;
	/*check if its register*/
	else if(par[0]=='r')
		return REGISTER_ADRESS;
	/* must be label*/
	else
		return DIRECT_ADRESS;
}

cmd_tpye find_line_type(char* cmd)
{
	if(!strcmp(cmd,".data")||!strcmp(cmd,".string"))
		return DATA;
	else if(!strcmp(cmd,".entry"))
		return ENTERY;
	else if(!strcmp(cmd,".extern"))
		return EXTERNAL;
	else
		return CODE;
}

int get_opcode_num(char* name)
{
	int i =0;
	while(strcmp(name,opcodes_table[i])!=0)
		i++;
	return i;
}

int is_empty_or_whitespace(const char *line) {
    const char *p = line;
    while (*p && isspace((unsigned char)*p))
        ++p;
    return (*p == '\0');
}

int is_comment(const char *str)
{
    while (isspace((unsigned char)*str)) {
        str++;
    }
	return (*str == ';');
}
