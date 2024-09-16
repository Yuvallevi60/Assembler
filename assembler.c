#include "assembler.h"



/* The function gets file names from the argument list.
   The function scan twice every file with the suffix ".as".
   On the first scan the function check if every line in the file is vaild line in assambly language.
   As long as it not find erros it save the detailes from every line in tables.
   After the first scan, and if there were no error, the function scan the file for the secaned time.  
   on the seconed scan, it search for another errors and detailes that cannot discovers in the first scan and create "extern" file.
   After the seconed scan, and if there were no error, the function create "object" and "entery" files. empty files will be deleted.
   All the files are based on the detalies stored in the tables. 
   If at any time the function find errors it stop the scanning and delete files that was created. */
int main(int argc, char **argv)
{
   int i = 0, ICF, DCF;
   char *file_name;
   FILE *fp;
   symbol *symbol_table_head;
   memory *data_table_head, *command_table_head;

   while (++i < argc)
   {
      if ((fp = open_next_file (argv[i])) == NULL) /* open_next_file return NULL if the 'argv[i]' is not vaild file name. */
         continue;

      set_arguments( &ICF, &DCF, &symbol_table_head, &data_table_head, &command_table_head);

      file_name = (char *) malloc (strlen(argv[i])+2);
      if (file_name == NULL)
      {
         fprintf(stderr,"\nFailed to allocate space.\nThe program will stop running.\n");
         exit(0);
      }
      strcpy (file_name, argv[i]); 
       
      if (!first_scan(fp, argv[i], &ICF, &DCF, &symbol_table_head, &data_table_head, &command_table_head))
      {
         fp = freopen (argv[i], "r", fp);    
         if (!second_scan(fp, file_name, &symbol_table_head, &command_table_head))     
            create_files (file_name, symbol_table_head, data_table_head, command_table_head, ICF, DCF);

      }

      delete_tables (symbol_table_head, data_table_head, command_table_head);
      fclose(fp);
      free (file_name);     
   }

return 0;
}



/* The function gets pointers to pointers to symbol table, data table and command table, and pointers to command count and data count.
   The function set the table's pointers to NULL, the data  count to 0 and the command count to MEMORY_START. */
void set_arguments(int *ICF, int *DCF, symbol **symbol_table_head, memory **data_table_head, memory **command_table_head)
{
   *ICF = MEMORY_START;
   *DCF = ZERO;
   *symbol_table_head = NULL;
   *data_table_head = NULL;
   *command_table_head = NULL;
}



/* The function gets pointers to symbol table, data table and command table, file name, command count and data count and call to function to create entries and object files from the tables and counters. */
void create_files (char *file_name, symbol *symbol_table, memory *data_table, memory *command_table, int ICF, int DCF)
{ 
   create_entries_file (file_name, symbol_table);
   create_object_file  (file_name, data_table, command_table, ICF, DCF);
}



/* The function gets pointers to symbol table, data table and command table and call to delete-functions to free their previously allocated space.*/
void delete_tables (symbol *symbol_table_head, memory *data_table_head, memory *command_table_head)
{
   delete_symbol_table (symbol_table_head);
   delete_memory_table (data_table_head);
   delete_memory_table (command_table_head);     
}

















