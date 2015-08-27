



code 	segment
start:
 	push 	ax

 	mov 	ah, 04fh   //function number
 	mov 	al, 02   //to set graphic mode
 	mov 	bx, 101h  //640*480 @ 256
 	int 	10h

 	pop ax
	
code 	ends
end 	start