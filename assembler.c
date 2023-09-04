#include "code_image.h"
#include "data_image.h"
#include "table.h"
#include "first_pass.h"
#include "second_pass.h"
#include "files.h"
#include "pre_processor.h"
#include "is_valid_line.h"

/*Function deceleration*/
int file_encoding(char* file_name);

/*
The main function receives a list of file names as command-line arguments
and processes them one by one, calling the function file_encoding to
compile each file.If compilation is successful,it prints a success message,
otherwise it prints an error message.
*/
int main(int argc, char *argv[])
{
	int i;
	if(argc<2)
	{
		printf("Missing file name in command line\n");
		return EXIT_FAILURE;
	}
	for(i=1;i<argc;i++)
	{
		if(file_encoding(argv[i]))
		{
			printf("------------------------------------------------\n");
			printf("%s.as: Compilation failed\n",argv[i]);
			printf("------------------------------------------------\n");
		}
	}
	return EXIT_SUCCESS;
}
/*This function file_encoding takes a string parameter file_name and encodes an assembly
 *file into a binary executable file. It performs several steps, including pre processing the code,
 *validating the syntax, generating the binary code, and creating the output file.
 *It returns a success or failure value.*/
int file_encoding(char* file_name)
{
    /* Define variables */
    char post_macro_file_name[MAX_FILE_NAME_SIZE];
    char original_file_name_with_exten[MAX_FILE_NAME_SIZE];
    FILE *original_file_ptr=NULL,*post_macro_file_ptr=NULL;

    /* Create file names */
    strcpy(original_file_name_with_exten, file_name);
    strcat(original_file_name_with_exten, ".as");
    strcpy(post_macro_file_name, file_name);
    strcat(post_macro_file_name, ".am");

    /* Open original file */
    original_file_ptr = fopen(original_file_name_with_exten,"r");
    if (original_file_ptr == NULL) {
        printf("Failed to open file: %s\n", file_name);
        return EXIT_FAILURE;
    }

    /* Open post macro file */
    post_macro_file_ptr = fopen(post_macro_file_name,"w+");

    /* Execute pre-processor */
    execute_pre_processor(original_file_ptr, post_macro_file_ptr);

    /* Validate post macro file */
    if (validating_file(post_macro_file_ptr)!=0){
        /* Cleanup and return failure */
        clear_labels_list();
        clear_entry_extern_list();
        free_macro_list();
        fclose(post_macro_file_ptr);
        fclose(original_file_ptr);
        return EXIT_FAILURE;
    }

    /* Execute first and second passes */
    execute_first_pass(post_macro_file_ptr);
    execute_second_pass();

    /* Create assembler files */
    create_assembler_files(file_name);

    /* Cleanup and return success */
    free_table(extern_list_head);
    free_table(entry_list_head);
    free_table(symbol_list_head);
    free_code_image_list(code_image_head);
    free_data_image_list(data_image_head);
    fclose(post_macro_file_ptr);
    fclose(original_file_ptr);
    clear_labels_list();
	clear_entry_extern_list();
    free_macro_list();
    return EXIT_SUCCESS;
}
