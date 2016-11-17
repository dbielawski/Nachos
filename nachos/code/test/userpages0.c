#include "syscall.h"



int main()
{
	PutChar('a');
	PutChar('b');
	PutChar('c');
	PutChar('d');
	PutChar('\n');

	PutString("Hello world\n");
	Exit(0);
}