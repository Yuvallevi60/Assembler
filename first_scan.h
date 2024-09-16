#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

#define ZERO 0
#define ONE 1
#define MAX_LINE_LEN 82 
#define MAX_LABEL_LEN 31 
#define ATTRIBUTES_MAX_LEN 10
#define ERROR 1
#define BITS 8
#define OPCODE_PLACE 0
#define OPCODE_LEN 6
#define RS_PLACE 6
#define RS_LEN 5
#define RT_PLACE 11
#define RT_LEN 5
#define RD_PLACE 16
#define RD_LEN 5
#define FUNCT_PLACE 21
#define FUNCT_LEN 5
#define NOT_USED_PLACE 26
#define NOT_USED_LEN 5
#define IMMED_PLACE 16
#define IMMED_LEN 16
#define REG_PLACE 6
#define REG_LEN 1
#define ADDRESS_PLACE 7
#define ADDRESS_LEN 25

enum command_table_columns {name, type, funct, opcode};
enum command_type { R, I, J};
enum flag {off, on};




/*prototypes to functions from the file "first_scan.c"*/

int first_scan (FILE *fp, char* file_name, int *IC, int *DC, symbol **symbol_table_head, memory **data_table_head, memory **command_table_head); /*do first scan of the assembler on the file. check for syntactic errors, create symbol and data table and part of the memory table. return 0 if no erros discovered in the file.*/

void add_data_to_table(int *DC, memory **memory_table, char *data_type, char *data); /* store memory code of the data from 'data' in the memory table */

void add_chars_to_table (int *DC, memory **memory_table, char *data); /* store the chars of the sring in 'data' in the memory table */

void add_numbers_to_table (int *DC, memory **memory_table, int data_size, char *data);  /* store value of the numbers in 'data' in the memory table */

void add_command_to_table(int *IC ,memory **command_table, char *command_name, char *operands);  /* store memory code of command 'command_name' and its operand from 'operands' in the memory table */

void add_R_command(char *command_name, char *operands, char *binary_code); /* store memory code of R-type command 'command_name' and its operand from 'operands' in the memory table */

void add_I_command(char *command_name, char *operands, char *binary_code); /* store memory code of I-type command 'command_name' and its operand from 'operands' in the memory table */

void add_J_command(char *command_name, char *operands, char *binary_code); /* store memory code of J-type command 'command_name' and its operand from 'operands' in the memory table */




/*prototypes to functions from other files*/

int check_line_len (FILE *fp, char *line, int line_number, char *file_name); /*check if the line is longer then MAX_LINE_LEN*/

int empty_or_comment (char *line);  /*check if line is empty or comment line*/

int examine_the_line (FILE *fp, char *line, int line_number, char *file_name);  /*check if the line have syntactic errors acording to the assembly language*/

char *get_word(char *line, char *string); /*insert the first word in 'line' to 'word'. return pointr to the char after the first word end. */

char *skip_whitespace(char *pc); /* The function get pointr to string and return pointr to the first char in the string that isnt tab or space*/

int get_data_size(char *data_type);  /* The function get string data_type and return the size this type reprsent. */

int search_in_command_table (char *command_name, int column);  /* serach for 'command_name' in the command table and, if found, return the value of the 'column' colimn in the found line. if not found, return the numbers of commands. */

int get_reg_num (char *reg_name);  /* the function get string with name of register and return the number of the register*/

void create_binary_code (char *binary_code, int code_length, int num);  /* write in 'binary_code' array the value of 'num' in banry base, using 'code_length' digits and the two's complement method.*/

void add_memory_line (memory **memory_table, int size, int address, char *binary_code);  /* add to memory_table new memory struct, filled with the arguments the function got*/

int add_new_symbol (symbol **symbol_table, char *name, int value, char *attribute);  /*create new symbol line, fill it with the given argument and add it to the table*/

void update_address(memory *memory_table, int num); /* goes through every line in the table and add the value of 'num' to its addres.*/

void update_value(symbol *symbol_table, int num); /* increase by 'num' the value of every symbol int the tabel with the attribute "data"*/


