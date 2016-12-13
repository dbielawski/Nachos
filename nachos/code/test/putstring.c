#include "syscall.h"

int main()
{
	
	const char* string = "Hello World!!!\n";

	PutString(string);
	PutString("\n");
	while(1);
		// Exit(0);
}