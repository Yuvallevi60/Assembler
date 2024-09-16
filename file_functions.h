#include <stdio.h>
#include <string.h>
#include "structs.h" 

#define ERROR 1
#define MEMORY_START 100 /* The address where the abliable-for-use memory start*/




/*prototypes to functions from the file "file_functions.c"*/

FILE *open_next_file (char *argv_name); /* check if the 'argv_name' is valid name for assembler file and, if it is, open the file with that name. */

int check_file_name (char *filename); /* check if the 'file_name' is valid name for assembler file. */

void create_entries_file ( char *file_name, symbol *symbol_table); /*use the argumens it got and create "entry" file*/

void create_object_file ( char *file_name, memory *data_table, memory *command_table, int ICF, int DCF); /*use the argumens it got and create "object" file*/

int print_hex_memory (FILE *fp, memory *memory_table, int address); /*print the memory code in the memory_table in hex base*/




/*prototypes to functions from other files*/

void remove_file_extension (char *old_name, char *new_name); /*insert to 'new_name' the string 'old_name' without suffix */



