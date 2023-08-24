; unused code
public add_bcd_btoe
public add_bcd_etob

; define the function
add_bcd_btoe:
    push ebp                    ; save the base pointer
    mov ebp, esp                ; set the base pointer
    push ebx                    ; save ebx
    push esi                    ; save esi
    push edi                    ; save edi

    ; initialize the carry saver byte
    mov byte [carry_saver], 0

    ; retrieve the function arguments
    mov edi, [ebp+8]            ; pInput1
    mov esi, [ebp+12]           ; pInput2
    mov ebx, [ebp+16]           ; pOutput
    mov ecx, [ebp+20]           ; size

    ; check if any of the pointers are null
    cmp edi, 0
    je .return
    cmp esi, 0
    je .return
    cmp ebx, 0
    je .return

    ; loop through the arrays and perform the addition of BCD numbers
.add_loop:
    mov al, [edi]               ; load the first BCD number
    daa                         ; decimal adjust AL
    adc al, [esi]               ; add the second BCD number and the carry flag from the previous byte
    adc al, byte [carry_saver]  ; add the carry flag from the previous byte
    daa                         ; decimal adjust AL
    mov [ebx], al               ; store the result in result array
    ; set the carry flag for the next byte if necessary
    mov byte [carry_saver], 0
    jnc .skip_carry
    mov byte [carry_saver], 1
    .skip_carry:

    add edi, 1                  ; increment all pointers to arrays
    add esi, 1
    add ebx, 1 

    loop .add_loop              ; repeat until all BCD numbers are added

    .return:
    pop edi                     ; restore edi
    pop esi                     ; restore esi
    pop ebx                     ; restore ebx
    mov esp, ebp                ; restore the stack pointer
    pop ebp                     ; restore the base pointer
    ret

; reverse function, more untuitive to people
add_bcd_etob:
    push ebp             ; save the base pointer
    mov ebp, esp         ; set the base pointer
    push ebx             ; save ebx
    push esi             ; save esi
    push edi             ; save edi

    ; retrieve the function arguments
    mov edi, [ebp+8]     ; pInput1
    mov esi, [ebp+12]    ; pInput2
    mov ebx, [ebp+16]    ; pOutput
    mov ecx, [ebp+20]    ; size

    ; check if any of the pointers are null
    cmp edi, 0
    je .return
    cmp esi, 0
    je .return
    cmp ebx, 0
    je .return

    ; loop through the arrays and perform the addition of BCD numbers
    add edi, ecx         ; point to the end of pInput1
    add esi, ecx         ; point to the end of pInput2
    add ebx, ecx         ; point to the end of pOutput
    sub edi, 1           ; also substract 1 for actual pointers to the end of arrays
    sub esi, 1
    sub ebx, 1
    
.add_loop:
    mov al, [edi]        ; load the first BCD number
    daa                  ; decimal adjust AL
    adc al, [esi]        ; add the second BCD number and the carry flag from the previous byte
    adc al, byte [carry_saver]; add the carry bit from the previous byte
    daa                  ; decimal adjust AL
    mov [ebx], al        ; store the result in result array
    
    ; set the carry flag for the next byte if necessary
    mov byte [carry_saver], 0
    jnc .skip_carry
        mov byte [carry_saver], 1
    .skip_carry:

    sub edi, 1           ; decrement all pointers to arrays
    sub esi, 1           ;
    sub ebx, 1           ;

    loop .add_loop       ; repeat until all BCD numbers are added

    .return:
    pop edi              ; restore edi
    pop esi              ; restore esi
    pop ebx              ; restore ebx
    mov esp, ebp         ; restore the stack pointer
    pop ebp              ; restore the base pointer
    ret






