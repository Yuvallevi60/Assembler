#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ZERO 0

enum code_size {bs = 1, hs = 2, ws = 4}; /* size of the code in btyes*/
enum flag {off, on};




/*prototypes to function from the file "general_functions.c"*/

char *skip_whitespace(char *string); /* The function get pointr to string and return pointr to the first char in the string that isnt tab or space*/

char *get_word(char *line, char *word); /*insert the first word in 'line' to 'word'. return pointr to the char after the first word end. */

void remove_file_extension (char *old_name, char *new_name); /*copy the string 'old_name' to new_name' and then delete the file name's suffix from 'new_name' */

int get_reg_num (char *reg_name); /* the function get string with name of register and return the number of the register*/

int get_data_size(char *data_type); /* The function get string data_type and return the size this type reprsent. */

void create_binary_code (char *binary_code, int code_length, int num); /* write in 'binary_code' array the value of 'num' in banry base, using 'code_length' digits and the two's complement method.*/

