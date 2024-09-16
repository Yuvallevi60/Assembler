#include "check_line.h"




/*The function get pointer to file, pointer to line in the file, the number of the line in the file and the file name.
  The function check if the line is not larger then the max length limit.
  If the length pass the limit the function will get the rest of the line from the file and return non-zero value.
  If the length not pass the limit the function delete the '\n' from the end of the line and return zero value.*/
int check_line_len (FILE *fp, char *line, int line_number, char *file_name)
{
   if (line[strlen(line)-1] != '\n') /*if the string didnt end with '\n' it means the line in the file was longer then allowed. */
   { 
     printf("ERROR: In file \"%s\" line %d - The line length is more then %d charcters.\n", file_name, line_number, MAX_LINE_LEN-2);
     do /*get the rest of the line from the file.*/
     { 
        fgets( line, MAX_LINE_LEN, fp);
     } while (line[strlen(line)-1] != '\n');
     return ERROR;
   }
   else
   {
      line[strlen(line)-1] = '\0'; 
      return (!ERROR); 
   }
}




/*The function get string of char and return non-zero if the first non-whitspace cahr is '\0' or ';'(whice mean the line is empty or comment respectively)*/
int empty_or_comment (char *line)
{
   line = skip_whitespace(line);
   return ((*line == '\0') || (*line == ';') || (*line == '\n'));
}




/* The function get pointer to file, string conating line from the file, the line number and the file name.
   The function check if there is syntactic error in the line acording to the asmenbly language.
   If error is found the function will print appropriate message, stop checking the line and return 1.
   If all the line is without errors the function will return 0. */
int examine_the_line (FILE *fp, char *line, int line_number, char *file_name)
{
    char word[MAX_LINE_LEN]; 

    if (search_comas_errors (line, line_number, file_name))
       return ERROR;

    line = get_word(line, word); /*'word' containing the first word of the line and 'line' point to the rest of the line.*/

    if (*(word+strlen(word)-1) == ',')
    {
       printf("ERROR: In file \"%s\" line %d - Cant have ',' after first word.\n", file_name, line_number); 
       return ERROR;
    }

    if (word[strlen(word)-1] == ':') /* if the first word is end with ':' the word (if not invaild) can only be label.*/
    {
       word[strlen(word)-1] = '\0';
       if (check_label (word, line_number, file_name)) /* check if it valid label*/
          return ERROR;
       line = get_word(line, word); /*'word' containing the next word in the line and 'line' point to the rest of the line.*/
       if ((*word == '\0') || (*word == ','))
       {
          if (*word == '\0')
             printf("ERROR: In file \"%s\" line %d - Line cant contain only label, It must have command or guide word following it.\n", file_name, line_number); 
          else
             printf("ERROR: In file \"%s\" line %d - Cant have ',' before command or guide word.\n", file_name, line_number); 
          return ERROR;
       }
    }       
    if (*word == '.') /* if the word start with '.' the line (if not invaild) can only be guide line. */
       return check_guideword (word, line, line_number, file_name);        
    else  /* if the word not start with '.' the line (if not invaild) can only be command line. */
       return check_command (word, line, line_number, file_name);
}




/* The function get a string 'label' that contain word, name of the file containing the word and the number of line in the file the word is in. 
   The function check if the word can used as a valid label name in the asembly language.
   The function print error message and return 1 if the word is not valid as label.*/
int check_label (char *label, int line_number, char *file_name)
{
   int i;

   if (!isalpha(*label))
   {
      printf ("ERROR: In file \"%s\" line %d - Label must start with an alphabet char.\n", file_name, line_number);
      return ERROR;
   }
   for (i = 0 ; i < strlen(label) ; i++)
   {
      if (!isalnum(*(label+i)))
      {
         printf ("ERROR: In file \"%s\" line %d - Label must contain only numbers and alphabet chars.\n", file_name, line_number);
         return ERROR;
      }
   }
   if (strlen(label) >= MAX_LABEL_LEN)
   {
      printf ("ERROR: In file \"%s\" line %d - Label cant be longer then %d characters.\n", file_name, line_number, MAX_LABEL_LEN-1);
      return ERROR;
   }
   if (search_in_command_table(label, name) != NUM_OF_COMMANDS) /* search_in_command_table return NUM_OF_COMMANDS if command didnt found.*/ 
   {
      printf ("ERROR: In file \"%s\" line %d - Label name cant be the same as one of the commands name.\n", file_name, line_number);
      return ERROR;    
   }
   if ((!strcmp(label,"db")) || (!strcmp(label,"dh")) || (!strcmp(label,"dw")) || (!strcmp(label,"asciz")) || (!strcmp(label,"entry")) || (!strcmp(label,"extern")))
   {
      printf ("ERROR: In file \"%s\" line %d - Label name cant be the same as one of the guide words name.\n", file_name, line_number);
      return ERROR;    
   }
   return (!ERROR);
}




