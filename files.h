#ifndef FILES_H_
#define FILES_H_

#include "code_image.h"
#include "data_image.h"
#include "table.h"
#include "first_pass.h"
#include "second_pass.h"

/* Function Prototypes */

/*
 * Creates the output files for the compiled assembly program.
 *
 * This function creates the object file, the entry file, and the external file for the compiled assembly program.
 *
 * gets file_name The name of the input file.
 */
void create_assembler_files(char* file_name);

/*
 * Adds line numbers to the output files.
 *
 * This function adds line numbers to the output files by counting the number of newlines in the input file and writing the
 * line number before each line in the output files.
 *
 * gets fp A pointer to the file to which line numbers will be added.
 */
void add_line_numbers(FILE *fp);

/*
 * Prints the bits of an other_word struct to file.
 *
 * This function prints the bits of an other_word struct to file in binary format.
 *
 * gets w The other_word struct to be printed.
 * gets fp A pointer to the file to which the other_word struct will be printed.
 */
void print_other_word_bits(other_word w, FILE* fp);

/*
 * Prints a code_image_node struct to file in binary format.
 *
 * This function prints a code_image_node struct to file in binary format, including the bits of its first_word struct and
 * up to three other_word structs.
 *
 * gets cin The code_image_node struct to be printed.
 * gets fp A pointer to the file to which the code_image_node struct will be printed.
 */
void printBinary_code_image_node(code_image_node cin, FILE* fp);

/*
 * Prints the linked list of code_image_node structs to file in binary format.
 *
 * This function prints the linked list of code_image_node structs to file in binary format, including the bits of each
 * first_word struct and up to three other_word structs.
 *
 * gets fp A pointer to the file to which the linked list will be printed.
 */
void print_code_image_list_to_file(FILE* fp);

/*
 * Prints the linked list of data_image_node structs to file in binary format.
 *
 * This function prints the linked list of data_image_node structs to file in binary format, including the 14-bit data
 * value of each node.
 *
 * gets fp A pointer to the file to which the linked list will be printed.
 */
void print_data_image_list_to_file(FILE* fp);

/*
 * Prints a uint to file in binary format.
 *
 * This function prints a uint to file in binary format.
 *
 * gets word The uint to be printed.
 * gets fp A pointer to the file to which the uint will be printed.
 */
void print_binary_uint(unsigned int word, FILE* fp);

/*
 * Prints the symbol and entry tables to file.
 *
 * This function prints the symbol and entry tables to file, including their labels, addresses, and attributes.
 *
 * gets table_head A pointer to the head of the symbol or entry table.
 * gets fp A pointer to the file to which the table will be printed.
 */
void print_table_to_file(table_node* table_head, FILE* fp);

#endif /* FILES_H_ */
