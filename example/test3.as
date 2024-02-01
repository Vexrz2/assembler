.entry LIST 
.extern W
inc  r6
sub r1, r4 
macro m1        
	inc  r6       
	mov r3, W
endm
sub r7, r5
m1
LIST:              .data  6, -9	     
stop
