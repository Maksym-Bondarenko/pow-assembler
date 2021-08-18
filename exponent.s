.intel_syntax noprefix
.text
.section	.rodata
.LC0:
    .text
    .globl	exponent

# e^x = 1 + (x/1) (1 + (x/2) (1 + (x/3) (........) ) ) 

# Input:  xmm0 = y * ln(x)
# Output: xmm0 = e^(y*ln(x))
exponent:
    push     rbp               # Stack-alignment
    mov      rbp, rsp
    sub      rsp, 16

    mov      rcx, 100          # Constant amount of Taylor-steps for accuracy; rcx = k

    movsd    xmm2, xmm0        # Save exponent y*ln(x)

    pxor     xmm1, xmm1        # Accumulating temporal result = acc
    addsd    xmm1, .LC1[rip]

.L0: 
    cvtsi2sd xmm3, rcx         # convert k to perform (y*ln(x))/k in xmm registers
    divsd    xmm0, xmm3        # (y*ln(x)) / k
    
    mulsd    xmm1, xmm0        # acc *= y*ln(x) / k 
    addsd    xmm1, .LC1[rip]   # acc + 1

    movsd    xmm0, xmm2        # Restore xmm0 = y *ln(x)
    
    dec      rcx
    cmp      rcx, 0
    jg .L0

    movsd    xmm0, xmm1
      
    leave
    ret


.section	.rodata
    .LC1:                      # Const = 1
        .long 0
        .long 1072693248
