.intel_syntax noprefix
.text
.section    .rodata
.LC0:
    .text
    .globl f_alternativ

# xmm0 = x, xmm1 = y
# return to xmm0
f_alternativ:
    push rbp
    mov rbp, rsp
    sub rsp, 32

    comisd xmm1, xmm1 # if y = nan
    jp .L7

    pxor xmm3, xmm3     # test if y = 0
    comisd xmm1, xmm3
    je .L6

    comisd xmm0, .LC1[rip] # test if x = 1
    je .L0 

    comisd xmm0, .LC2[rip] # test if x = -1
    je .L1 

    comisd xmm0, xmm0 # if x = nan
    jp .L0 

    comisd xmm0, xmm3 # test if x = 0
    je .L2

    comisd xmm1, .LC5[rip] # test if y=-inf
    je .L8

    comisd xmm0, .LC3[rip] # test if x=inf
    je .L4

    comisd xmm1, .LC3[rip] # test if y=inf
    je .L3

    movsd xmm3, xmm1        # save y to xmm3

    cvttsd2si rdi, xmm3             # (int) y ---> rdi                       
    movsd  qword ptr [rbp-16], xmm0 # push xmm0=x
    movsd  qword ptr [rbp-32], xmm1 # push xmm1=y
    push rdi

    call pow_int # xmm0=x, rdi=(int)y

    pop rdi
    movsd  xmm2, qword ptr [rbp-32] # pop y to xmm2

    movsd xmm1, xmm2

    cvtsi2sd xmm3, rdi
    comisd xmm2, xmm3
    jz .L0 

    subsd xmm2, xmm3    # xmm2= float part of y

    movsd xmm3, xmm0 # xmm3 result from my_pow_int
    movsd xmm0, qword ptr [rbp-16] # pop x to xmm0

    movsd  qword ptr [rbp-16], xmm3 # push xmm3=x^((int) y)
    movsd  qword ptr [rbp-32], xmm2 # push xmm2=float part of y

    call ln_alternativ # xmm0=x; return = ln(x) in xmm0

    movsd xmm1, qword ptr [rbp-32] # pop xmm1=float part of y
    mulsd xmm0, xmm1 # y * ln(x)

    call exponent

    movsd xmm1, qword ptr [rbp-16] # pop x^((int) y)
    mulsd xmm0, xmm1
.L0: 
    leave
    ret
.L1:
    cvttsd2si rdi, xmm1
    and rdi, 1
    cmp rdi, 1  
    je .L0  # ungerade
    mulsd xmm0, .LC2[rip]
    leave
    ret

.L2: # only if x = 0
    pxor xmm3, xmm3
    comisd xmm1, xmm3 # test if y = 0          
    jbe .L5           # if y<0 return inf
    leave
    ret

.L3: 
    comisd xmm0, .LC1[rip]  # test if -1<x<1 case y = inf
    jae .L5                  # x>1
    comisd xmm0, .LC2[rip]
    jbe .L5                  # x>-1
    movsd xmm0, xmm3
    leave 
    ret

.L4: 
    comisd xmm1, xmm3       # test if y<0 case x = inf
    jbe .L8

.L5: 
    movsd xmm0, .LC3[rip]     # inf 
    leave
    ret

.L6: # if x = 0 and y = 0
    movsd xmm0, .LC1[rip]   # 1
    leave
    ret

.L7: # if y = nan           # nan/-nan
    comisd xmm0, .LC1[rip]
    je .L0
    # por xmm1, xmm0          # check if x neg
    movsd xmm0, xmm1
    leave
    ret

.L8:
    comisd xmm0, .LC1[rip]  # test if -1<x<1 case y = -inf
    ja .L9
    comisd xmm0, .LC2[rip]
    jae .L5

.L9:
    pxor xmm0, xmm0
    leave 
    ret


.section    .rodata
    .LC1:                   # 1
        .long 0
        .long 1072693248
    .LC2:                   # -1
        .long 0
        .long 3220176896
    .LC3:                   # inf
        .long 0
        .long 2146435072
    .LC4:                   # -inf
        .long 0
        .long 4293918720
    .LC5:                   # mask 0x800..00
        .long 0
        .long 2147483648
  