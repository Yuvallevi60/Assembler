#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"

#define ERROR 1
#define NOT_FOUND -1




/*prototypes to function from the file "symbol_table.c"*/

int add_new_symbol (symbol **symbol_table, char *name, int value, char *attribute); /*create new symbol line, fill it with the given argument and add it to the table*/

symbol *create_new_symbol(char *name, int value, char *attribute); /* create new symbol line, fill it with the given argument and return pointer to the symbol*/

void delete_symbol_table (symbol *symbol_table); /*delete the symbol table that the pointer point to*/

symbol *symbol_table_search(symbol *symbol_table, char *name); /*search in the table the symbol 'name'. If found return pointer to the symbol and if not found it return NULL.*/

void update_value(symbol *symbol_table, int num); /* increase by 'num' the value of every symbol int the tabel with the attribute "data"*/

int add_symbol_attribute (symbol *symbol_table, char *symbol_name, char *new_attribute); /* search in the table the symbol 'symbol_name' and add the string 'new_attribute' to its attribute. The founction return 1 if the symbol is not in the table.*/

char *get_symbol_attribute(symbol *symbol_table,char *symbol_name); /* search in the table the symbol 'symbol_name' and return its attribute. If not found return NULL*/

int get_symbol_value (symbol *symbol_table, char *symbol_name); /* search in the table the symbol 'symbol_name' and return its value. If not found return -1*/

