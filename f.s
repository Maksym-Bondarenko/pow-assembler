.intel_syntax noprefix
.text
.section    .rodata
.LC0:
    .text
    .globl f

# f(x, y) = x^y = e^(y*ln(x))

# Input:  xmm0 = x, xmm1 = y
# Output: xmm0 = x^y
f:
    push      rbp                      # Stack-alignment
    mov       rbp, rsp
    sub       rsp, 32

# Testing border-values
    comisd    xmm1, xmm1               # If y = NaN
    jp .L7

    pxor      xmm3, xmm3               # If y = 0
    comisd    xmm1, xmm3
    je .L6

    comisd    xmm0, .LC1[rip]          # If x = 1
    je .L0 

    comisd    xmm0, .LC2[rip]          # If x = -1
    je .L1 

    comisd    xmm0, xmm0               # If x = NaN
    jp .L0 

    comisd    xmm0, xmm3               # If x = 0
    je .L2

    comisd    xmm1, .LC5[rip]          # If y = -INF
    je .L8

    comisd    xmm0, .LC3[rip]          # If x = INF
    je .L4

    comisd    xmm1, .LC3[rip]          # If y = INF
    je .L3

    movsd     xmm3, xmm1               # Save y to xmm3

    cvttsd2si rdi, xmm3                # Converting y to Integer, save in rdi                       
    movsd     qword ptr [rbp-16], xmm0 # Push xmm0 = x
    movsd     qword ptr [rbp-32], xmm1 # Push xmm1 = y
    push      rdi

    call pow_int                       # Call function 'pow_int.s' with input parameters: xmm0 = x, rdi = (int)y

    pop       rdi
    movsd     xmm2, qword ptr [rbp-32] # Pop y to xmm2

    movsd     xmm1, xmm2

    cvtsi2sd  xmm3, rdi                # If original y was an Integer -> end calculation and return the result
    comisd    xmm2, xmm3
    jz .L0 

    subsd     xmm2, xmm3               # Save in xmm2 float part of y

    movsd     xmm3, xmm0               # Save in xmm3 result from 'pow_int'-function
    movsd     xmm0, qword ptr [rbp-16] # Pop x to xmm0

    movsd     qword ptr [rbp-16], xmm3 # Push xmm3 = x^((int) y)
    movsd     qword ptr [rbp-32], xmm2 # Push xmm2 = float part of y

    call ln                            # Call function 'ln.s' with input parameters: xmm0 = x; return = ln(x) in xmm0

    movsd     xmm1, qword ptr [rbp-32] # Pop float part of y to y
    mulsd     xmm0, xmm1               # xmm0 = y * ln(x)

    call exponent

    movsd     xmm1, qword ptr [rbp-16] # Pop x^((int) y) to xmm1
    mulsd     xmm0, xmm1
.L0: 
    leave
    ret
.L1:
    cvttsd2si rdi, xmm1
    and       rdi, 1
    cmp       rdi, 1  
    je .L0                             # For odd
    mulsd     xmm0, .LC2[rip]
    leave
    ret

.L2:                                   # If x = 0
    pxor      xmm3, xmm3
    comisd    xmm1, xmm3               # If y = 0          
    jbe .L5                            # If y < 0 -> return INF
    leave
    ret

.L3:                                   # If y = INF
    comisd    xmm0, .LC1[rip]
    jae .L5                            # If x >= 1 -> return INF
    comisd    xmm0, .LC2[rip]
    jbe .L5                            # If x <= -1 -> return INF
    movsd     xmm0, xmm3               # Else -> return 0
    leave 
    ret

.L4:                                   # If x = INF
    comisd    xmm1, xmm3               # If y < 0
    jbe .L8

.L5: 
    movsd     xmm0, .LC3[rip]          # Return INF 
    leave
    ret

.L6:                                   # If x = 0 and y = 0
    movsd     xmm0, .LC1[rip]          # Return 1
    leave
    ret

.L7:                                   # If y = NaN/-NaN
    comisd    xmm0, .LC1[rip]          # If x = 1 -> return 1
    je .L0
    movsd     xmm0, xmm1               # Else -> return NaN
    leave
    ret

.L8:                                   # If y = -INF
    comisd    xmm0, .LC1[rip]          # If x > 1 -> return 0       If -1 < x < 1
    ja .L9
    comisd    xmm0, .LC2[rip]          # If x >= -1 -> return INF
    jae .L5                            # Else -> return 0

.L9:
    pxor      xmm0, xmm0               # Return 0
    leave 
    ret


.section    .rodata
    .LC1:                              # Const = 1
        .long 0
        .long 1072693248
    .LC2:                              # Const = -1
        .long 0
        .long 3220176896
    .LC3:                              # Const = INF
        .long 0
        .long 2146435072
    .LC4:                              # Const = -INF
        .long 0
        .long 4293918720
    .LC5:                              # Mask = 0x800..00
        .long 0
        .long 2147483648
  