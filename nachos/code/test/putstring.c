#include "syscall.h"

int main()
{
	
	// const char* string = "Hello World!!!\n";
	const char* string = "HEllo world\n";

	PutString(string);
	PutString("\n");
	PutString(string);

	Halt();
}