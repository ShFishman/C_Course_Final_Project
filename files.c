#include "files.h"
int LINE_COUNTER;

/* This function creates assembler files with the given file name */
void create_assembler_files(char* file_name)
{
    char ob_file_name[MAX_FILE_NAME_SIZE];
    char ext_file_name[MAX_FILE_NAME_SIZE];
    char ent_file_name[MAX_FILE_NAME_SIZE];
    FILE *ps_ob,*ps_ent,*ps_ext;
    LINE_COUNTER = LINE_NUM_START;
    sprintf(ob_file_name, "%s.ob", file_name);
    sprintf(ext_file_name, "%s.ext", file_name);
    sprintf(ent_file_name, "%s.ent", file_name);

    ps_ob = fopen(ob_file_name, "w");
    fseek(ps_ob, 0, SEEK_SET);
    fprintf(ps_ob, "%d %d\n", IC-99, DC);
    print_code_image_list_to_file(ps_ob);
    print_data_image_list_to_file(ps_ob);
    fclose(ps_ob);

    if(extern_list_head->next != NULL)
    {
        ps_ext = fopen(ext_file_name, "w");
        fseek(ps_ext, 0, SEEK_SET);
        print_table_to_file(extern_list_head, ps_ext);
        fclose(ps_ext);
    }

    if(entry_list_head->next != NULL)
    {
        ps_ent = fopen(ent_file_name, "w");
        fseek(ps_ent, 0, SEEK_SET);
        print_table_to_file(entry_list_head, ps_ent);
        fclose(ps_ent);
    }
}

/* This function prints the code image list to the given file */
void print_code_image_list_to_file(FILE* fp)
{
    code_image_node *ptr = code_image_head->next;
    while(ptr != NULL)
    {
        printBinary_code_image_node(*ptr, fp);
        ptr = ptr->next;
    }
}

/* This function prints a binary code image node to the given file */
void printBinary_code_image_node(code_image_node cin, FILE* fp)
{
    int i;
    fprintf(fp, "%04d\t", LINE_COUNTER);
    LINE_COUNTER++;
    print_binary_uint(*((unsigned int*)&cin.a), fp);
    for(i = 0; i < cin.par_counter; i++)
    {
        fprintf(fp, "%04d\t", LINE_COUNTER);
        print_other_word_bits(cin.b[i], fp);
        LINE_COUNTER++;
    }
}

/* This function prints other word bits to the given file */
void print_other_word_bits(other_word word, FILE* fp)
{
    unsigned int bits = 0;
    bits |= word.ARE;
    bits |= (word.data.normal_case.val << 2);
    print_binary_uint(bits, fp);
}

/* This function prints the data image list to the given file */
void print_data_image_list_to_file(FILE* fp)
{
    data_image_node* ptr = data_image_head->next;
    while(ptr)
    {
        fprintf(fp, "%04d\t", LINE_COUNTER);
        LINE_COUNTER++;
        print_binary_uint(ptr->data, fp);
        ptr = ptr->next;
    }
}

/* This function prints a binary unsigned int to the given file */
void print_binary_uint(unsigned int word, FILE* fp)
{
    int i;
    for (i = 13; i >= 0; i--)
    {
        if (word & (1 << i))
            fprintf(fp, "/");
        else
            fprintf(fp, ".");
    }
    fprintf(fp, "\n");
}
/* This function prints a table to the given file */
void print_table_to_file(table_node* table_head,FILE* fp)
{
	table_node *ptr = table_head->next;
	while(ptr!=NULL)
	{
		fprintf(fp,"%s %d\n",ptr->name,ptr->address);
		ptr=ptr->next;
	}
}
