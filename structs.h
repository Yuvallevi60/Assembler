#define MAX_LABLE_LEN 31
#define MAX_ATTRIBUTE_LEN 12  /* asuming the longest attributes possibol are "data , entry" or "code , entry"*/



enum code_size {bs = 1, hs = 2, ws = 4}; /* size of the code in btyes*/

/*struct used for the memory tables. evrey 'memory' object is line in the table*/
typedef struct memorys memory;
struct memorys {
   int address; /* the address in the code this memory represnt */
   enum code_size size; /* size in bytes of the memory. */
   unsigned char *machine_code; /*array that represnt the code of the memory. every char in the array is the ascii value of 8 bits of the code. the first char is the 8 MSB and so on. */
   memory *next; /*point to the next line (memory struct) in the table. point to NULL if its the last in the table or not in table. */
};





/*struct used for the symnols table. evrey 'symbol' object is line in the table*/
typedef struct symbols symbol;
struct symbols {
   char name[MAX_LABLE_LEN];
   int value; /* the address in the code this label used, or 0 in case of 'external' label */
   char attribute[MAX_ATTRIBUTE_LEN];
   symbol *next; /*point to the next line (symbol struct) in the table. point to NULL if its the last in the table or not in table. */
};



