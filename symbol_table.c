#include "symbol_table.h"




/*The function get pointer to pointer to symbol table and name, value and attribute of symbol.
  If the table is empty the function fill the new symbol's detailes in the head of the table.
  If not, the function go through the table and if there is no symbol with the same name it fill the new symbol's detailes in the end of the table. 
  The function return 1 if there is symbol with the same name, execpt when the labels's attributes are both "external". */ 
int add_new_symbol (symbol **symbol_table, char *name, int value, char *attribute)
{
   symbol *p = *symbol_table; 

   if (p == NULL) /*Check if table is empty*/
      *symbol_table = create_new_symbol(name, value, attribute);
   else
   {
      if (symbol_table_search(*symbol_table, name) != NULL)/* Check for symbol with the same name.*/
      { 
         if (strcmp(attribute, "external") || strcmp((get_symbol_attribute(*symbol_table, name)), "external"))        
            return ERROR;
      }
      else
      {
         while (p->next != NULL)
            p = p->next;
         p->next = create_new_symbol(name, value, attribute);
      }
   }
   return (!ERROR);
}



/*The function get name, value and attribute, allocate space for symbol struct and fill it with the arguments.
  The function return pointer to the new symbol. */
symbol *create_new_symbol(char *name, int value, char *attribute)
{
   symbol *new_symbol;
   new_symbol = (symbol *) malloc(sizeof(symbol));
   if (new_symbol == NULL)
   {
      fprintf(stderr,"\nFailed to allocate space.\nThe program will stop running.\n");
      exit(0);
   }
   else
   {
      strcpy(new_symbol->name, name);
      new_symbol->value = value;
      strcpy(new_symbol->attribute, attribute);
      new_symbol->next = NULL;
   }
   return new_symbol;
}



/*The founction get pointer to symbol table.
  The founction go through the table and delete every line by free its previously allocated spacep.*/
void delete_symbol_table (symbol *symbol_table)
{
   if (symbol_table != NULL)
   {
      delete_symbol_table(symbol_table->next);
      free(symbol_table);
   }
}



/*The founction get pointer to symbol table and symbol name in string.
  The founction search in the table the symbol with the same name.
  If found the founction return pointer to the symbol's line in the table and if not found it return NULL. */
symbol *symbol_table_search(symbol *symbol_table, char *name)
{
   if (!strcmp(symbol_table->name, name))
      return symbol_table;
   else
   {
      if (symbol_table->next != NULL)
         return symbol_table_search(symbol_table->next, name);
      else
         return NULL;
   }
}



/*The founction get pointer to symbol table and integer 'num'.
  The founction go through the table and for every symbol with the attribute "data" the function increase its value by 'num'.*/
void update_value(symbol *symbol_table, int num)
{
   while (symbol_table != NULL)
   {
      if (!strcmp(symbol_table->attribute, "data"))
         symbol_table->value += num;
      symbol_table = symbol_table->next;
   } 
}



/*The founction get pointer to symbol table, symbol name in string and attribute in string.
  The founction searc in the table the symbol with the same name and add 'new_attribute' to its attribute, unless it allready have this attribute.
  The founction return 1 if the symbol is not in the table.*/
int add_symbol_attribute (symbol *symbol_table, char *symbol_name, char *new_attribute)
{
   char *symbol_attribute;
   symbol_attribute = get_symbol_attribute ( symbol_table, symbol_name);
   if (symbol_attribute == NULL)
      return ERROR; 
   if (strstr(symbol_attribute, new_attribute) == NULL) 
   {
      strcat (symbol_attribute, ", ");
      strcat (symbol_attribute, new_attribute);
   }
   return (!ERROR);
}



/*The founction get pointer to symbol table and symbol name in string.
  The founction search in the table the symbol with the same name and return pointer to its attribute.
  If the symbol is not in the table it return NULL*/
char *get_symbol_attribute (symbol *symbol_table, char *symbol_name)
{
   symbol *p_symbol = symbol_table_search(symbol_table, symbol_name);
   if (p_symbol != NULL)
      return p_symbol->attribute;
   else 
      return NULL;
}




/*The founction get pointer to symbol table and symbol name in string.
  The founction search in the table the symbol with the same name and return its value.
  If the symbol is not in the table it return -1*/
int get_symbol_value (symbol *symbol_table, char *symbol_name)
{
   symbol *p_symbol = symbol_table_search(symbol_table, symbol_name);
   if (p_symbol != NULL)
      return p_symbol->value;
   else 
      return NOT_FOUND;
}









