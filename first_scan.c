#include "first_scan.h"



/* The function get pointer to file, the name of this file, two pointers to integer which will count the bytes used for data and command and pointers to pointers of symbol,data and command table
  The function read every line in the file and check the following: if its length is less than the max limit, if it empty or comment line and check if it syntactically-valid line in assembly.
   Empty or comment lines will be skipped while for every error discovered in other lines the function will print error message.
   As long as error did not found, for every line the function will store its label (if have) in the symbol table and its details in the data or command tables if its guide word or command line respectively.
   The function return 1 if error found in the file and 0 otherwise. */
int first_scan (FILE *fp, char* file_name, int *IC, int *DC, symbol **symbol_table_head, memory **data_table_head, memory **command_table_head)
{
   enum flag f_label, f_error = off;

   int label_value, line_number = ZERO;
   char line[MAX_LINE_LEN], *pc, line_check[MAX_LINE_LEN] ;
   char temp_string[MAX_LINE_LEN];  
   char label_name[MAX_LABEL_LEN], label_atributes[ATTRIBUTES_MAX_LEN];
      
   while (fgets( line, MAX_LINE_LEN, fp) != NULL )
   {
      line_number++; 
      if (check_line_len (fp, line, line_number, file_name)) /*check if the line's length is less then MAX_LINE_LEN*/
      {
         f_error = on;
         continue;
      }
      if (empty_or_comment (line))
         continue;
      
      strcpy (line_check, line); /*copy the line because the string may changed while syntactical check*/                      
      if (examine_the_line(fp, line_check, line_number, file_name))
      {
         f_error = on;
         continue;
      }
          /* The loop will reach this part only if no errors discoverd in the line so far.*/         
      f_label = off;
      pc = get_word(line, temp_string); /* temp_string is the first word, pc is pointer to the char after the word end*/ 

      if (temp_string[strlen(temp_string)-1] == ':') /*Check if the first word is label, which must end with ':'*/
      {
         strcpy(label_name ,temp_string);
         label_name[strlen(label_name)-1] = '\0';
         f_label = on;  
         pc = get_word(pc, temp_string);  /*temp_string is the seconed word(which is guide word or command) */
      }
      if (*temp_string == '.') /*guide word allways star with '.'*/
      {
         if (!strcmp(temp_string,".entry")) /* 'entry' guide word are ignored at first scan.*/
            continue; 
         if (!strcmp(temp_string,".extern"))
         {
            f_label = on; 
            label_value = ZERO;
            strcpy(label_atributes ,"external");
            get_word(pc, temp_string); /*temp_string is the "extern" guide word's oprand, which is label*/
            strcpy(label_name ,temp_string);
         }
         else /* The guide word is "db","dh","dw" or "asci"*/
         {  
            label_value = *DC;
            strcpy(label_atributes ,"data");
            add_data_to_table(DC, data_table_head, temp_string, pc);
         }
      }
      else /*the line is command line*/
      {  
         label_value = *IC;
         strcpy(label_atributes ,"code");
         add_command_to_table(IC ,command_table_head, temp_string, pc);
      }   
      if (f_label)
      {
         if (add_new_symbol(symbol_table_head ,  label_name,  label_value, label_atributes)) /*add_new_symbol return non-zero if there is label with the same name.*/
         {
            printf("ERROR: In file \"%s\" line %d - The label \"%s\" is allready used.\n", file_name, line_number,label_name);
            f_error = on;
         }
      }
   } 
   if (!f_error) /* Adding the the IC to data symbol's value and data memory-line's address.*/
   {
      update_address(*data_table_head, *IC);
      update_value(*symbol_table_head, *IC);
   }
   return f_error;
}









/* The function get pointer to data counter, pointer to pointer to memoy table, string 'data_type' and string 'data'.
   The function call for other function(according to 'data_type') that will store the data from 'data' in the memory table*/
void add_data_to_table(int *DC, memory **memory_table, char *data_type, char *data)
{ 
   if (!strcmp(data_type,".asciz"))
      add_chars_to_table (DC, memory_table, data);
   else
      add_numbers_to_table (DC, memory_table, get_data_size(data_type), data);
}





