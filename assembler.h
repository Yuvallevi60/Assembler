#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

#define MEMORY_START 100 /* The address where the abliable-for-use memory start*/
#define ZERO 0



/*prototypes to functions from the file "assembler.c"*/

void set_arguments(int *ICF, int *DCF, symbol **symbol_table_head, memory **data_table_head, memory **command_table_head); /*asigne start values to the arguments*/

void create_files (char *file_name, symbol *symbol_table, memory *data_table_head, memory *command_table, int ICF, int DCF);/*use the argumens it got and create "entry" and "object" files*/

void delete_tables (symbol *symbol_table_head, memory *data_table_head, memory *command_table_head); /*delete the tables that the pointers point to*/





/*prototypes to functions from other files*/

int first_scan (FILE *fp, char *file_name, int *IC, int *DC, symbol **symbol_table_head, memory **data_table_head, memory **command_table_head); /*do first scan of the assembler on the file. check for syntactic errors, create symbol and data table and part of the memory table. return 0 if no erros discovered in the file.*/

int second_scan (FILE *rfp, char *file_name, symbol **symbol_table_head, memory **command_table_head); /*do the seconed scan of the assembler on the file. check erros that could be found on first scan and complete the memory table.  */

FILE *open_next_file ( char *argv_name); /* check if the 'argv_name_ is valid name for assembler file and, if it is, open the file with that name. */

void create_entries_file ( char *file_name, symbol *symbol_table); /*use the argumens it got and create "entry" file*/

void create_object_file ( char *file_name, memory *data_table, memory *command_table, int ICF, int DCF); /*use the argumens it got and create "object" file*/

void delete_symbol_table (symbol *symbol_table); /*delete the symbol table that the pointer point to*/

void delete_memory_table (memory *memory_table); /*delete the memory table that the pointer point to*/



