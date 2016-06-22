/* Name : head.S
 *  Startup code
 */
/*
 * CHANGELOG:
   * 23-07-2003	created by ARCA TECH
   * 26-08-2005 copied from Zi1221
  
*-------------------------------------------------------------------------*/

#include<config.h>
	.little
	.section	.sys_text.head ,"ax",@progbits
	.align
/*
 * Entry point.
 */

/*if boot from 0x00000000 , add these definitions; else if boot from 0x00008000 ,please mask here*/
/*
	.global __entry
__entry:
	.long __entry0
*/
__entry0:
/* 	set vector table  */
	mova	r1, 0x0c000000
	mova      r2, __entry0
	sd32     r2, [r1, 0]
	mova      r2, __entry1
	sd32     r2, [r1, 4]
	mova      r2, __entry2
	sd32     r2, [r1, 8]
	mova      r2, __entry3
	sd32     r2, [r1, 12]
	mova      r2, __entry4
	sd32     r2, [r1, 16]
	mova      r2, __entry5
	sd32     r2, [r1, 20]
	mova      r2, __entry6
	sd32     r2, [r1, 24]
	mova      r2, __entry7
	sd32     r2, [r1, 28]

		
/* set SR */
REG_SR	= 0
/*  SR.SM = 1 (p mode).   */
/*  SR.DS = 0 */
/*  SR.DE = 1 (enable debug) */
/*  SR.IE = 1 (enable interrupt) */
	
/*  SR.VB = 100 (sram). (bit6~4) */
IPR=0x1803F010
IMR=0x1803F004
	addi 	r8, r0, 0x3B
/*mask all interrupts*/
   	mova	r1, IMR
	mova	r2, 0xFFFFFFFF
	sd32	r2, r1,0
	wcr	r8, REG_SR
/*init stack top in RAM ,plz read link script*/	
	mova      r1, _stack
/*jump to hardware init function*/	
	mova      r8, init
	ja	r18,r8,0
	
	.little
	.section	.sys_text ,"ax",@progbits
	.macro SAVE_ALL
	addi	r1,r1,-136
	
	sd32	r31,[r1,132]	 
	sd32	r30,[r1,128]
	 	 
	sd32	r29,[r1,124] 	 
	sd32	r28,[r1,120] 	 
	sd32	r27,[r1,116] 	 
	sd32	r26,[r1,112] 	 
	sd32	r25,[r1,108] 	 
	sd32	r24,[r1,104] 	 
	sd32	r23,[r1,100]	 
	sd32	r22,[r1,96] 	 
	sd32	r21,[r1,92] 	 
	sd32	r20,[r1,88]
	
	sd32	r19,[r1,84] 	 
	sd32	r18,[r1,80]
	sd32	r17,[r1,76]
	sd32	r16,[r1,72]
	sd32	r15,[r1,68] 	 
	sd32	r14,[r1,64] 	 
	sd32	r13,[r1,60] 	 
	sd32	r12,[r1,56] 	 
	sd32	r11,[r1,52] 	 
	sd32	r10,[r1,48]
	
	sd32	r9 ,[r1,44]  	 
	sd32	r8 ,[r1,40] 	 
	sd32	r7 ,[r1,36] 	 
	sd32	r6 ,[r1,32] 	 
	sd32	r5 ,[r1,28] 	 
	sd32	r4 ,[r1,24] 	 
	sd32	r3 ,[r1,20] 	 
	sd32	r2 ,[r1,16] 	 
	sd32	r1 ,[r1,12]
	sd32	r0 ,[r1,8]
	rcr	r2, 2		!spc
	sd32	r2,[r1,4]
	rcr	r2, 1		!ssr
	sd32	r2,[r1,0]	 
	.endm	

	
	.macro RESTORE_ALL
	ld32	r31,[r1,132]	 
	ld32	r30,[r1,128]
	 	 
	ld32	r29,[r1,124] 	 
	ld32	r28,[r1,120] 	 
	ld32	r27,[r1,116] 	 
	ld32	r26,[r1,112] 	 
	ld32	r25,[r1,108] 	 
	ld32	r24,[r1,104] 	 
	ld32	r23,[r1,100]	 
	ld32	r22,[r1,96] 	 
	ld32	r21,[r1,92] 	 
	ld32	r20,[r1,88]
	
	ld32	r19,[r1,84] 	 
	ld32	r18,[r1,80]
	ld32	r17,[r1,76]
	ld32	r16,[r1,72]
	ld32	r15,[r1,68] 	 
	ld32	r14,[r1,64] 	 
	ld32	r13,[r1,60] 	 
	ld32	r12,[r1,56] 	 
	ld32	r11,[r1,52] 	 
	ld32	r10,[r1,48]
	
	ld32	r9 ,[r1,44]  	 
	ld32	r8 ,[r1,40] 	 
	ld32	r7 ,[r1,36] 	 
	ld32	r6 ,[r1,32] 	 
	ld32	r5 ,[r1,28] 	 
	ld32	r4 ,[r1,24] 	 
	ld32	r3 ,[r1,20] 	 
	ld32	r2 ,[r1,4]	!spc
	wcr	r2, 2
	ld32	r2 ,[r1,0]	!ssr
	wcr	r2, 1		 
	ld32	r2 ,[r1,16] 	 
	addi	r1,r1,136
	.endm	

	.align 2
	.globl VBR
	.globl __entry1
	.globl __entry2
	.globl __entry3
	.globl __entry4
	.globl __entry5
	.globl __entry6
	.globl __entry7
	.align 8
