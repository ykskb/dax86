BITS 32
    org 0x7c00
	mov eax, 4
	mov ecx, 4
	
	; ZF: 0 ;
label1:
	loopz label1
    dec eax
	push ecx ; 4 -> 3 -> 2 -> 1 -> 0
	push eax ; 3 -> 2 -> 1 -> 0 (ZF: 1) -> next
	loopnz label1

	; ZF: 1 ;
	mov ecx, 4
label2:
	loopnz label2
	inc eax
	push ecx ; 4 -> 3 -> 2 -> 1 -> 0
	push eax ; 1 -> 2 -> 3 -> 4 -> next
	loopz label2

	mov ecx, 4
label3:
	dec eax
	push ecx ; 4 -> 3 -> 2 -> 1 -> 0
	push eax ; 3 -> 2 -> 1 -> 0 -> next
	loop label3

	mov ecx, 4
label4:
	inc eax
	push ecx ; 4 -> 3 -> 2 -> 1 -> 0
	push eax ; 1 -> 2 -> 3 -> 4 -> next
	loop label4
	push ecx ; 0

	mov ecx, 1
	jecxz label5
	push 0x1 ; comes here as ecx: 1
	mov ecx, 0
	jecxz label5
	push 0xFF ; should not come here with ecx: 0
label5:
	push 0x2 ; comes here from jecxz: 2
	jmp 0
