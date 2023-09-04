#ifndef UNTITLED_IDENTIFY_H
#define UNTITLED_IDENTIFY_H

#define DELIM " \t\n"
#define DELIM_COMMA " \t\n,"
#define DELIM_LABEL ", ()\t\n"
#define MAX_LABEL_SIZE 30
#define MAX_LINE_SIZE 81
#define MAX_FILE_NAME_SIZE 100
#define LINE_NUM_START 100
#define OPCODE_ARR_LENGTH 16

/* Checks if the given token represents data. */
int is_data(char* token);

/* Checks if the given token represents a string. */
int is_string(char* token);

/* Checks if the given token represents a command. */
int is_command(char* token);

/* Checks if the given token represents a label. */
int is_label(char* token);

/* Checks if the given token represents an entry. */
int is_entry(char* token);

/* Checks if the given token represents an extern. */
int is_extern(char* token);

/* Checks if the given string is not a reserved word. */
int not_reserved(char* str);

/* Checks if the given string is a register name. */
int is_reg( char *str);

/* Checks if the given string is a number. */
int is_num(char *token);

/* Checks if the given string is an immediate value. */
int is_immediate(char *str);

/* Determines the command number of the given command line. */
int what_command(char *line);

/* Checks if the given command line has one parameter. */
int check_one_param(char* line);

/* Checks if the given command line has two parameters. */
int check_two_params(char* line);

/* Checks if the given command line has a label and two parameters. */
int check_label_and_two_params(char* line);

/* Checks if the given command line has a label and no parameters. */
int check_label_no_params( char *line);

/* Returns the addressing type of the first parameter of the given command. */
int first_param_types( int cmd_num , char *line);

/* Returns the addressing type of the second parameter of the given command. */
int second_param_types(int cmd_num , char *line);

/* Returns a copy of the given line with the first two tokens excluded. */
char* exclude_first_two_tokens( char* s);

/* Returns a copy of the given line with the first token excluded. */
char* exclude_first_token(char* s);

/* Returns a copy of the given line with the first two tokens and commas excluded. */
char* exclude_first_two_tokens_and_commas(char* s);

/* Returns a copy of the given line with the first token and commas excluded. */
char* exclude_first_token_and_commas(char* s);

/* Checks if the given string contains only valid characters. */
int no_trash(char *str);

/* Checks if the given string is a parameter. */
int is_param( char* str);

/* Checks if the commas in the given line alternate with strings. */
int alternatly_string_comma(char *line);

/* Makes a copy of the given line. */
void make_cpy_line(char* line, char *cpy_line);

/* Checks if the given command line has a single comma inside a .prn parameter. */
int check_prn_comma( char* line);

/* Returns the number of parameters of the given command. */
int command_params(int cmd_num, char *line);

/* Checks if the given line is a note (i.e. starts with ";"). */
int is_note_line(char *line);

/* Checks if the given line contains only whitespace. */
int is_white_space_line(char *line);

#endif /* UNTITLED_IDENTIFY_H */