VBR:	
		
__entry1:
	SAVE_ALL
	add	r2,r1,r0
	mova	r4,do_entry1
	ja	r18,r4,0
	and     r0,r0,r0
	RESTORE_ALL
	rte
	and     r0,r0,r0
__entry2:
	SAVE_ALL
	add	r2,r1,r0
	mova	r4,do_entry2
	ja	r18,r4,0
	and     r0,r0,r0
	RESTORE_ALL
	rte
	and     r0,r0,r0

__entry3:
	SAVE_ALL
	add	r2,r1,r0
	mova	r4,do_entry3
	ja	r18,r4,0
	and     r0,r0,r0
	RESTORE_ALL
	rte
	and     r0,r0,r0
	
__entry4:
	SAVE_ALL
	add	r2,r1,r0
	mova	r4,do_entry4
	ja	r18,r4,0
	and     r0,r0,r0
	RESTORE_ALL
	rte
	and     r0,r0,r0
	
__entry5:
	SAVE_ALL
	add	r2,r1,r0
	mova	r4,do_entry5
	ja	r18,r4,0
	and     r0,r0,r0
	RESTORE_ALL
	rte
	and     r0,r0,r0

__entry6:
	SAVE_ALL
	add	r2,r1,r0
	mova	r4,do_entry6
	ja	r18,r4,0
	and     r0,r0,r0
	RESTORE_ALL
	rte
	and     r0,r0,r0

__entry7:
	SAVE_ALL
	add	r2,r1,r0
	mova	r4,do_entry7
	ja	r18,r4,0
	and     r0,r0,r0
	RESTORE_ALL
	rte
	and     r0,r0,r0


	.section	.text ,"ax",@progbits
	.align 2
	.global	__umodsi3
	.type	__umodsi3,@function
__umodsi3:
	beq	r3,r0,.L42
	bltu	r2,r3,.L42
	sari	r30,r2,31
	addi	r21,r0,31
	bne	r30,r0,.L44
	csb	r6,r3
	sub	r6,r21,r6
.L40:
	csb	r7,r2
.L48:	sub	r7,r21,r7
	sub	r7,r7,r6
	sll	r20,r3,r7
	sltu	r28,r2,r20
	sub	r7,r7,r28
	sll	r20,r3,r7
	sub	r2,r2,r20
	sari	r30,r2,31
	bne	r30,r0,.L46
	bgeu	r2,r3,.L40

.L42:
	ja	r0,r18,0

.L44:
	sari	r30,r3,31
	sub	r31,r2,r3
	mvnz	r2,r30,r31
	bne	r30,r0,.L42
	csb	r6,r3
	sub	r6,r21,r6
	
.L46:	addi	r7,r0,0
	beq	r7,r0,.L48
