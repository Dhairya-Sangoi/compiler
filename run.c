#include <stdio.h>
#include <stdlib.h>
int main(){
	//system("nasm -f elf64 -l code.lst code.asm");
	//system("gcc -o code code.o);
	//system("./code");
	
	
	system("nasm -f elf code.asm");
	system("gcc -m32 code.o -o code");
	system("./code");
	return 0;
}