/* The function get two string whic are the first word in line and the rest of the line, it also get the name of the file containing the line and the line's number in the file.
   The function check if the word is guide word in asmbly.  If no, it print error message and return 1.
   If yes, it will call to another function to check the rest of the line acording to the gide word's type and return what it return. */
int check_guideword (char *guideword, char *operands_list, int line_number, char *file_name)
{ 
   if (!strcmp(guideword,".entry") || !strcmp(guideword,".extern"))
      return check_label_operand (operands_list, line_number, file_name);
   if (!strcmp(guideword,".db") || !strcmp(guideword,".dh") || !strcmp(guideword,".dw"))
      return check_numbers_operand (guideword, operands_list, line_number, file_name);
   if (!strcmp(guideword,".asciz"))
      return check_string_operand (operands_list, line_number, file_name);
   printf("ERROR: In file \"%s\" line %d - There is no guide word named \"%s\".\n", file_name, line_number, guideword);
   return ERROR;
}




/* The function get a string , the name of the file containing that string and the number of the line containing the string in the file.
   The function check if the string is valid as oprends list containg only one label acording to the asmebly language.
   If error found the function print error message, stop checking and return 1.
   If the string is a valid The function return 0. */
int check_label_operand (char *operands_list, int line_number, char *file_name)
{
   char label[MAX_LINE_LEN];

   CHECK_LIST_EMPTY_OR_START_COMMA(operands_list) /* check if the oprand list is empty or have ',' before the first operand. */

   operands_list = get_word (operands_list, label); /*'label' containing the first word in 'oprand_list' and 'operands_list' update to point the char after it.*/
   if (check_label (label, line_number, file_name))
      return ERROR;
   operands_list = skip_whitespace(operands_list);
   if (*operands_list != '\0')
   {
      if ((*operands_list == ',') && (*skip_whitespace(++operands_list) == '\0'))
         printf("ERROR: In file \"%s\" line %d - Cant have ',' after the last paramter.\n", file_name, line_number);
      else
         printf("ERROR: In file \"%s\" line %d - There must be only one parmeter.\n", file_name, line_number);
      return ERROR;
   }
   return (!ERROR);  
}




/* The function get two pointers to char, the first to a guide word in asembly (.db/.dh/.dw) and the seconed is oprands list. 
   The function also get the name of the file containing those string and the number of the line in the file those string are.
   The function check if 'oprands_list' is valid as operands list containg numbers acording to the guide word type and to the asmebly language.
   If error found the function print error message, stop checking and return 1.
   If the string is a valid The function return 0. */
int check_numbers_operand(char *guideword, char *operands_list, int line_number, char *file_name)
{
   int size = ZERO, firstoperand = on, num, isnegative;
   char *operand;   

   size = get_data_size(guideword);

   operand = strtok (operands_list, ","); /* Between every two operands must be comma so (if 'operands_list' is valid) 'operand' will point to one oprand.*/
   while (operand != NULL) /* Run until no oprand left in 'operand_list'*/ 
   {
      isnegative = off;
      num = ZERO;
      operand = skip_whitespace (operand);
      if (((*operand) == '-') || ((*operand) == '+')) /* numbers in the list can be signed.*/
      {
         isnegative = ((*operand) == '-' ? on : off);
         operand++;
      }
      if (isdigit(*operand))
      {
         do
         {
            num = (num*10) + ((*operand)- 48); /*reducing 48 from the ascii value of digit char will result in the integer value of that digit*/
         } while (isdigit(*(++operand)));
         num *=  (isnegative? -1 : 1); 

         if ( (*operand) == '.' )
         {
            printf("ERROR: In file \"%s\" line %d - The numbers given to store in data must be integers.\n", file_name, line_number);
            return ERROR;
         }
         if ( (num >= pow(2,(size*BITS-1))) || (num < (-1)*pow(2,(size*BITS-1))) )
         {
            printf("ERROR: In file \"%s\" line %d - The number %d is not in the size limit for %s sized space.\n", file_name, line_number, num, guideword);
            return ERROR;
         }

         CHECK_MISSING_COMA(operand) /* Check if there is another oprand in the string 'operand'*/
         firstoperand = off;
      }
      else /*found oprand taht is not number.*/
      {
         if (*operand == '\0') /* 'operand' is empty */
         {
            operand = strtok (NULL, ",");
            if (firstoperand)
            {
               if (operand != NULL)
                  printf("ERROR: In file \"%s\" line %d - Cant write ',' before the first parameter.\n", file_name, line_number);
               else
                  printf("ERROR: In file \"%s\" line %d - Cant write \"%s\" guide word without parameters.\n", file_name, line_number, guideword);
            }
            else
            {
               if (operand != NULL)
                  printf("ERROR: In file \"%s\" line %d - Cant write two consestive ','.\n", file_name, line_number);
               else
                  printf("ERROR: In file \"%s\" line %d - Cant end parmeters list with ','.\n", file_name, line_number);
            }
         }
         else 
            printf("ERROR: In file \"%s\" line %d - Parameter for \"%s\" guide word can be only numbers.\n", file_name, line_number, guideword);
         return ERROR;
      }
      operand = strtok (NULL, ","); 
   }
   return (!ERROR);
}




