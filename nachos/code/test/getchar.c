#include "syscall.h"


int main()
{
	char c = GetChar();
	PutChar(c);
	PutChar('\n');
	return 0;
}