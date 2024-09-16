  ;this file example errors that the aseembler will find in the first scan
        .asciz	" string without end  
        .asciz	" char after the string" end
        .asciz	end" char before the string" 
        .asciz	" comma after the sring" , 	   
        .asciz	," comma before the sring"
        .asciz	" one sring", "two string" *****
; to long comment -----------------------------------------------------------------
	.dw 25 48 
	.dh 25 , , 48 
	.db 12, -8 , 11250
	.db 5, -8.1 , 12
	.db ,5, -81 , 12 
add:	add      $3,$5,$9
        add      3,$5,$9
db:   ori	 $9,-5,$2
extern:	.dw	 31,-12
5hello:	jmp	 $20
toolonglabellllllllllllllllllllllllllll:	.db 5 , 12
        move	 $20,$4,$6
        move	 $ty,$4
        sw 	 $0    ,$10  , 4 
 	bgt	 $43,$2,END
	bne	 $31,$9
        call	 $5 
	jmp	     
        sub $26,,$5,$7
        or $26,$5,$5,$7
        stop $5
       , nori $26, 56,$5
        .db ,5, -81 , 12
goodline:  add      $3,$5,$9
goodline:  add      $3,$5,$9




