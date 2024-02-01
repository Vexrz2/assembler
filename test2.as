
inc r5
macro m1        
	inc  r6
	inc r4
endm  
MAIN: mov r2, r4
m1
LOOP: prn r4
