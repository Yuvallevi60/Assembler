#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"

#define ZERO 0
#define NOT_FOUND -1
#define BITS 8

enum flag {off, on};




/*prototypes to function from the file "memory_table.c"*/

void add_memory_line (memory **memory_table, int size, int address, char *binary_code); /* add to memory_table new memory struct, filled with the arguments the function got*/

void build_machine_code (unsigned char *machine_code, char *binary_code, int start_place, int code_length); /* build machine_code that its bits are the same as binary_code celles. 'code_length' bits/celles from the 'start_place' bit/cell.   */

void create_empty_line(memory **new_memory_line); /* alocate space for memory struct and assined its address to *new_memory_line. */ 

void fill_line(memory *new_memory_line, int address, int data_size, unsigned char *machine_code);/* fill *new_memory_line members with the argument the function got.*/

void insert_line_to_table(memory *new_line, memory ** memory_table); /*enter 'new_line' to the end of memory table or to the head if the table is empty */

void update_address(memory *memory_table, int num); /* goes through every line in the table and add the value of 'num' to its addres.*/

void update_machine_code (memory *memory_table, int address, char *binary_code, int code_place, int code_length); /*update the machine code of the memory line in 'address' in memory_table acording to the value of 'binary_code', 'code_place' and 'cose_length' */

unsigned char *get_machine_code (memory *memory_table, int address); /*search the line 'address' in the memory table and return the line's meachin
_code if found or return NULL if not found */

