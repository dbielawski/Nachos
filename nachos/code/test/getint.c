#include "syscall.h"

int main()
{
	int n;
	GetInt(&n);
	PutInt(n);

	PutChar('\n');
	GetInt(&n);
	PutInt(n);
	PutChar('\n');
	Exit(0);
}