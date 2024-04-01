# /*reference: https://wiki.osdev.org/Setting_Up_Paging*/
# /*loadPageDirectory*/

# .globl loadPageDirectory
# loadPageDirectory:
# push %ebp
# mov %esp, %ebp
# mov 8(%esp), %eax
# mov %eax, %cr3
# mov %ebp, %esp
# pop %ebp
# ret


# /*enablePaging*/
# .globl enablePaging
# enablePaging:
# push %ebp
# mov %esp, %ebp
# mov %cr0, %eax
# or $0x80000000, %eax
# mov %eax, %cr0
# mov %ebp, %esp
# pop %ebp
# ret

# /*enable_4mb_Paging*/
# .globl enable_4mb_paging
# enable_4mb_Paging:
# push %ebp
# mov %esp, %ebp
# mov %cr4, %eax
# or $0x00000010, %eax
# mov %eax, %cr0
# mov %ebp, %es4
# pop %ebp
# ret
