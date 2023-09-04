#ifndef SECOND_PASS_H_
#define SECOND_PASS_H_

/* Updates the symbol table with the final values. */
void update_symbol_table();

/* Encodes the final labels into the code and data images. */
void encode_final_labels();

/* Encodes the entry list into the code and data images. */
void encode_entry_list();

/* Executes the second pass of the assembler. */
void execute_second_pass();

#endif /* SECOND_PASS_H_ */
