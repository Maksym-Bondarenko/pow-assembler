.intel_syntax noprefix
.text
.section .rodata
.LC3:
    .text
    .global simpson

simpson:
    push rbp
    mov rbp, rsp
    sub rsp, 16
   
    movsd xmm2, xmm1     # declare temp variable result, xmm2 = (b - a) / 30
    subsd xmm2, xmm0    # b-a

    movsd xmm7, xmm2          # save b-a for later

    divsd xmm2, .LC0[rip] # now xmm2 = (b - a) / 30

    movsd xmm3, .LC4[rip]   # xmm3=1
    divsd xmm3, xmm0        # xmm3 = 1/a

    mulsd xmm7, .LC1[rip]       # xmm7 = inner_step = (b-a)/10

    mov rcx, 9

    pxor xmm5, xmm5
    pxor xmm6, xmm6

 .L0:                           # ungerade
    addsd xmm0, xmm7            # xmm0 =  a + inner_step * ung
    movsd xmm4, .LC4[rip]       # xmm4 = 1
    divsd xmm4, xmm0            # xmm4 = 1/(a + inner_step*ung)
    addsd xmm6, xmm4            # unger_acc= xmm5
    dec rcx 
    cmp rcx, 0
    je .L1
                                # gerade
    addsd xmm0, xmm7            # xmm0 =  a + inner_step * ger
    movsd xmm4, .LC4[rip]       # xmm4 = 1
    divsd xmm4, xmm0            # xmm4 = 1/(a + inner_step*ger)
    addsd xmm5, xmm4            # ger_acc= xmm5
    dec rcx
    cmp rcx, 0
    jg  .L0
    
 .L1:
    mulsd xmm6, .LC2[rip]       # unger_acc*4
    mulsd xmm5, .LC6[rip]       # ger_acc * 2
    addsd xmm3, xmm5            # acc + ger_acc
    addsd xmm3, xmm6            # acc + unger_acc

    movsd xmm0, .LC4[rip]       # xmm4 = 1
    divsd xmm0, xmm1            # now xmm0 = f(b)
   
    addsd xmm0, xmm3            # all f's
    mulsd xmm0, xmm2            # its return value

    leave
ret


.section .rodata
        .LC0:                           # 30
                .long 0
                .long 1077805056

        .LC1:                           # 0.1
                .long   2576980378
                .long   1069128089


        .LC2:                           # 4
                .long   0
                .long   1074790400


        .LC4:                            # 1
                .long 0
                .long 1072693248
        
        .LC6:   
                .long 0                  # 2
                .long 1073741824     
