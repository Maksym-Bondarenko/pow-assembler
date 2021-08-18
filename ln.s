.intel_syntax noprefix
.text
.section .rodata
.LC0:
    .text
    .globl ln

# ln(x) = 2*SUM{[((x-1)/(x+1))^(2k-1)]/[2k-]}

# Input:  xmm0 = x
# Output: xmm0 = ln(x)
ln:
    push rbp                # Stack-alignment
    mov rbp, rsp
    sub rsp, 16

    movsd xmm4, .LC1[rip]
    mov rcx, .LC2[rip]      # 100
    movsd xmm1, xmm0
    movsd xmm2, xmm0        
    addsd xmm2, xmm4        # xmm2 = x+1
    subsd xmm1, xmm4        # xmm1 = x-1
    divsd xmm1, xmm2        # xmm1 = (x-1)/(x+1)

    movsd xmm2, xmm1        # Save (x-1)/(x+1)
    pxor xmm0, xmm0         # Result acc

    mov rcx, 100
    shl rcx, 1
    dec rcx
.L0:
    mov rax, rcx
.L1:  
    mulsd xmm1, xmm2        # xmm1 = acc * ((x-1)/(x+1))
    dec rax
    cmp rax, 1
    jne .L1
    
    cvtsi2sd xmm3, rcx      # Begin of next cycle
    divsd xmm1, xmm3
    addsd xmm0, xmm1
    movsd xmm1, xmm2
    sub rcx, 2
    cmp rcx, 1
    jg .L0                  # Jump-back to loop
    addsd xmm0, xmm2        # End
    mulsd xmm0, .LC2[rip]

    leave
ret


.section	.rodata
    .LC1:                   # 1
            .long 0
            .long 1072693248

    .LC2: 
            .long 0         # 2
            .long 1073741824
