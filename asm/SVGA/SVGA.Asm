

.8086

code 	segment
ORG 100H
start:
 	push 	ax

 	mov 	ah, 4fh   ;function number
 	mov 	al, 2   ;to set graphic mode
 	mov 	bx, 101h  ;640*480 @ 256
 	int 	10h

 	pop 	ax
 	retf
	
code 	ends
end 	start