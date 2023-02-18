		AREA |.text|, CODE, READONLY, ALIGN=2 
		THUMB
		PRESERVE8
		EXTERN currentPt
		EXPORT SysTick_Handler
		EXPORT scheduler_launch
		IMPORT kernel_scheduler
	
		
		
	
SysTick_Handler      ;save r0,r1,r2,r3,r12,lr,pc,psr
	ISB	
	CPSID 	I
	PUSH    {R4-R11}	;saves r4,r5,r6,r7,r8,r9,r10,r11
	LDR     R0,=currentPt		;ro points to current pointer
	LDR		R1,[R0]				;R1 = currentPt
	STR 	SP,[R1]				
	PUSH 	{R0,LR}
	BL	 	kernel_scheduler
	POP 	{R0,LR}
	LDR		R1,[R0]		;R1= current pointer 
	LDR 	SP,[R1]		; SP = currentPt->stackPt 
	POP 	{R4-R11}
	CPSIE 	I
	BX 		LR
	
scheduler_launch				; for first time loading
	LDR 	R0,=currentPt
	LDR 	R2,[R0]			;   R2 = currentPt 
	LDR 	SP,[R2]			;	sp= currentPt->stackpt
	POP	  	{R4-R11}
	POP 	{R0-R3}
	POP	    {R12}
	ADD 	SP,SP,#4       ; jump stack pointer
	POP 	{LR}
	ADD	 	SP,SP,#4        ;  jump program counter
	CPSIE 	I
	BX  	LR
	
ALIGN 
END