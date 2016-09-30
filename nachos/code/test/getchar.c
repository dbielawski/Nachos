#include "syscall.h"


int main()
{
	char c = GetChar();
	PutChar('\n');
	PutChar(c);
	PutChar('\n');
	return 0;
}