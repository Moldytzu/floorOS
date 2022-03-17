bits 64

extern _Z9dummy_isrv
extern _Z14divide_by_zerov
extern _Z14timer_callbackv
global isr_no_err_stub_257
global isr_divide_by_zero
global isr_pit_irq


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


%macro isr_err_stub 1
isr_stub_%+%1:
    push %1
    cld
    pushAll
    pushfq
    call _Z9dummy_isrv
    popfq
    popAll
    pop rdi ; pop the interrupt code we pushed into the stack into the rdi register
    iretq
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
    push %1
    pushAll
    cld
    call _Z9dummy_isrv
    popAll
    pop rdi ; pop the interrupt code we pushed into the stack into the rdi register
    iretq
%endmacro


isr_divide_by_zero:
    push 0
    cld
    pushAll
    pushfq
    call _Z14divide_by_zerov
    popfq
    popAll
    pop rdi ; pop the interrupt code we pushed into the stack into the rdi register
    iretq


isr_pit_irq:
    push 0
    cld
    pushAll
    pushfq
    call _Z14timer_callbackv
    popfq
    popAll
    pop rdi ; pop the interrupt code we pushed into the stack into the rdi register
    iretq

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

global isr_stub_table

isr_stub_table:
%assign i 0
%rep 32
    dq isr_stub_%+i
%assign i i+1
%endrep


