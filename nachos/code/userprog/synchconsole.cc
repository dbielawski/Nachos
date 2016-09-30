#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }


SynchConsole::SynchConsole(const char *in, const char *out)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(int ch)
{
	console->PutChar(ch);
	writeDone->P();
}

int SynchConsole::SynchGetChar()
{
	readAvail->P();
	char c = console->GetChar();
	return c;
}

void SynchConsole::SynchPutString(const char s[])
{
	const char* string = s;

	while (*string)
	{
		SynchPutChar(*string);
		string++;
	}
}

void SynchConsole::SynchGetString(char *s, int n)
{
	char c;
	int i = 0;

	while (i < n - 1)
	{
		c = SynchGetChar();

		if (c == EOF || c == '\n')
		{
			break;
		}
		else
			s[i] = c;
		++i;
	}

	s[i] = '\0';
}

#endif // CHANGED
