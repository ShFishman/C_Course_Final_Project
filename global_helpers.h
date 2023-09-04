#ifndef GLOBAL_HELPERS_H_
#define GLOBAL_HELPERS_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define DELIM " \t\n\"(),"
#define DELIM_COMMA " \t\n,"
#define DELIM_LABEL ", ()\t\n"
#define MAX_LABEL_SIZE 30
#define MAX_LINE_SIZE 81
#define MAX_FILE_NAME_SIZE 100
#define LINE_NUM_START 100
#define OPCODE_ARR_LENGTH 16

/* Enum representing the command type */
typedef enum  {CODE,DATA,ENTERY,EXTERNAL} cmd_tpye;

/* Enum representing the addressing type */
typedef enum  {IMMEDIATE_ADRESS,DIRECT_ADRESS,JMP_W_PAR_ADRESS,REGISTER_ADRESS} adressing_tpye;

/* Table of opcodes */
char opcodes_table[OPCODE_ARR_LENGTH][5];

/* Returns the next word in the string and updates the string pointer. */
char *get_next_word(char *string1, char *string2);

/* Returns the addressing type of the given parameter. */
adressing_tpye get_parameter_type(char *par);

/* Determines the type of the given command. */
cmd_tpye find_line_type(char* cmd);

/* Returns the opcode number of the given opcode name. */
int get_opcode_num(char* name);

/* Checks if the given string is empty or contains only whitespace. */
int is_empty_or_whitespace(const char *line);

/* Checks if the given string is a comment. */
int is_comment(const char *str);

#endif /* GLOBAL_HELPERS_H_ */
