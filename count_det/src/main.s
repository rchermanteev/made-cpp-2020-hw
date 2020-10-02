.text

.global count_det
count_det:
	push {r4-r10}

	ldr r2, [r0], #4
	ldr r3, [r0], #4
	ldr r4, [r0], #4
	ldr r5, [r0], #4
	ldr r6, [r0], #4
	ldr r7, [r0], #4
	ldr r8, [r0], #4
	ldr r9, [r0], #4
	ldr r10, [r0], #4

	mov r0, #0
	mov r1, #1
	
	mul r1, r2
	mul r1, r6
	mul r1, r10

	add r0, r1

	mov r1, #1

	mul r1, r3
	mul r1, r7
	mul r1, r8

	add r0, r1

	mov r1, #1 

	mul r1, r5
	mul r1, r9
	mul r1, r4

	add r0, r1

	mov r1, #1 

	mul r1, r4
	mul r1, r6
	mul r1, r8

	sub r0, r1

	mov r1, #1

	mul r1, r9
	mul r1, r7
	mul r1, r2

	sub r0, r1

	mov r1, #1 

	mul r1, r5
	mul r1, r3
	mul r1, r10

	sub r0, r1

	pop {r4-r10}

	bx lr