/* The function get pointer to data counter, pointer to pointer to memoy table and string 'data'.
   The string 'data' represnt string: its first and last non-whitespace char are '"', and between them there are printable charcters
   For any of those charcters and for '\0' char at the end: 
    The function store there value in binary base in the string 'binary_code'.
    The function then create empty line in the memory table and with the 'binary_code' and cournt data count it fill the new line.
    The function update the value of the data counter after every new added line. */
void add_chars_to_table (int *DC, memory **memory_table, char *data)
{
   char *binary_code;
   int  i,j;

   binary_code = (char *) malloc(bs*BITS); /*The binary code for char is 8-bits long*/
   if (binary_code == NULL)
   {
      fprintf(stderr,"\nFailed to allocate space.\nThe program will stop running.\n");
      exit(0);
   } 
   data = skip_whitespace(data);
   data++; /*skip the first '"'*/
   for (i = strlen(data) ; *(data+i) != '\"' ; i--); /* find the last '"' */
   for (j = 0 ; j < i ; j++)   /* go from the first to the last '"' */
   {  
      create_binary_code (binary_code, bs*BITS, *(data+j)); /* the string 'binary_code' will be *(data+j) in binary base */
      add_memory_line (memory_table, bs, *DC, binary_code);
      *DC += bs;
   }
   create_binary_code (binary_code, bs*BITS, '\0'); /* adding the value of '\0' to the table */
   add_memory_line (memory_table, bs, *DC, binary_code);
   *DC += bs;   
   free(binary_code);  
}




/* The function get pointer to a data counter 'DC', pointer to pointer to memoy table, integer 'data_size' and string 'data'.
   The string 'data' containing list of integers numbers seprated by commas.
   For every number the function will store there value in binary base in the string 'binary_code'.
   The number of digit in 'binary_code' is decided by 'data_size'.
   The function then will create empty line in the memory table and with the 'binary_code' and cournt DC it will fill the new line.
   The function update the value of the data counter after every new added line. */
void add_numbers_to_table (int *DC, memory **memory_table, int data_size, char *data)
{
   char *binary_code, *number;

   binary_code = (char *) malloc(data_size*BITS); /* the code of the numbers from 'data' will be (data_size*8)-bits long. */
   if (binary_code == NULL)
   {
      fprintf(stderr,"\nFailed to allocate space.\nThe program will stop running.\n");
      exit(0);
   }
   number = strtok(data, ",");

   while (number != NULL)
   {  
      create_binary_code (binary_code, data_size*BITS, atoi(number)); /* the string 'binary_code' will be atoi(number) in binary base */
      add_memory_line (memory_table, data_size, *DC, binary_code);
      *DC += data_size;
      number = strtok(NULL, ",");
   }
   free(binary_code); 
}









/* The function get pointer to instrunction counter 'IC', pointer to pointer to memoy table, command name in string and string string of operands.
   The function call for other function(according to the type of the command) that build the binary code for the command and the operands in the string 'binary_code'.
   Then the fnction create new line in the memory table and store there the 'binary_code' and its address (which is the instruction count).
   Finaly the fnction updated the inctruction count. */
void add_command_to_table(int *IC ,memory **memory_table, char *command_name, char *operands)
{
   int c_type; 
   char *binary_code;

   binary_code = (char *) malloc(ws*BITS); /* the machine code for command line is 32-digit long.*/
   if (binary_code == NULL)
   {
      fprintf(stderr,"\nFailed to allocate space.\nThe program will stop running.\n");
      exit(0);
   }
   c_type = search_in_command_table (command_name, type);
   switch (c_type)
   {
      case R: 
      {
         add_R_command(command_name, operands, binary_code);
         break;
      }
      case I: 
      {
         add_I_command(command_name, operands, binary_code);
         break;
      }
      default: 
      {
         add_J_command(command_name, operands, binary_code);
         break;
      }
   } /* now 'binay_code' is string represnt the machine code of the line */ 

   add_memory_line (memory_table, ws, *IC, binary_code);
   *IC += ws; 
   free(binary_code);
}




/* The function get a name of R-type command, string with list of operandes , and pointer to 'binary_code' a empty space in the size of 32 chars.
   The function will fill 'binary_code' with the digit of the machine code of the command in binary base.
   The function return pointer to 'binary_code'.  */