/* The function get pointer to oprands list, the name of the file containing this list and the number of the line in the file this string is.
   The function check if 'oprands_list' is valid as operands list to the '.asciz' guide word and acordding to the asmebly language.
   If error found the function print error message, stop checking and return 1.
   If the error is not found The function return 0. */
int check_string_operand( char *operands_list, int line_number, char *file_name)
{
   int i, j;

   CHECK_LIST_EMPTY_OR_START_COMMA(operands_list) /* check if the oprand list is empty or have ',' before the first operand. */

   if (*operands_list != '"')
   {
      printf("ERROR: In file \"%s\" line %d - Parameter of \"asciz\" guide word must start with '\"' char.\n", file_name, line_number);
      return ERROR;
   }
   for (i = strlen(operands_list)-1 ; *(operands_list+i) != '\"' ; i--); /*search the last '"' in 'oprands_list'*/
   if (!i)
   {
      printf("ERROR: In file \"%s\" line %d - The string of \"asciz\" guide word must end with another '\"' char.\n", file_name, line_number);
      return ERROR;
   }

   operands_list++;  /* skip the openning '"' */    
   for (j = 1 ; j < i ; j++) /* check if every char between the first and last '"' is printable.*/
   {
      if (!isprint(*operands_list))
      {
         printf("ERROR: In file \"%s\" line %d - In the string parameter of \"asciz\" guide word must be only printable chcharacters and '%c' is not printable.\n", file_name, line_number, *operands_list);
         return ERROR;
      }
      operands_list++; 
   }
   operands_list++; /* skip the closing '"' */ 
   operands_list = skip_whitespace(operands_list);

   if (*operands_list != '\0')
   {
      if ((*operands_list == ',') && (*(skip_whitespace(++operands_list)) == '\0'))
         printf("ERROR: In file \"%s\" line %d - The string parameter of \"asciz\" guide word cant be followed by ','.\n", file_name, line_number);
      else
         printf("ERROR: In file \"%s\" line %d - The \"asciz\" guide word must have only one parameter.\n", file_name, line_number);
      return ERROR;
   }
   return (!ERROR);
}




/* The function get two string whice are the first word in line and the rest of the line, it also get the name of the file containing the line and the line's number in the file.
   The function check if the word is command name in asmbly. If no, it print error message and return 1.
   If yes, it will call to another function to check the rest of the line acording to the command's type and return what that function return. */
int check_command (char *word, char *operands_list, int line_number, char *file_name)
{
   int command_type = search_in_command_table (word, type);

   if ((command_type != NUM_OF_COMMANDS) && (strcmp(word, "stop")))
   {
      CHECK_LIST_EMPTY_OR_START_COMMA(operands_list) /* check if the oprand list is empty or have ',' before the first operand. */
   }
   switch (command_type)
   {
   case (R):
      return check_R_command_operands (word, operands_list, line_number, file_name);
   case (I):
      return check_I_command_operands(word, operands_list, line_number, file_name);
   case (J):
      return check_J_command_operands(word, operands_list, line_number, file_name);
   }
   printf("ERROR: In file \"%s\" line %d - There is no command named \"%s\".\n", file_name, line_number, word);
   return ERROR;
}




