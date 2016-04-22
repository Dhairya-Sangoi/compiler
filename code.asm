	SECTION .data
	SECTION .bss
programdata	resd	16
	SECTION .text
			global main
main:
	mov dword [programdata + 12], 7
l3:
	mov EBX, 0
	mov EAX, 1
	int 0x80
