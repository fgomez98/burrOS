GLOBAL beep
GLOBAL unBeep
section .text

beep:
    push rbp
    mov rbp, rsp

    mov al, 0xb6
    out 0x43, al
    mov rax, 0
;    mov al, 72549
    mov al, 11931
    out 0x42, al
    mov al, ah
    out 0x42, al
    in al, 0x61
    or al, 0x03
    out 61h, al

    mov rsp, rbp
    pop rbp
    ret

unBeep:
;call .pause1
    in al, 0x61
    and al, 0xFC
    out 0x61, al
    ret


.pause1:
    mov ecx, 65535
.pause2:
    dec ecx
    jne .pause2
    dec ebx
    jne .pause1
    in al, 0x61 ;apaga la nota
    and al, 0xFC ;apaga speaker
    out 0x61, al
    ret