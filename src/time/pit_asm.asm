extern _Z14timer_callbackv

%macro pushAll 0
    push rbp
    push r11
    push r10
    push r9
    push r8
    push rdi
    push rsi
    push rcx
    push rdx
    push rax
%endmacro


%macro popAll 0
    pop rax
    pop rdx
    pop rcx
    pop rsi
    pop rdi
    pop r8
    pop r9
    pop r10
    pop r11
    pop rbp
%endmacro


pit_irq_handler:
    cld
    pushAll
    pushfq
    call _Z14timer_callbackv
    mov al, 0x20
    out 0x20, al
    popfq
    popAll
    pop rdi
    iretq
    