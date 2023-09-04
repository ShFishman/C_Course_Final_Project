#ifndef CODE_IMAGE_H_
#define CODE_IMAGE_H_

#include <stdlib.h>
#include <string.h>
#include "global_helpers.h"

/**
Struct that represents the first word of a compiled instruction.
The struct contains bit fields that represent the opcode, source addressing mode,
target addressing mode, parameter 1,parameter 2, and ARE values of the instruction.
*/

typedef struct
{
unsigned int ARE:2;
unsigned int target_adressing:2;
unsigned int source_adressing:2;
unsigned int opcode:4;
unsigned int par2:2;
unsigned int par1:2;
} first_word;

/**
Struct that represents the second or third word of a compiled instruction.
The struct contains a label, address, ARE value, and data union that
 may contain either a 12-bit value or source and destination
register values, depending on the addressing mode of the instruction.
*/
typedef struct
{
	char label[MAX_LABEL_SIZE];
	unsigned int adress;
    unsigned int ARE:2;
    union
	{
        struct
		{
          unsigned int val:12;
        } normal_case;
        struct
		{
            unsigned int dest_reg:6;
            unsigned int source_reg:6;
        } split_case;
    } data;
} other_word;

/**
Struct that represents a node in the linked list that holds a first_word struct and up to three other_word structs.
The struct contains a first_word struct and an array of up to three other_word structs that represent the second and third words
of a compiled instruction. The par_counter field keeps track of how many other_word structs are currently in use.
*/

typedef struct code_image_node {
	first_word a;
	other_word b[3];
	int par_counter;
	struct code_image_node* next;
} code_image_node;

/**
Creates a new code image node and returns a pointer to the newly created node.
gets the head The head of the linked list to which the new node will be added.
return A pointer to the newly created node.
*/
code_image_node* create_new_code_image_node(code_image_node* head);

/**
Frees a linked list of code image nodes.
tets the head The head of the linked list to be freed.
*/
void free_code_image_list(code_image_node *head);

#endif /* CODE_IMAGE_H_ */
