    .text
    .global process

    .text
scn_format:     .asciz   "%d"
prid_format:    .asciz   "%d "
new_line:       .asciz   "\n"
process:
    push    %ebp
    mov     %esp, %ebp
    push    %edi
    sub     $4, %esp
    lea     -8(%ebp), %eax
    push    %eax
    push    $scn_format
    call    scanf
    add     $8, %esp
    mov     -8(%ebp), %edi
st_cycle:
    cmp     $0, %edi
    je      end_cycle
    sub     $4, %esp
    lea     (%esp), %eax
    push    %eax
    push    $scn_format
    call    scanf
    add     $8, %esp
    dec     %edi
    jmp     st_cycle
end_cycle:
    mov     -8(%ebp), %edi
st_pri_cycle:
    cmp     $0, %edi
    je      end_pri_cycle
    push    $prid_format
    call    printf
    dec     %edi
    add     $8, %esp
    jmp     st_pri_cycle
end_pri_cycle:
    push    $new_line
    call    printf
    //add     $8, %esp
    mov     -4(%ebp), %edi
    mov     %ebp, %esp
    pop     %ebp