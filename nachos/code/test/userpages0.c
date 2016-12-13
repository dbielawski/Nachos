#include "syscall.h"

void affiche(char c)
{
	int i;
	for (i = 0; i < 3; ++i)
		PutChar(c);
	PutChar('\n');
	ThreadExit();
}

void fait_rien(int rien)
{
	int i;
	for (i = 0; i < rien; ++i);
}

int main()
{
	// ThreadCreate(affiche, 'a');
	// ThreadCreate(affiche, 'B');
	// ThreadCreate(affiche, 'c');
	// PutString("Hello world\n");


	// ForkExec("test/putchar");
	// ForkExec("test/putint");

	ForkExec("test/userpages1");
	ForkExec("test/userpages2");
}