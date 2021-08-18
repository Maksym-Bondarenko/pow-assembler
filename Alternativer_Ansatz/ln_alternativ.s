.intel_syntax noprefix
.text
.section	.rodata

.LC3:
	.text
	.global	ln_alternativ

# xmm0 == a
# return to xmm0
ln_alternativ:
    push rbp
    mov rbp, rsp
    sub rsp, 80

    pxor xmm3, xmm3  # declare temp result

    movsd xmm4, xmm0 # xmm4 == step == a

    subsd xmm4, .LC0[rip] # xmm4 == step == a - 1

    mulsd xmm4, .LC1[rip] # xmm4 == step == (a-1)*0.01
    
    movsd xmm5, xmm0 # save xmm0=a
    
    movsd xmm0, .LC0[rip] # double i = 1; xmm0 == i
    movsd xmm1, .LC0[rip] # xmm1 = 1

    addsd xmm1, xmm4 # xmm1 = i + step

.L0:        # for (double i = 1 ; i < a; i += step)
        # xmm0 = i
        # xmm1 = i += step
        # xmm3 = sum of all steps simpson(i, i + step);
        # xmm4 = step
        # xmm5 = a        
        movsd  qword ptr [rbp-16], xmm0 # push xmm0 to stack
        movsd  qword ptr [rbp-32], xmm1 # push xmm1 to stack
        movsd  qword ptr [rbp-48], xmm3 # push xmm3 to stack
        movsd  qword ptr [rbp-64], xmm4 # push xmm4 to stack
        movsd  qword ptr [rbp-80], xmm5 # push xmm5 to stack, its our a

        # call simpson(xmm0 = i, xmm1 = i + step)
        call simpson # return -> xmm0

        movsd  xmm5, qword ptr [rbp-80] # pop xmm5 back from stack
        movsd  xmm4, qword ptr [rbp-64] # pop xmm4 back from stack
        movsd  xmm3, qword ptr [rbp-48] # pop xmm3 back from stack
        movsd  xmm1, qword ptr [rbp-32] # pop xmm1 back from stack

        addsd xmm3, xmm0 # result(xmm3) += simpson(return value in xmm0)

        movsd  xmm0, qword ptr [rbp-16] # pop xmm0 back from stack

        addsd xmm0, xmm4 
        
        addsd xmm1, xmm4 # xmm1 = i + step

        comisd xmm0, xmm5
        jb .L0

        movsd xmm0, xmm3

    leave
ret

.section	.rodata
    .LC0:                            # 1
        .long 0
        .long 1072693248

    .LC1:                           # 0.01
        .long   1202590843
        .long   1065646817

    # .LC1:                          # 0.0000001, for big numbers
    #   .long 2596056904
    #   .long 1048238066