.Lfe2:
	.size	__umodsi3,.Lfe2-__umodsi3
	.align 2
	.global	__udivsi3
	.type	__udivsi3,@function
__udivsi3:
	
	add	r5,r2,r0
	add	r2,r0,r0
	beq	r3,r0,.L32
	addi	r4,r0,1
	bltu	r5,r3,.L32
	addi	r21,r0,31
	sari	r30,r5,31
	bne	r30,r0,.L34
	csb	r6,r3
	sub	r6,r21,r6
	
.L30:
	csb	r7,r5
.L38:	sub	r7,r21,r7
	sub	r7,r7,r6                                                     
	sll	r28,r3,r7
	sltu	r29,r5,r28
	sub	r7,r7,r29
	sll	r28,r3,r7
	sub	r5,r5,r28
	sll	r29,r4,r7
	add	r2,r2,r29
	sari	r30,r5,31
	bne	r30,r0,.L36
	bgeu	r5,r3,.L30
.L32:
	ja	r0,r18,0
.L34:
	sari	r30,r3,31
	addi	r31,r0,1
	mvnz	r2,r30,r31
	bne	r30,r0,.L32
	csb	r6,r3
	sub	r6,r21,r6
	
.L36:	addi	r7,r0,0
	beq	r7,r0,.L38	
.Lfe3:
	.size	__udivsi3,.Lfe3-__udivsi3
	.align 2
	.global	__sdivsi3
	.type	__sdivsi3,@function
__sdivsi3:	
	sge	r20,r2,r0
	sub	r5,r0,r2
	mvnz	r5,r20,r2
	sge	r4,r3,r0
	sub	r6,r0,r3
	mvz	r3,r4,r6
	xor	r21,r20,r4

	bltu	r5,r3,.L_R0
	slli	r28,r3,1
	slli	r6,r21,1
	bltu	r5,r28,.L_R1
	add	r2,r0,r0
	beq	r3,r0,.L12
	addi	r4,r0,1
	addi	r29, r0, 31;
	csb	r6,r3
	sub	r6,r29,r6
.L10:
	csb	r7,r5
	sub	r7,r29,r7
	sub	r7,r7,r6
	sll	r20,r3,r7
	sltu	r28,r5,r20
	sub	r7,r7,r28
	sll	r20,r3,r7
	sub	r5,r5,r20
	sll	r28,r4,r7
	add	r2,r2,r28
	bgeu	r5,r3,.L10	!!consider 0x80000000 / 0x80000000
.L12:
        sub     r3,r0,r2
	mvnz	r2,r21,r3
	ja	r0,r18, 0
.L_R0:
	addi	r2,r0,0
	ja	r0,r18,0
.L_R1:
	addi	r2,r0,1
	sub	r2,r2,r6	
	ja	r0,r18,0
.Lfe4:
	.size	__sdivsi3,.Lfe4-__sdivsi3
	.align 2
	.global	__smodsi3
	.type	__smodsi3,@function
__smodsi3:
	sge	r4,r3,r0
	sub	r5,r0,r3
	mvz	r3,r4,r5
	sge	r6,r2,r0
	sub	r7,r0,r2
	mvz	r2,r6,r7
	addi	r21,r0,31
	
	beq	r3,r0,.L22
	bltu	r2,r3,.L22
	csb	r5,r3
	sub	r5,r21,r5
.L20:	
	csb	r7,r2
	sub	r7,r21,r7
	sub	r7,r7,r5
	sll	r20,r3,r7
	sltu	r28,r2,r20
	sub	r7,r7,r28
	sll	r20,r3,r7
	sub	r2,r2,r20
	bgeu	r2,r3,.L20	
.L22:	
        sub     r3,r0,r2
	mvz	r2,r6,r3
	ja	r0,r18,0

/*get current stack top */

	.section	.text ,"ax",@progbits
	.align 2
	.global	_getstack
	.type	_getstack,@function
_getstack:
    mova    r2,0
	addi    r2,r1,0 
	ja	r0,r18,0
	
	.section	.text ,"ax",@progbits
	.align 2
	.global	_jumptoentry
	.type	_jumptoentry,@function
_jumptoentry:
    mova    r2,0x00008000
	ja	r0,r2,0	

