# _entry should be the first function invoked when
# the QEMU CPU starts executing. 

.section .text
.global _entry
_entry:
        # CSE 536: Task 2.3
        # Load the end of bl_stack to the sp register
        # set up a stack for C.
        # bl_stack is declared in start.c,
        # with a 4096-byte stack per CPU.
        # sp = bl_stack + (hartid * 4096)
        la sp, bl_stack
        li a0, 1024*4
        csrr a1, mhartid
        addi a1, a1, 1
        mul a0, a0, a1
        add sp, sp, a0
        # jump to start() in start.c
        call start
.global spin
spin:
        j spin