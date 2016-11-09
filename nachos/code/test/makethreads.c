#include "syscall.h"

void fait_rien(int rien)
{
	int i;
	for (i = 0; i < rien; ++i);
}

void affiche(char c)
{
	PutChar(c);
	ThreadExit();
}

void affiche_a(char c)
{
	volatile int i;
	for (i = 0; i < 1; ++i)
	{
		PutChar(c);
	}
	ThreadExit();
}

int main()
{
	// ThreadCreate(affiche, 'A');
	// PutChar('B');
	// PutChar('C');
	ThreadCreate(affiche_a, 'a');
	ThreadCreate(affiche_a, 'b');
	ThreadCreate(affiche_a, 'c');
	ThreadCreate(affiche_a, 'd');
	ThreadCreate(affiche_a, 'e');

	fait_rien(100);
	ThreadExit();
}