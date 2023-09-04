#ifndef FIRST_PASS_H_
#define FIRST_PASS_H_

/* Executes the first pass of the assembler on the given input file.
 * Returns 1 on success, 0 otherwise.
 */
int execute_first_pass(FILE* fpn);

/* Encodes the label in the given line according to the given command type. */
void encode_label(char* label,cmd_tpye type);

/* Encodes the data in the given data line. */
void encode_data(char*);

/* Encodes the entry in the given entry line. */
void encode_ent(char*);

/* Encodes the code in the given line. */
void encode_code(char *line);

/* Encodes the entire line (data, entry, or code). */
void encode_line(char *line);

/* Sets the second word of the given code image node according to the label and IC. */
void set_second_words_label_ic(code_image_node *current_node);

/* Encodes a special register in the given code image node. */
void special_register_encoding(code_image_node *current_node);

/* Checks if the given code image node is a jump command with a parameter.
 * Returns 1 if true, 0 otherwise.
 */
int is_jump_with_par(code_image_node *current_node);

typedef enum  {REG_PAR_SOURCE_STATE=1,REG_PAR_DEST_STATE=2} DEST_OR_SOURCE_REG_FLAG;

/* Encodes the other word (not register) of the given parameter in the given state (source or destination).
 * Returns the encoded word.
 */
other_word encode_other_word(char* par,DEST_OR_SOURCE_REG_FLAG par_state);

table_node *extern_list_head;
table_node *entry_list_head;
table_node *symbol_list_head;
code_image_node* code_image_head;
data_image_node* data_image_head;
int IC,DC;

#endif /* FIRST_PASS_H_ */