void add_R_command(char *command_name, char *operands, char *binary_code)
{
   int reg , c_opcode, c_funct;

   c_opcode = search_in_command_table (command_name, opcode);
   c_funct = search_in_command_table (command_name, funct);

   create_binary_code (binary_code + OPCODE_PLACE , OPCODE_LEN, c_opcode); /* fill the opcode value in binary digits according to its place in the machine code*/
   create_binary_code (binary_code + FUNCT_PLACE , FUNCT_LEN, c_funct); /* fill the funct value in binary digits according to its place in the machine code */
   create_binary_code (binary_code + NOT_USED_PLACE , NOT_USED_LEN, ZERO); /* fill 0 in the unsued place in R-type command's machine code */
      
   reg = get_reg_num (strtok(operands, ","));            
   create_binary_code (binary_code + RS_PLACE ,RS_LEN, reg); /* fill the register num in binary digits according to its place in the machine code*/
    
   reg = get_reg_num (strtok(NULL, ","));
   if (!c_opcode) /* secenod operand in commands with opcode 0 is RT while for commands with opcode 1 is RD.*/   
      create_binary_code (binary_code + RT_PLACE ,RT_LEN, reg);
   else
      create_binary_code (binary_code + RD_PLACE ,RD_LEN, reg);

   if (!c_opcode)  /* commands with opcode 0 have 3 operands while commands with opcode 1 have 2 operands.*/
   {
      reg = get_reg_num (strtok(NULL, ","));
      create_binary_code (binary_code + RD_PLACE ,RD_LEN, reg);
   }
   else
      create_binary_code (binary_code + RT_PLACE ,RT_LEN, ZERO);       
}




/* The function get a name of I-type command, string with list of operandes , and pointer to 'binary_code' a empty space in the size of 32 chars.
   The function will fill 'binary_code' with the digit of the machine code of the command in binary base.
   The function return pointer to 'binary_code'.  */
void add_I_command(char *command_name, char *operands_list, char *binary_code)
{
   int  c_opcode, reg;
   char *operand;
   c_opcode = search_in_command_table (command_name, opcode);
   create_binary_code (binary_code + OPCODE_PLACE , OPCODE_LEN, c_opcode); /* fill the opcode value in binary digits according to its place in the machine code*/

   operand = strtok(operands_list, ",");  
   reg = get_reg_num (operand)  ;      
   create_binary_code (binary_code + RS_PLACE ,RS_LEN, reg); /* fill the register num in binary digits according to its place in the machine code*/
   
   operand = strtok(NULL, ",");
   if (*command_name == 'b') /* the seconed operand of branch command is register and the seconed operand of non-brunch command is intger*/
   {
      reg = get_reg_num (operand) ;  
      create_binary_code (binary_code + RT_PLACE ,RT_LEN, reg);
   }  /* the third operand of branch command is label and will not add the binary code by this function*/
   else
   {
      create_binary_code (binary_code+IMMED_PLACE ,IMMED_LEN, atoi(operand));
      operand = strtok(NULL, ",");
      reg = get_reg_num (operand);  
      create_binary_code (binary_code + RT_PLACE ,RT_LEN, reg);
   }      
}




/* The function get a name of J-type command, string with list of operandes , and pointer to 'binary_code' a empty space in the size of 32 chars.
   The function will fill 'binary_code' with the digit of the machine code of the command in binary base.
   The function return pointer to 'binary_code'.  */
void add_J_command(char *command_name, char *operands, char *binary_code)
{
   int  c_opcode, reg;

   c_opcode = search_in_command_table (command_name, opcode);
   create_binary_code (binary_code + OPCODE_PLACE , OPCODE_LEN, c_opcode); /* fill the opcode value in binary digits according to its place in the machine code*/
   operands = skip_whitespace(operands); 
   if (*operands == '$') /* the operand is register */
   {
      create_binary_code (binary_code + REG_PLACE , REG_LEN, ONE);
      reg = get_reg_num (operands); 
      create_binary_code (binary_code + ADDRESS_PLACE , ADDRESS_LEN, reg); /* fill the register number in binary digits according to its place in the machine code */
   }
   else /* the comand may have label operand or non operand at all (stop command). the function fill the code with 0-s in any case. */
   {
      create_binary_code (binary_code + REG_PLACE , REG_LEN, ZERO);
      create_binary_code (binary_code + ADDRESS_PLACE , ADDRESS_LEN, ZERO);
   }          
}











