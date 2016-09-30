#include "syscall.h"

int main()
{
	
	const char* string = "Hello World!!!\n";

	PutString(string);

	Halt();
}