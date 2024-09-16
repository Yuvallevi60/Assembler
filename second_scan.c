#include "second_scan.h"




/* The function get pointer to file, the name of this file and pointers to pointers of symbol and command tables.
   The function create ".ext" file then read every line in the file and skip the comments or empty lines and non-entry guide line. 
   For entry line, the function serach the operand label in the symbol table and add ".entry" to its attributes. if the label didnt found it print error message.
   If its a command line with label operand the function will update the machine_code of the line in the command table according to the command type and the label value.
   And if its a command line with label operand the function also check if the label's attributes is ".extern", if so the function will print error message in case the commans is I-type or, in case the commans is J-type, print the line number and label name in the ".ext" file.
   If error was found or the function delete the ".ext" and return 1, otherwise the function return 0. */
int second_scan (FILE *rfp, char *file_name, symbol **symbol_table_head, memory **command_table_head)
{
   int line_number = ZERO, address = MEMORY_START, f_error = off;
   char line[MAX_LINE_LEN], *pc ;
   char temp_string[MAX_LINE_LEN];  
   FILE *wfp; 

   remove_file_extension (file_name, file_name);
   strcat (file_name, ".ext");
   wfp = fopen (file_name , "w");

   while (fgets( line, MAX_LINE_LEN, rfp) != NULL )
   {
      line_number++;    
      if (empty_or_comment (line))
         continue;

      pc = get_word(line, temp_string);  /* temp_string is the first word of the line. pc point to the char after*/
      if (temp_string[strlen(temp_string)-1] == ':') /* the first word is label */  
         pc = get_word(pc, temp_string);  
                             /* temp_string is the command or guide word of the line. pc point to the char after*/
      if (*temp_string == '.')
      {
         if (strcmp(temp_string,".entry"))
            continue; 
         pc = get_word(pc, temp_string);   /* temp_string is the label for the guide word '.entry'*/
         if (add_entry_attribute (*symbol_table_head, temp_string, file_name, line_number))
            f_error = on;
      }
      else
      {
         if (add_label_address (address, temp_string, pc, *command_table_head, *symbol_table_head, wfp, file_name, line_number))
            f_error = on;    
         address += ws;
      }
   }
   if ((!ftell(wfp)) || (f_error)) /*If nothing printed to the file, wfp will keep pointing to the start of the file and ftell(wfp) will rutern 0*/
      remove (file_name);
   fclose(wfp);
   return f_error;
}





/*The founction get pointer to symbol table, label name in string , and the name of the file and the number of the line in the file that the command is from.
  The founction get from  table the attribute of the symbol with the same name. if the symbol not fund the function print error message and return 1.
  If the symbol's attribute is "external" the function print error message and return 1.
  Otherwise the function add "entery" to its attribute, unless it allready have this attribute, and return 0. */
int add_entry_attribute (symbol *symbol_table_head,char *label_name, char *file_name, int line_number)
{
   char *label_attribute;
   label_attribute = get_symbol_attribute (symbol_table_head, label_name);
   if (label_attribute == NULL)
   {
      printf("ERROR: In file \"%s\" line %d - The label \"%s\" isnt used in the file.\n", file_name, line_number, label_name);
      return ERROR;
   }
   if (!strcmp(label_attribute, "external"))
   {
      printf("ERROR: In file \"%s\" line %d - The label \"%s\" allready define as \"external\". Label can not be define both as \"external\" and \"entry\".\n", file_name, line_number, label_name);
      return ERROR;
   } 
   add_symbol_attribute (symbol_table_head, label_name, "entry");
   return (!ERROR);
}




/* The function gets command's address, name and operands, the name of the file and the number of the line in the file that the command is from, memory and sembol tables and pointer to "ext" file .
   the function does the following:
   The function do-nothing if the command is not branc command or non-"stop" J-type command.
   If its branc, the function take the 3 operand in 'command_oprands'. If its J-type, the function take first(and only) operand in 'command_oprands'.
   If the operand is register the function then nothing. otherwise, the operand is label.
   The function search the label in the symbol table and if not found print error message if not found and retrun 1.
   The function check if the label's attribute is "external". 
   If yes and the command is branch the function print error message and retrun 1. if yes and the command is J-type the function write the label and the comand's addres in new line in the "ext" file.
   then the function will update in the memory table the line of 'command_address' acordding to the label value and the command type and return 0. */ 
int add_label_address (int command_address, char *command_name, char *command_oprands, memory *command_table, symbol *symbol_table, FILE *fp, char *file_name , int line_number)
{
   int c_type, label_address, code_value, label_code_len, label_code_place;
   char label[MAX_LABEL_LEN], *binary_code;

   c_type = search_in_command_table (command_name, type);
   if (((c_type != J) || (*command_name == 's')) && (*command_name != 'b')) /* just branc command or non-"stop" J-type command may have label operand*/
      return (!ERROR);
   
   if (c_type == I)
   {
      command_oprands = strrchr (command_oprands, ',');
      command_oprands++; /*skip ','*/
      label_code_len = IMMED_LEN;
      label_code_place = IMMED_PLACE;
   }
   else
   {
      label_code_len = ADDRESS_LEN;
      label_code_place = ADDRESS_PLACE;
   }
   get_word(command_oprands,label);

   if (*label == '$')  /*J-type command may have register operand insted of label*/
      return (!ERROR);

   label_address = get_symbol_value (symbol_table, label);
   if (label_address == NOT_FOUND)
   {
      printf("ERROR: In file \"%s\" line %d - The label \"%s\" isnt used in the file.\n", file_name, line_number , label);
      return ERROR;
   }  
   if (!strcmp(get_symbol_attribute(symbol_table, label) ,"external"))
   {  
      if (c_type == I)
      {
         printf("ERROR: In file \"%s\" line %d - Cant use external label in branch command.\n", file_name, line_number);
         return ERROR;
      } 
      else
         fprintf(fp,"%s %04d\n",label ,command_address);
   }

   code_value = ((c_type == I) ? label_address - command_address : label_address); /* for branch command code the distance from the command address to the label asress. otherwise, code the label address*/

   binary_code = (char *) malloc(ws*BITS);
   if (binary_code == NULL)
   {
      fprintf(stderr,"\nFailed to allocate space.\nThe program will stop running.\n");
      exit(0);
   }
   create_binary_code (binary_code + label_code_place , label_code_len, code_value);
   update_machine_code (command_table, command_address, binary_code,  label_code_place,  label_code_len);
   free (binary_code);

   return (!ERROR);
}








