#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define ZERO 0
#define ZERO_ASCII 48 /* the asci value of '0'*/
#define NUM_OF_COMMANDS 27
#define BITS 8
#define MAX_LINE_LEN 82
#define MAX_LABEL_LEN 31 
#define ERROR 1
#define REG_AMOUNT 32
#define IMMED_LEN 16

#define CHECK_MISSING_COMA(Variable) \
         Variable = skip_whitespace (Variable);\
         if (*Variable != '\0')\
         {\
            printf("ERROR: In file \"%s\" line %d - Missing ',' between parameters.\n", file_name, line_number);\
            return ERROR;\
         }

#define CHECK_LIST_EMPTY_OR_START_COMMA(Variable)\
   Variable = skip_whitespace(Variable); \
   if ((*Variable == '\0') || (*Variable == ',')) \
   {\
      if (*Variable == '\0')\
         printf("ERROR: In file \"%s\" line %d - Operands list is empty.\n", file_name, line_number);\
      else\
         printf("ERROR: In file \"%s\" line %d - Cant write ',' before the first operand.\n", file_name, line_number);\
      return ERROR;\
   } 

#define CHECK_FEW_OPERANDES_OR_CONSECUTIVE_COMAS(operand)\
      operand = strtok (NULL, ","); \
      if (operand == NULL)\
      {\
         printf("ERROR: In file \"%s\" line %d - Too few operands.\n", file_name, line_number);\
         return ERROR;\
      }\
      operand = skip_whitespace(operand);\
      if (*operand == '\0')\
      {\
         if ((operand = strtok (NULL, ",")) == NULL)\
            printf("ERROR: In file \"%s\" line %d - Too few operands.\n", file_name, line_number);\
         else\
            printf("ERROR: In file \"%s\" line %d - Two consecutive ','.\n", file_name, line_number);\
         return ERROR;\
      }

/*check if the line end or there are more oprands*/
#define CHECK_END_OPRANDS_LIST(operand)\
         operand = strtok (NULL, ",");\
         if (operand != NULL)\
         {\
            printf("ERROR: In file \"%s\" line %d - Too many operands.\n", file_name, line_number);\
            return ERROR;\
         }

enum command_table_columns {name, type, funct, opcode};
enum command_type { R, I, J};
enum flag {off, on};

enum code_size {bs = 1, hs = 2, ws = 4}; /* size of the code in btyes*/


/*prototypes to function from the file "check_line.c"*/

int check_line_len (FILE *fp, char *line, int line_number, char *file_name); /*check if the line is longer then MAX_LINE_LEN*/

int empty_or_comment (char *line); /*check if line is empty or comment line*/
 
int examine_the_line (FILE *fp, char *line, int line_number, char *file_name); /*check if the line have syntactic errors acording to the assembly language*/

int check_label (char *label, int line_number, char *file_name); /*check if the sting 'label' is valid name for label*/

int check_guideword (char *word, char *operands_list, int line_number, char *file_name); /*check if 'word' is guide word and 'operands_list' is vaild oprand list for that guide word*/

int check_label_operand(char *operands_list, int line_number, char *file_name); /*check if 'operands_list' is valid as oprand list that need to contain just one label*/

int check_numbers_operand(char *guideword, char *operands_list, int line_number, char *file_name); /*check if 'operands_list' is valid as oprand list that need to contain list of numbers from the type of 'guideword'*/

int check_string_operand(char *operands_list,int line_number,char *file_name); /*check if 'operands_list' is valid as oprand list that need to contain just one string*/

int check_command (char *word, char *operands_list, int line_number, char *file_name); /*check if 'word' is command name and 'operands_list' is vaild oprand list for that command*/

int check_R_command_operands(char *command,char *operands_list,int line_number,char *file_name);/*check if 'operands_list' is valid as oprand list for R-type commands*/

int check_I_command_operands(char *command,char *operands_list,int line_number,char *file_name);/*check if 'operands_list' is valid as oprand list for I-type commands*/

int check_J_command_operands(char *command,char *operands_list,int line_number,char *file_name);/*check if 'operands_list' is valid as oprand list for J-type commands*/

int check_reg_operand (char *operand, int line_number, char *file_name); /*check if 'operand' is valid as register name*/

int search_comas_errors(char *line, int line_number, char *file_name); /*check if the last or the first non-whitespace char in the line is comma or if there are two consecutive commas*/




/*prototypes to function from other files*/

char *get_word(char *line, char *word); /* point 'word' to the first word in 'line'. return point to the cahr after the first word*/

char *skip_whitespace(char *pc); /* return pointer to the first non whitespace char in 'pc' string*/

int search_in_command_table (char *command_name, int column); /* serach for 'command_name' in the command table and, if found, return the value of the 'column' colimn in the found line. if not found, return the numbers of commands. */

int get_data_size(char *data_type); /* return the size in bytes of the data type in the string 'data_type'. */










