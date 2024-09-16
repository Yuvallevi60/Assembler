#include "file_functions.h"



/*The function get string, check if the string is valid name for assembler file and open the file with that name. */
FILE *open_next_file (char *argv_name)
{
   FILE *fp = NULL;
   if (!check_file_name(argv_name))
   {
      if ((fp = fopen(argv_name,"r")) == NULL) 
        printf("ERROR: The file \"%s\" cannot be opend.\n", argv_name);   
   }
   return fp;
}





/*The function get string 
  The function if the string is valid file's for assambler.
  The function return 1 if not.*/
int check_file_name (char *filename)
{
   char *p;

   if (strchr(filename,'/'))
   {
      printf("ERROR: \"%s\" is not a vaild file name - the char '/' is not allowed in file name.\n", filename);
      return ERROR;
   }

   p = strrchr(filename,'.');
   if ((p == NULL) || (strcmp(p,".as")))
   {
      printf("ERROR: \"%s\" is not a vaild file name - file name must end with \".as\".\n", filename);
     return ERROR;
   }

   return (!ERROR);
}




/*The function get name of file and pointer to symbol-table's head.
  If the table is not empty the function create new file with the name it got and suffix "ent".
  In the file the function print every label from the symbol-table that have "entry" attribute, and the lable address in hexadecimal base. */
void create_entries_file (char *file_name, symbol *symbol_table)
{
   FILE *fp;

   remove_file_extension (file_name, file_name); /* In case the name file include another suffix. */
   strcat (file_name, ".ent");
   fp = fopen (file_name , "w");

   while (symbol_table != NULL) /*Go through every label in the table till the end and check the attribute.*/
   {
      if (strstr(symbol_table->attribute, "entry") != NULL)
         fprintf(fp,"%s %04d\n",symbol_table->name,symbol_table->value);
      symbol_table = symbol_table->next;
   }

   if (!ftell(fp))   /*If nothing printed to the file, fp will keep pointing to the start of the file and ftell(fp) will rutern 0*/
      remove (file_name);
   fclose(fp);   
}




/*The function get name of file the numbers ICF and DCF and pointers to data-table's head and command-table's head. 
  If the tables are not empty the function create file with the name it got and suffix "ob".
  The file include the macine code in hexadecimal base of the command and data table (in that order).
  In the first line wrote the number of bytes used for commands and the number of bytes used for data.
  The other lines cointing 4 two-digit hexa-numbers, each represnt byte of memory, and at the start of the line written the address of the first byte.*/
void create_object_file (char *file_name, memory *data_table, memory *command_table, int ICF, int DCF)
{
   FILE *fp;
   int  address = MEMORY_START;

   remove_file_extension (file_name, file_name); /* In case the name file include another suffix. */
   strcat (file_name, ".ob");
   fp = fopen (file_name , "w");

   fprintf (fp,"     %d %d", ICF-MEMORY_START, DCF);

   address = print_hex_memory (fp, command_table, address); /*Print the command memory*/
   address = print_hex_memory (fp, data_table, address); /*Print the data memory*/

   if (!ftell(fp)) /*If nothing printed to the file, fp will keep pointing to the start of the file and ftell(fp) will rutern 0*/
      remove (file_name);
   fclose(fp);  
}




/*The function get pointer to file, pointer to  memory table and address.
  It go through the table's line and prints its machine code in two-digits hexa-numbers, each represnt byte of memory. After every byte it update the address.
  before every print the function check if the adress is product of 4, if so it print new line start with the updated address. 
  The function return the updated address after all the code in the table was printed*/
int print_hex_memory (FILE *fp, memory *memory_table, int address)
{
   int i;

   while (memory_table != NULL) /*Go through every line in the table till the end.*/
   { /*The code is store in array and every cell is char represent the ascii value of the code. starting from the most significant byte of the code*/
      for (i = (memory_table->size)-1 ; i >= 0 ; i--) /*Going down in index will print the least significant bytes first */ 
      {    
         if (!(address%4)) /*Every 4 byte print new line and the address*/
            fprintf(fp,"\n%04d",address); 
         fprintf(fp  ," %02X", (memory_table->machine_code)[i]);
         address++;
      }
      memory_table = memory_table->next;     
   }
   return address;
}









