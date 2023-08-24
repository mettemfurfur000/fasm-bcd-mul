format ELF

public bcd_mul

section '.text' executable

; newer code, works fine

; bcd_mul - multiplies two BCD numbers represented as arrays of bytes
; input:
;   eax - pointer to the first input array
;   edi - pointer to the second input array
;   ecx - pointer to the output array
;   ebx - size of the input arrays in bytes
; output:
;   none
; modifies:
;   eax, ebx, ecx, edx, esi, edi, esp, ebp
;
bcd_mul:
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi

	mov esi, [ebp+8]				; esi = pointer to the first input array
	mov edi, [ebp+12]			   	; edi = pointer to the second input array
	mov ebx, [ebp+16]			   	; ebx = pointer to the output array
	mov ecx, [ebp+20]			   	; size of input arrays

	cmp esi, 0					  	; if any of the pointers are null, exit from function
	je .exit
	cmp edi, 0
	je .exit
	cmp ebx, 0
	je .exit
	cmp ecx, 0
	je .exit

	mov edx, 0					  	; outer loop counter, points to second digit to multiply
.outer_loop:
	mov ecx, 0					  	; inner loop counter, points to first digit to multiply
.inner_loop:
	mov al,	 [esi+ecx]		   		; load digit from the first array
	mul byte	[edi+edx]		   	; load digit from the second array and multiply the digits
	aam							 	; adjust the result
	
	push esi						; save pointer to input_1 array
	mov esi, ebx					; copy output pointer
	add esi, ecx					; add inner counter
	add esi, edx					; add outer counter
	add [esi], byte al			  	; add lower digit to output array
	; call .carry_prop_proc			; propogate carry, procedure will save any registers we want to use
	add [esi+1], byte ah			; add lower digit to output array, using edx
	call .carry_prop_proc		   	; propogate carry
	pop esi						 	; restore input_1 pointer

	inc ecx							; move to the next digit
	cmp ecx, [ebp+20]			   	; compare counter to size of array
	jb .inner_loop				 	; repeat
	inc edx					  		; move to the next digit in the second array
	cmp edx, [ebp+20]			   	; check if all digits have been processed
	jb .outer_loop				 	; repeat
.exit:
	xor eax, eax					; clear the return value
	pop edi						 	; restore registers
	pop esi
	pop ebx
	mov esp, ebp
	pop ebp
	ret							 	; return to the caller






.carry_prop_proc:
	push esi						; save esi
	push ecx						; save ecx
	push edx						; save eax
	mov esi, ebx					; copy output pointer
	mov ecx, 0					  	; use ecx as counter
	mov edx, [ebp+20]			   	; copy array size
	add edx, edx					; twice for real output size
	dec edx						 	; -1 for better work, use edx as size of array
.cr_prop_loop:
	mov al, [esi]				   	; copy byte
	cmp al, 0x0a					; check for carry
	jb .adj_skip_proc			   	; if digit is ok (<10), skip
	add al, 0x06					; fix to bcd
	mov ah, al					  	; copy to ah
	and al, 0x0f					; delete high digit
	mov [esi], al				   	; update on output
	shr ah, 4					   	; shift high byte to make lower
	and ah, 0x0f					; delete any other bits
	add [esi+1], ah				 	; add to output
.adj_skip_proc:
	inc ecx						 	; increment offset
	inc esi						 	; move to next byte
	cmp ecx, edx					; compare counter to output array size
	jb .cr_prop_loop				; loop while not all array processed
	pop edx						 	; load edx
	pop ecx						 	; load ecx
	pop esi						 	; restore esi
	ret							 	; return to caller