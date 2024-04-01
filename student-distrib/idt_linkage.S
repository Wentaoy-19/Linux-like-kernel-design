# idt_linkage.S - connect IDT to handlers

#define ASM    1

.text

# extern variables
.global exception_00, exception_01, exception_02, exception_03, exception_04, exception_05
.global exception_06, exception_07, exception_08, exception_09, exception_0a, exception_0b
.global exception_0c, exception_0d, exception_0e, exception_0f, exception_10, exception_11
.global exception_12, exception_13, exception_14, exception_15, exception_16, exception_17
.global exception_18, exception_19, exception_1a, exception_1b, exception_1c, exception_1d
.global exception_1e, exception_1f
.global pic_20, pic_21, pic_22, pic_23, pic_24, pic_25, pic_26, pic_27, pic_28, pic_29
.global pic_2a, pic_2b, pic_2c, pic_2d, pic_2e, pic_2f, idt_80
.global idt_else

# exception linkage
exception_00:
     pushal
     call ex00
     popal
     iret
# exception linkage
exception_01:
     pushal
     call ex01
     popal
     iret
# exception linkage
exception_02:
     pushal
     call ex02
     popal
     iret
# exception linkage
exception_03:
     pushal
     call ex03
     popal
     iret
# exception linkage
exception_04:
     pushal
     call ex04
     popal
     iret
# exception linkage
exception_05:
     pushal
     call ex05
     popal
     iret
# exception linkage
exception_06:
     pushal
     call ex06
     popal
     iret
# exception linkage
exception_07:
     pushal
     call ex07
     popal
     iret
# exception linkage
exception_08:
     pushal
     call ex08
     popal
     iret
# exception linkage
exception_09:
     pushal
     call ex09
     popal
     iret
# exception linkage
exception_0a:
     pushal
     call ex0a
     popal
     iret
# exception linkage
exception_0b:
     pushal
     call ex0b
     popal
     iret
# exception linkage
exception_0c:
     pushal
     call ex0c
     popal
     iret
# exception linkage
exception_0d:
     pushal
     call ex0d
     popal
     iret
# exception linkage
exception_0e:
     pushal
     call ex0e
     popal
     iret
# exception linkage
exception_0f:
     pushal
     call ex0f
     popal
     iret
# exception linkage
exception_10:
     pushal
     call ex10
     popal
     iret
# exception linkage
exception_11:
     pushal
     call ex11
     popal
     iret
# exception linkage
exception_12:
     pushal
     call ex12
     popal
     iret
# exception linkage
exception_13:
     pushal
     call ex13
     popal
     iret
# exception linkage
exception_14:
     pushal
     call ex14
     popal
     iret
# exception linkage
exception_15:
     pushal
     call ex15
     popal
     iret
# exception linkage
exception_16:
     pushal
     call ex16
     popal
     iret
# exception linkage
exception_17:
     pushal
     call ex17
     popal
     iret
# exception linkage
exception_18:
     pushal
     call ex18
     popal
     iret
# exception linkage
exception_19:
     pushal
     call ex19
     popal
     iret
# exception linkage
exception_1a:
     pushal
     call ex1a
     popal
     iret
# exception linkage
exception_1b:
     pushal
     call ex1b
     popal
     iret
# exception linkage
exception_1c:
     pushal
     call ex1c
     popal
     iret
# exception linkage
exception_1d:
     pushal
     call ex1d
     popal
     iret
# exception linkage
exception_1e:
     pushal
     call ex1e
     popal
     iret
# exception linkage
exception_1f:
     pushal
     call ex1f
     popal
     iret
# interrupt linkage
pic_20:
     pushal
     call pic20
     popal
     iret
# interrupt linkage
pic_21:
     pushal
     call pic21
     popal
     iret
# interrupt linkage
pic_22:
     pushal
     call pic22
     popal
     iret
# interrupt linkage
pic_23:
     pushal
     call pic23
     popal
     iret
# interrupt linkage
pic_24:
     pushal
     call pic24
     popal
     iret
# interrupt linkage
pic_25:
     pushal
     call pic25
     popal
     iret
# interrupt linkage
pic_26:
     pushal
     call pic26
     popal
     iret
# interrupt linkage
pic_27:
     pushal
     call pic27
     popal
     iret
# interrupt linkage
pic_28:
     pushal
     call pic28
     popal
     iret
# interrupt linkage
pic_29:
     pushal
     call pic29
     popal
     iret
# interrupt linkage
pic_2a:
     pushal
     call pic2a
     popal
     iret
# interrupt linkage
pic_2b:
     pushal
     call pic2b
     popal
     iret
# interrupt linkage
pic_2c:
     pushal
     call pic2c
     popal
     iret
# interrupt linkage
pic_2d:
     pushal
     call pic2d
     popal
     iret
# interrupt linkage
pic_2e:
     pushal
     call pic2e
     popal
     iret
# interrupt linkage
pic_2f:
     pushal
     call pic2f
     popal
     iret

# linkage for other interrupts
idt_else:
     pushal
     call idtelse
     popal
     iret

# system call linkage

# syscall_table:
#	.long 0x0, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn

# assemble linkage for system call handler
idt_80:
	pushfl

	pushl %ebp
	pushl %edi
	pushl %esi

	pushl %edx		# push arguments
	pushl %ecx
	pushl %ebx

     cmpl $1, %eax	# check eax
	jl invalid

     cmpl $10, %eax
     jg invalid 
	
     # call *syscall_table(, %eax, 4)
	jmp return_to_user_space

invalid:
 	movl $-1, %eax	#return -1

return_to_user_space:
	popl %ebx
	popl %ecx
	popl %edx
	popl %esi
	popl %edi
	popl %ebp
	popfl

	iret
