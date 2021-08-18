.intel_syntax noprefix
.text
.section	.rodata
.LC0:
	.text
	.global	pow_int

# Input:  xmm0 = x, rdi = y
# Output: xmm0 = x^y (y is Integer)
pow_int:
    push rbp                # Stack-alignment
    mov rbp, rsp
    sub rsp, 16
    
    pxor xmm2, xmm2

    movsd xmm1, .LC1[rip]   # res = 1 = xmm1 (start-value)
    xor rax, rax            # int neg = 0 = rax (flag for negative y)
    cmp rdi, 0              # If (y > 0) -> y is negative
    jg .L0

# If y negative -> set flag (neg) and negate y
    mov rax, 1              # neg = 1
    neg rdi                 # y = -y

.L0:
    cmp rdi, 0
    jle .L1
    mulsd xmm1, xmm0        # res *= x
    dec rdi                 # y--

    comisd xmm1, xmm2       # If res = 0 -> return 0
    jz .L3
    
    cmp rdi, 0
    jg .L0

.L1:
    cmp rax, 1              # If (neg == 1) -> y is negative
    je .L2
    movsd xmm0, xmm1        # Else -> return res
    leave
    ret

.L2:                        # If y is negative -> return 1/res
    movsd xmm0, .LC1[rip]   # xmm0 = 1
    divsd xmm0, xmm1        # 1/res

    leave 
    ret
.L3:
    pxor xmm0, xmm0
    leave 
    ret

.section	.rodata
    .LC1:                   # 1
        .long 0
        .long 1072693248
