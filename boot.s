.set ALIGN,    1<<0		# align loaded modules on page boundaries
.set MEMINFO,  1<<1		# provide memory map
.set FLAGS,    ALIGN | MEMINFO	# this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS)	# checksum of above, to prove we are multiboot

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .data
.align 4
.global _multiboot_info
.type _multiboot_info, @object
_multiboot_info:
.long 0

.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
	mov %ebx, (_multiboot_info)
	call kernel_main
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
