#include "syscall.h"

int main()
{
	
	// const char* string = "Hello World!!!\n";
	const char* string = "A2";

	PutString(string);
	PutString(string);
	PutString(string);
	PutString(string);
	PutString("\n");

	return 0;
}