#include "syscall.h"

void affiche_a(char c)
{
	int i;
	for (i = 0; i < 10; ++i)
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
	ThreadCreate(affiche_a, 'a');
	PutString("Hello world\n");
	fait_rien(10000);
	ThreadExit();
}