

.8086
code 	segment
ORG 100H
start:
	push ax
	push dx

	mov dx, 64h
	mov al, 0d1h
	out dx, al
	nop
	nop
	nop
	mov dx, 60h
	mov al, 0dfh
	out dx, al
	nop
	nop
	nop
	mov dx, 64h
	mov al, 0xff
	out dx, al
	nop
	nop
	nop

	pop dx
	pop ax
	
	retf
code 	ends

end 	start