#ifndef PRE_PROCESSOR_H_
#define PRE_PROCESSOR_H_

#include "global_helpers.h"

typedef enum {normal,read_macro} state;

typedef struct macro
{
    char name[100];
    char* macro_line;
    struct macro* next;
} macro ;

/* Analyzes the given line and performs preprocessing if necessary.
 * Updates the current state and writes to the output file if needed.
 */
void analyze_line(char* line, state *current_state, FILE* fpn);

/* Inserts a new link to the macro list with the given name. */
void insert_link(const char* name);

/* Returns a pointer to the macro link with the given name. */
macro* get_link(const char* name);

/* Adds the current macro line to the last link in the macro list. */
void macro_line_to_last_link(char* current_macro_line);

/* Frees the memory allocated for the macro list. */
void free_macro_list();

/* Analyzes the given file and performs preprocessing if necessary.
 * Writes to the output file if needed.
 */
void analyze_file(FILE* fp, FILE* fpn);

/* Executes the preprocessor for the given input and output files. */
void execute_pre_processor(FILE* fp, FILE* fpn);

#endif /* PRE_PROCESSOR_H_ */
