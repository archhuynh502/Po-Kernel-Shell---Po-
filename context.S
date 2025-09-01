.global save_context
.global load_context
.global timer_interrupt_handler

// writedisk to the register and sends it to the pcb thingy
// rdi = pointer to PCB
save_context:
    mov [rdi + 0x00], rsp
    mov [rdi + 0x08], rax
    mov [rdi + 0x10], rbx
    mov [rdi + 0x18], rcx
    mov [rdi + 0x20], rdx
    mov [rdi + 0x28], rsi
    mov [rdi + 0x30], rdi
    mov [rdi + 0x38], r8
    mov [rdi + 0x40], r9
    mov [rdi + 0x48], r10
    mov [rdi + 0x50], r11
    mov [rdi + 0x58], r12
    mov [rdi + 0x60], r13
    mov [rdi + 0x68], r14
    mov [rdi + 0x70], r15
    ret

// registers and rcp from PCB
// i used to know a bit of this
// only the Lord knows :(
load_context:
    mov rsp, [rdi + 0x00]
    mov rax, [rdi + 0x08]
    mov rbx, [rdi + 0x10]
    mov rcx, [rdi + 0x18]
    mov rdx, [rdi + 0x20]
    mov rsi, [rdi + 0x28]
    mov rdi, [rdi + 0x30]
    mov r8,  [rdi + 0x38]
    mov r9,  [rdi + 0x40]
    mov r10, [rdi + 0x48]
    mov r11, [rdi + 0x50]
    mov r12, [rdi + 0x58]
    mov r13, [rdi + 0x60]
    mov r14, [rdi + 0x68]
    mov r15, [rdi + 0x70]
    ret

// the timer thingy isr i think for irq0 
timer_interrupt_handler:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    extern scheduler_tick
    call scheduler_tick

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    mov al, 0x20        //  idk anymore i think it sends the EOI to  the PIC
    out 0x20, al
    iretq
