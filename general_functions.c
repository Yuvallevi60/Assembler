#include "general_functions.h"



/* The function get pointr to string and return pointr to the first char in the string that isnt tab or space*/
char *skip_whitespace(char *string)
{
   while ((*string == ' ') || (*string == '\t'))
      string++;
   return string;
}



/* The function get two pointers to strings.
   The function insert the first word in 'line' to 'word'. 
   The function return pointr to the place after the first word ended.*/
char *get_word(char *line, char *word)
{
   line = skip_whitespace (line); 

   while ((*line != ' ') && (*line != '\t') && (*line != '\n') && (*line != '\0'))
      *(word++) = *(line++);

   *word = '\0';
   return line;
}




/*The function get two pointers to char. old_name is string cointing file name and new_name is pointer to space at least the size of old_name.
  The function copy the string at old_name to new_name and then delete the file name's suffix from new_name*/
void remove_file_extension (char *old_name, char *new_name)
{
   new_name = strcpy (new_name, old_name);
   *(strrchr(new_name, '.')) = '\0';
}




/* the function get string with name of register and return the number of the register*/
int get_reg_num (char *reg_name)
{
   reg_name = skip_whitespace (reg_name); /* there may be white space before the name */
   reg_name++; /* skip the '$' in the start of register name*/
   return atoi( reg_name);
}




/* The function get string data_type and return the size this type reprsent. */
int get_data_size(char *data_type)
{
   if (!strcmp(data_type,".db")) 
      return bs;   /* bs = 1 , which mean 8-digits long (1 byte) binary number*/
   if (!strcmp(data_type,".dh"))
      return hs;   /* hs = 2 , which mean 16-digits long (2 byte) binary number*/
   return ws;     /* ws = 4 , which mean 32-digits long (4 byte) binary number*/
}




/* The function get integer 'code_length', 'binary_code' a array at least the size of 'code_length' and intger 'num'.
   The function fill the binary value of 'num' in the first (code_length) cells of the array.
   every in cell 'binary_code' represnt binary digit when in (binary_code[code_length-1]) will stored the least sgnifecnt digit of 'num'.
   unused cells wil be fill with '0' char and if 'num' is negative its value will be stored in the two's complement method. */
void create_binary_code (char *binary_code, int code_length, int num)
{
   int  i , isnegative;  
   
   isnegative = ((num < ZERO) ? on : off);
   num = fabs(num);

   for (i = (code_length)-1 ; i >= 0 ; i--)
   {
      binary_code[i] = (num%2 ? '1' : '0');
      num /= 2;
   }

   if (isnegative)
   {
      for (i = (code_length)-1 ; binary_code[i] == '0' ; i--); /* serach the last '1' in the array which is the least sagnifecnt 1 in the num*/
      for (--i; i >= 0 ; i--) /* change every cell before the last '1' which means change every digit after the first 1 in num  */ 
         binary_code[i] = (binary_code[i] == '1' ? '0' : '1');
   } 
} /* now the first (code_size) cells in 'binary_code' array are string represent the banary value of num. */



