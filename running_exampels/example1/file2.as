 ; file with no errors but with many withsaces

		start:    or	$13 , $05, $19  
  move  $2, $000018
 STEP:   la madrID
    	 	 

addi $5,  190, $5
madrID: subi $5 , -12, $12
.extern    BLAbla
 	 	 	
   ; message number 1 /*---*/
.extern notused
  	.entry  	NUMBER
NUMBER: .asciz " " "
  jmp $8
 .db -9 , 54 , 0 , 14 
 	.dw 89, 55, -2568
Add: stop 
call BLAbla

beq $9 , $6, start

blabla: add $01,$6   ,$31

.extern    BLAbla
.entry NUMBER

.asciz ""