/* The function get two strings one is name of R-type command in asmbly, and the seconed is oprands list.
   It also get the file's name whice the command and oprand list are from and the number of the line in the file containing those strings.
   The function check if 'oprands_list' is valid oprands list for the command according to asmbli language.
   If no, it print error message and return 1. If yes, the  function return 0. */
int check_R_command_operands(char *command, char *operands_list, int line_number, char *file_name)
{
   char *operand;
   int i ,c_opcode = search_in_command_table (command, opcode); /*search_in_command_table return 1 or 0 according to the command opcode.*/

   operand = strtok (operands_list, ","); /* Between every two operands must be comma so (if 'operands_list' is valid) 'operand' will point to one oprand.*/
   if (check_reg_operand (operand, line_number, file_name)) /* all operands in R-type command are registers*/
      return ERROR;

   for (i = 1 ; i >=  c_opcode ; i--) /* command with opcode 1 need two operands while command with opcode 0 need three operands. so the loop will run one more time for command with opcode 0.*/
   {                                 
      CHECK_FEW_OPERANDES_OR_CONSECUTIVE_COMAS(operand) /* chcek if there is no more operands in the list or if two operands are not seperated by ','. */
      if (check_reg_operand (operand, line_number, file_name))
         return ERROR;  
   }
   CHECK_END_OPRANDS_LIST(operand) /*check if the line end or there are more oprands*/
   return(!ERROR); 
}




/* The function get two strings, one is name of I-type command in asmbly, and the seconed is oprands list.
   It also get the file's name whice the command and oprand list are from and the number of the line in the file containing those strings.
   The function check if 'oprands_list' is valid oprands list for the command according to asmbli language.
   If no, it print error message and return 1. If yes, the  function return 0. */   
int check_I_command_operands(char *command, char *operands_list, int line_number, char *file_name)
{
   char *operand, label[MAX_LABEL_LEN];
   int isnegative = off, num = ZERO;

   operand = strtok (operands_list, ","); /* Between every two operands must be comma so (if 'operands_list' is valid) 'operand' will point to one oprand.*/

   if (check_reg_operand (operand, line_number, file_name)) /*the first operand in I-type command is integer*/
      return ERROR;
   CHECK_FEW_OPERANDES_OR_CONSECUTIVE_COMAS(operand) /* chcek if there is no more operands in the list or if two operands are not seperated by ','. */
   if (*command == 'b') /*the seconed operand in branch command is register*/
   {
      if (check_reg_operand (operand, line_number, file_name))
         return ERROR;
   }
   else /*the seconed operand in non-branch I-type command is integer*/
   {
      if (((*operand) == '-') || ((*operand) == '+')) /* numbers in the list can be signed.*/
      {
         isnegative = ((*operand) == '-' ? on : off);
         operand++;
      }
      if (!isdigit(*operand))
      {
         printf("ERROR: In file \"%s\" line %d - Seconed operand must be intger num.\n", file_name, line_number);
         return ERROR;
      }
      do
      {
         num = (num*10) + ((*operand)- ZERO_ASCII); /*reducing 48 from the ascii value of digit char will result in the integer value of that digit*/
      } while (isdigit(*(++operand)));
      num *=  (isnegative? -1 : 1);
      if ( (num >= pow(2,(IMMED_LEN-1))) || (num < (-1)*pow(2,(IMMED_LEN-1))) ) /*check if in the range for IMMED_LEN-bits signed intger */
      {
         printf("ERROR: In file \"%s\" line %d - The number %d is not in the size limit.\n", file_name, line_number, num);
         return ERROR;
      }
      if (*operand == '.')
      {
         printf("ERROR: In file \"%s\" line %d - Seconed operand must be intger num.\n", file_name, line_number);
         return ERROR;
      }
      CHECK_MISSING_COMA(operand) /* Check if there is another oprand in the string 'operand'*/
   } 

   CHECK_FEW_OPERANDES_OR_CONSECUTIVE_COMAS(operand) /* chcek if there is no more operands in the list or if two operands are not seperated by ','. */
   
   if (*command != 'b') /*the thired operand in branch command is register*/
   {
      if (check_reg_operand (operand, line_number, file_name))
         return ERROR;
   }
   else /*the thired operand in non-branch I-type command is label*/
   {
      operand = get_word( operand, label); /* 'label' is the first word in 'operand'*/
      if (check_label (label, line_number, file_name))
         return ERROR;

      CHECK_MISSING_COMA(operand) /* Check if there is another oprand in the string 'operand'*/
   }  
   CHECK_END_OPRANDS_LIST(operand) /*check if the line end or there are more oprands*/
   return(!ERROR);       
}




