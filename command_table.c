#include <string.h>
#include <stdio.h>

#define NUM_OF_COMMANDS 27

enum command_table_columns {name, type, funct, opcode};
enum command_type { R, I, J};

typedef struct commands{
char *name;
int type;
int funct;
int opcode;
} command;


/* The function get name of command and number of column.
   The function search in  command table it bulit a command with the name she got.
   If the name found in the table the function return the value in the the row of the command and the column she got, except if the column is the names colimn, then it return the row's number of the command.
   If the name didnt found the total number of command in the table.*/
int search_in_command_table (char *command_name, int column)
{
   command commands_table[] = {
   {"add", R, 1, 0}, {"sub", R, 2, 0}, {"and", R, 3, 0}, {"or", R, 4, 0}, 
   {"nor", R, 5, 0}, {"move", R, 1, 1}, {"mvhi", R, 2, 1}, {"mvlo", R, 3, 1}, 
   {"addi", I, 0, 10}, {"subi", I, 0, 11}, {"andi", I, 0, 12}, {"ori", I, 0, 13},
   {"nori", I, 0, 14}, {"bne", I, 0, 15}, {"beq", I, 0, 16}, {"blt", I, 0, 17},
   {"bgt", I, 0, 18}, {"lb", I, 0, 19}, {"add", I, 0, 20}, {"sb", I, 0, 21},
   {"sw", I, 0, 22}, {"lh", I, 0, 23}, {"sh", I, 0, 24}, {"jmp", J, 0, 30},
   {"la", J, 0, 31}, {"call", J, 0, 32}, {"stop", J, 0, 63} };

   int i;
   for (i = 0 ; i < NUM_OF_COMMANDS ; i++)
      if (!strcmp(commands_table[i].name, command_name))
         break;
   
   if (i == NUM_OF_COMMANDS)
      return i;

   switch (column)
   { 
      case type:
         return commands_table[i].type;
      case funct:
         return commands_table[i].funct;
      case opcode:
         return commands_table[i].opcode;
      default:
         return i;   
   }      
}




















