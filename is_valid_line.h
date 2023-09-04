#ifndef IS_VALID_LINE_H
#define IS_VALID_LINE_H

/* Validates the given file and returns 1 on success, 0 otherwise. */
int validating_file(FILE *fp);

/* Checks if the given token represents data. */
int is_data(char* token);

/* Checks if the given token represents a string. */
int is_string(char* token);

/* Checks if the given token represents a command. */
int is_command(char* token);

/* Checks if the given token represents a label. */
int is_label(char* token);

/* Checks for double names in the symbol table.
 * Returns 1 on success, 0 otherwise.
 */
int check_double_names();

/* Makes a copy of the given string. */
void make_cpy_string(char *cpy_line , char *line);

/* Validates the given string as a string parameter.
 * Returns 1 on success, 0 otherwise.
 */
int validate_string(char* line);

/* Makes a copy of the given line. */
void make_cpy_line(char* line, char *cpy_line);

/* Checks if the commas in the given line alternate with strings.
 * Returns 1 on success, 0 otherwise.
 */
int alternatly_string_comma(char *line);

/* Checks if the given token represents a number.
 * Returns the number of characters consumed by the number.
 */
int is_number(char *token, int index);

/* Validates the given string as a data line.
 * Returns 1 on success, 0 otherwise.
 */
int validate_data(char* line);

/* Validates the given string as a command line.
 * Returns 1 on success, 0 otherwise.
 */
int validate_command(char *line);

/* Validates the given string as a label line.
 * Returns 1 on success, 0 otherwise.
 */
int validate_label(char *line);

/* Checks if the given string is a valid assembly line.
 * Returns 1 on success, 0 otherwise.
 */
int is_valid_line(char* line);

/* Returns a copy of the given line with the first two tokens excluded. */
char* exclude_first_two_tokens( char* s);

/* Returns a copy of the given line with the first token excluded. */
char* exclude_first_token(char* s);

/* Returns a copy of the given line with the first two tokens and commas excluded. */
char* exclude_first_two_tokens_and_commas(char* s);

/* Returns a copy of the given line with the first token and commas excluded. */
char* exclude_first_token_and_commas(char* s);

/* Checks if the given token represents an entry. */
int is_entry(char* token);

/* Checks if the given token represents an extern. */
int is_extern(char* token);

/* Validates the given string as a command line.
 * Returns 1 on success, 0 otherwise.
 */
int validate_command(char* line);

/* Checks if the given string is a register name. */
int is_reg( char *str);

/* Checks if the given string is a number. */
int is_num(char *token);

/* Checks if the given string is an immediate value. */
int is_immediate(char *str);

/* Checks if the given string is not a reserved word. */
int not_reserved(char* str);

/* Checks if the given string contains only valid characters. */
int no_trash(char *str);

/* Checks if the given string is a parameter. */
int is_param( char* str);

/* Checks if the given command line has a label and two parameters.
 * Returns 1 on success, 0 otherwise.
 */
int check_label_and_two_params(char* line);

void clear_labels_list();
void clear_entry_extern_list();
#endif