/* The function get two strings, one is name of J-type command in asmbly, and the seconed is oprands list.
   It also get the file's name whice the command and oprand list are from and the number of the line in the file containing those strings.
   The function check if 'oprands_list' is valid oprands list for the command according to asmbli language.
   If no, it print error message and return 1. If yes, the  function return 0. */  
int check_J_command_operands(char *command, char *operands_list, int line_number, char *file_name)
{
   char *operand;

   operands_list = skip_whitespace(operands_list);
   if (!strcmp(command, "stop")) /* 'stop' command must have no operands*/
   {
      if (*operands_list == '\0')
         return (!ERROR);
      printf ("ERROR: In file \"%s\" line %d - After \"stop\" command can be only spaces or tabs in the line.\n", file_name, line_number);
         return ERROR;
   }
   if ((!strcmp(command, "call")) || (!strcmp(command, "la"))) 
      return check_label_operand (operands_list, line_number, file_name); /* 'call', 'la' commands have only one operand and its label*/

   /*if its non of the above, the commad is 'jmp' whice can have register or label as operand.*/
   if (*operands_list != '$') 
      return check_label_operand (operands_list, line_number, file_name); /*if the first char isnt '$' the operand (if valid) can only be label.*/

   operand = strtok(operands_list, ","); /*in case there are more then one operand in the list the function take the first and check if it valid register name.*/
   if (check_reg_operand (operand, line_number, file_name))
      return ERROR;

   CHECK_END_OPRANDS_LIST(operand) /*check if the line end or there are more oprands*/
   return(!ERROR);  
}




/* The function get the string 'operand' , the file's name whice the string is from and the number of the line in the file containing this strings.
   The function check if the string containg valid register.
   If the function found error it print message and return 1, otherwise it return 0/ */
int check_reg_operand (char *operand, int line_number, char *file_name)
{
   int reg_num = ZERO;

   if (*operand != '$')
   {
      printf("ERROR: In file \"%s\" line %d - The operand \"%s\" is not register.  Register name must start with '$'.\n", file_name, line_number, operand);
      return ERROR;
   }
   operand++;
   if (!isdigit(*operand))
   {
      printf("ERROR: In file \"%s\" line %d - The operand \"$%s\" command is not register.  Register name must have positive intger after the '$'.\n", file_name, line_number, operand);
      return ERROR;
   }
   do 
   {
      reg_num = (reg_num*10) + (*operand - ZERO_ASCII); /*reducing 48 from the ascii value of digit char will result in the integer value of that digit*/
   } while (isdigit(*(++operand)));

   if ((*operand) != '\0')
   {
      if (((*operand) == ' ') || ((*operand) == '\t'))
      {
         CHECK_MISSING_COMA(operand) /* Check if there is another oprand in the string 'operand'*/
      }
      else
      {
         printf("ERROR: In file \"%s\" line %d - Operand is not register. Register name is end after his number and in the command line it can be followed only by sapce, tab or coma.\n", file_name, line_number);
         return ERROR;
      }   
   }
   if (reg_num >= REG_AMOUNT) 
   {
      printf("ERROR: In file \"%s\" line %d - There is no register numbered %d.  Registers numbered from '0' to '%d'.\n", file_name, line_number, reg_num, (REG_AMOUNT-1));
      return ERROR;
   }    
   return (!ERROR);
}




/* The function get string containing line, the file's name that the line is from the number of the line in that file. 
   The function check if the last or the first non-whitespace char in the line is comma or if there are two consecutive commas.
   If the function found it print error message and return 1, otherwise it return 0.  */
int search_comas_errors( char *line, int line_number, char *file_name)
{
   int i;
   line = skip_whitespace(line);
   if (*(line) == ',')
   {
      printf("ERROR: In file \"%s\" line %d - First non-whitespace charcter cant be ','.\n", file_name, line_number);
      return ERROR;
   }
   for (i = 0 ; i < strlen(line)-1 ; i++)
   {
      if ((*(line+i) == ',') && (*(line+i+1) == ','))
      {
         printf("ERROR: In file \"%s\" line %d - Two consecutive ','.\n", file_name, line_number);
         return ERROR;
      }
   }
   line = line + strlen(line)-1;
   while ((*line == ' ') || (*line == '\t'))
      line--;
   if (*(line) == ',')
   {
      printf("ERROR: In file \"%s\" line %d - Last non-whitespace charcter cant be ','.\n", file_name, line_number);
      return ERROR;
   }
   return (!ERROR);
}





 





















