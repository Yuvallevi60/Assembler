  ;this file example errors that the aseembler will find in the second scan

.entry repeat
       .dh	 42,59  
	sw 	 $0,4,$10 
	bne	 $31,$9,LOOP
       .asciz	"string" 
	call	 vall
.extern LOOP
	jmp	 $4
	la	 notexist
  	add      $01,$15,$9
        ori	 $9,2,$9

.extern repeat
.entry notused
