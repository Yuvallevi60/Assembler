#include "memory_table.h"




/* The function get pointer to pointer to memory table, string of binary code and its size and address.
   The function create 'machine_code' from the binary code.
   The fnction create new memory struct and store there the 'machine_code' and its address. 
   Then the function conect to 'new_memory_line' to the end of the table or to the start if the table is empty/*/
void add_memory_line (memory **memory_table, int size, int address, char *binary_code)
{
   memory *new_memory_line;
   unsigned char *machine_code;
   
   machine_code = (unsigned char *) calloc(size, sizeof(char));
   if (machine_code == NULL)
   {
      fprintf(stderr,"\nFailed to allocate space.\nThe program will stop running.\n");
      exit(0);
   }
   build_machine_code (machine_code, binary_code, ZERO, size*BITS);

   create_empty_line(&new_memory_line);
   fill_line( new_memory_line, address, size, machine_code);
   insert_line_to_table( new_memory_line, memory_table);
}




/* The function gets two integers numbers 'start_place' and 'code_length'.
   The function also get two char arrays, the first is 'binary_code' that is its cells from (start_place)-index to (start_place+code_length-1)-index represnts digits of binary code at length of code_length.
   The seconed is 'machine_code' and its size is ('binary_code' length)/8.
   The function does that the following:
    1. The function start with the 'start_place' char in 'binary_code' and the 'start_place'%8 *bit* (MSB order) in the 'start_place'/8 char of 'machine_code'.
        for example: if (start_place = 13) the function start with binary_code[13] and, by using 'mask', the 5 bit of machine_code[1].
    3. By using bitwise, if the char in 'binary_code' is '1' the function "turn on" the bit in 'machine_code' and if the char is '0' the function "turn off" the bit.
    4. The function advance to the next char and to the next bit in the cell of 'machine_code'.
    5. After 8 chars the function advance to the MSB bit of the next cell in 'machine_code'.
    6. The function do steps 2-5 in a loop for 'code_length' times. */
void build_machine_code (unsigned char *machine_code, char *binary_code, int start_place, int code_length)
{
   int i = start_place;
   unsigned char mask = 1 << (BITS - (start_place%BITS + 1)); /*set mask to the right starting bit*/

   while (i < start_place + code_length)
   {    
      if (binary_code[i] == '1') 
         machine_code[i/BITS] = machine_code[i/BITS] | mask; /*trun on the bit*/
      else
         machine_code[i/BITS] = machine_code[i/BITS] & (~mask); /*trun off the bit*/
      i++;
      if (i % BITS) /*chec if 'i' is not product of 8*/
         mask = mask >> 1; /* advance mask for next bit */
      else
         mask = 1 << (BITS-1); /* set mask for MSB bit */
   }
}




/* The function get the argument 'new_memory_line' a pointer to undefined pointer of memory struct .
   The function allcate space for pointer to memory struct and assigned its address in (*new_memory_line). */
void create_empty_line(memory **new_memory_line)
{
   *new_memory_line = (memory *) malloc(sizeof(memory));
   if (*new_memory_line == NULL)
   {
      fprintf(stderr,"\nFailed to allocate space.\nThe program will stop running.\n");
      exit(0);
   }
}




/* The function get pointer to memory struct 'line',  two integers 'address' and 'size', and char array 'machine_code'.
   The function assign *'line' members with , address, size and machine_code and set its 'next' to NULL.*/
void fill_line (memory *line, int address, int size, unsigned char *machine_code)
{    
   line->address = address;
   line->size = size;
   line->machine_code = machine_code;
   line->next = NULL;
}



/* The function get 'memory_table' apointer to pointer to memory memory table and 'new_line' a pointer to memory.
   The the memory table is empty the function asigned to (*memory_table) the adress of new_line.
   If not empty the function will go through the table and assigned new line to the next of the last line in the table. */
void insert_line_to_table(memory *new_line, memory **memory_table)
{
   memory *p = *memory_table;

   if (p == NULL) /*the table is empty*/
      *memory_table = new_line;
   else 
   {
      while (p->next != NULL) /* advance until the table's end. */
         p = p->next;
      p->next = new_line;
   }
}





/* The function get pointer to memory table and integr num.
   The function goes through every line in the table and add the value of 'num' to its addres.*/
void update_address(memory *memory_table, int num)
{
   while (memory_table != NULL)
   {
      memory_table->address += num;
      memory_table = memory_table->next;
   } 
}




/* The function get pointer to memory table,  and the integers 'address', 'code_place', 'code_length'.
   The function also get char array, 'binary_code' that its cells from (code_place)-index to (code_place+code_length-1)-index represnts digits of binary number at length of code_length.
   The function search in the table the line with the 'address' and gets pointer to the line's machine_code and then update it acording to the digit in 'binary_array'.
   The bits in the machine_code that will be update are from index code_place to index 'code_place'+'code_length'. */
void update_machine_code (memory *memory_table, int address, char *binary_code, int code_place, int code_length)
{
   unsigned char *machine_code;
   machine_code = get_machine_code (memory_table, address);
   build_machine_code (machine_code, binary_code, code_place, code_length);
}



/* The function get pointer to memory table and integr address.
   The function goes through the table until it find line with the sames address as the function got/
   If The function found it return pointer to the machine_code of that line.
   If The function didnt find it return NULL. */
unsigned char *get_machine_code (memory *memory_table, int address)
{
   if (memory_table == NULL)
      return NULL;
   while (memory_table->address != address)
      memory_table = memory_table->next;
   return memory_table->machine_code;
}





/* The function get pointer to memory table and integr address.
   The function goes through the table until it find line with the sames address as the fnction got/
   If The function found it return the size of that line.
   If The function didnt find it return -1. */
int get_memory_size (memory *memory_table, int address)
{
   if (memory_table == NULL)
      return NOT_FOUND;
   while (memory_table->address != address)
      memory_table = memory_table->next;
   return memory_table->size;
}





/* The function get memory table.
   The function recursively call herself with the next line in the table (if have).
   The function free the space alloacted to the table 'machine_code' and the table (as a struct) itslef. */
void delete_memory_table (memory *memory_table)
{
   if (memory_table != NULL)
   {
      delete_memory_table(memory_table->next);
      free(memory_table->machine_code);
      free(memory_table);
   }
}






