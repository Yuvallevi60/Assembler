#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

#define ZERO 0
#define MEMORY_START 100 /* The address where the abliable-for-use memory start*/
#define MAX_LINE_LEN 82 
#define MAX_LABEL_LEN 31 
#define ATTRIBUTES_MAX_LEN 10
#define BITS 8
#define ERROR 1
#define NOT_FOUND -1
#define IMMED_PLACE 16
#define IMMED_LEN 16
#define ADDRESS_PLACE 7
#define ADDRESS_LEN 25


enum flag {off, on};
enum command_table_columns {name, type, funct, opcode};
enum command_type { R, I, J};




/*prototypes to functions from the file "seconed_scan.c"*/

int second_scan (FILE *rfp, char *file_name, symbol **symbol_table_head, memory **command_table_head); /*do the seconed scan of the assembler on the file. check erros that could not be found on first scan and complete the memory table. create "extern" file.*/

int add_entry_attribute (symbol *symbol_table_head,char *label_name, char *file_name, int line_number); /* add "entery" to the attribute of the label 'label_name' from the symbol tabel. return 1 if the label is not in the table or have attribute 'external'.*/

int add_label_address (int command_address, char *command_name, char *command_oprands, memory *command_table, symbol *symbol_table, FILE *fp, char *file_name , int line_number); /* update commands line that could not be update and create "extern" file */




/*prototypes to functions from other files*/

void remove_file_extension (char *old_name, char *new_name); /*copy the string 'old_name' to new_name' and then delete the file name's suffix from 'new_name' */

int empty_or_comment (char *line); /*check if line is empty or comment line*/

char *get_word(char *line, char *string); /*insert the first word in 'line' to 'word'. return pointr to the char after the first word end. */

int get_symbol_value (symbol *symbol_table, char *symbol_name); /* search in the table the symbol 'symbol_name' and return its value. If not found return -1*/

char *get_symbol_attribute(symbol *symbol_table,char *symbol_name); /* search in the table the symbol 'symbol_name' and return its attribute. If not found return NULL*/

int add_symbol_attribute (symbol *symbol_table, char *symbol_name, char *new_attribute); /* search in the table the symbol 'symbol_name' and add the string 'new_attribute' to its attribute. The founction return 1 if the symbol is not in the table.*/

int search_in_command_table (char *command_name, int column); /* serach for 'command_name' in the command table and, if found, return the value of the 'column' colimn in the found line. if not found, return the numbers of commands. */

void update_machine_code (memory *memory_table, int address, char *binary_code, int code_place, int code_length); /*update the machine code of the memory line in 'address' in memory_table acording to the value of 'binary_code' 'code_place' and 'cose_length' */ 

void create_binary_code (char *binary_code, int code_length, int num); /* write in 'binary_code' array the value of 'num' in banry base, using 'code_length' digits and the two's complement method.*/





