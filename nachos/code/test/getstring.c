#include "syscall.h"

int main()
{
	
	int size = 10;
	char string[size];
	GetString(string, size);
	PutString(string);
	PutString("Fin du programme\n");
	return 0;
}