assembler: assembler.o file_functions.o first_scan.o symbol_table.o memory_table.o command_table.o second_scan.o check_line.o general_functions.o structs.h 
	gcc -ansi -Wall -pedantic -g assembler.o file_functions.o first_scan.o symbol_table.o memory_table.o command_table.o second_scan.o check_line.o general_functions.o -o assembler -lm

assembler.o: assembler.c assembler.h structs.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

file_functions.o: file_functions.c file_functions.h
	gcc -c -ansi -Wall -pedantic file_functions.c -o file_functions.o

first_scan.o: first_scan.c first_scan.h structs.h 
	gcc -c -ansi -Wall -pedantic first_scan.c -o first_scan.o

symbol_table.o: symbol_table.c symbol_table.h structs.h
	gcc -c -ansi -Wall -pedantic symbol_table.c -o symbol_table.o

memory_table.o: memory_table.c memory_table.h structs.h
	gcc -c -ansi -Wall -pedantic memory_table.c -o memory_table.o

command_table.o: command_table.c
	gcc -c -ansi -Wall -pedantic command_table.c -o command_table.o

second_scan.o: second_scan.c second_scan.h structs.h
	gcc -c -ansi -Wall -pedantic second_scan.c -o second_scan.o

check_line.o: check_line.c check_line.h
	gcc -c -ansi -Wall -pedantic check_line.c -o check_line.o -lm

general_functions.o: general_functions.c general_functions.h
	gcc -c -ansi -Wall -pedantic general_functions.c -o general_functions.o 
