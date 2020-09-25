.text

.global count_det
count_det:
	push {r4-r12}

	ldrb r4, [r0], #4
	ldrb r5, [r0], #4
	ldrb r6, [r0], #4
	ldrb r7, [r0], #4
	ldrb r8, [r0], #4
	ldrb r9, [r0], #4
	ldrb r10, [r0], #4
	ldrb r11, [r0], #4
	ldrb r12, [r0], #4

	mov r0, #0
	mov r1, #1
	
	mul r1, r4
	mul r1, r8
	mul r1, r12

	add r0, r1

	mov r1, #1

	mul r1, r5
	mul r1, r9
	mul r1, r10

	add r0, r1

	mov r1, #1 
	
	mul r1, r6
	mul r1, r7
	mul r1, r11

	add r0, r1

	mov r1, #1 

	mul r1, r6
	mul r1, r8
	mul r1, r10

	sub r0, r1

	mov r1, #1

	mul r1, r4
	mul r1, r9
	mul r1, r11

	sub r0, r1

	mov r1, #1 

	mul r1, r5
	mul r1, r7
	mul r1, r12

	sub r0, r1
	
	pop {r4-r12}

	bx lr

