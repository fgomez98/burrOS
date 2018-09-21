GLOBAL keyPressed
GLOBAL getKey
section .text

keyPressed:
	  push rbp
  	mov rbp, rsp
	  in al, 64h
  	and al, 1h
  	mov rsp, rbp
    pop rbp
  	ret

getKey:
  	push rbp
  	mov rbp, rsp
  	mov rax, 0h
  	in al, 60h
    ;seteamos en 0 el controlador de keypressed
    mov bl, al
    in al, 64h
    and al, 11111110b
    out 64h, al
    mov al, bl
  	mov rsp, rbp
  	pop rbp
  	ret