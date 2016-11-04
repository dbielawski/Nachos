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

int main()
{
	ThreadCreate(affiche, 65); // 65 <=> A ASCII
	fait_rien(1000);

	Exit(0);
}
