#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;


static Semaphore *semaphoreSynchGetString;


static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }


SynchConsole::SynchConsole(const char *in, const char *out)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, NULL);

	semaphoreSynchGetString = new Semaphore("get string done", 1);
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
    int i = 0;

	while (s[i] && i < MAX_STRING_SIZE)
	{
		SynchPutChar(s[i]);
		++i;
	}
}

void SynchConsole::SynchGetString(char *s, int n)
{
	semaphoreSynchGetString->P();

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

	semaphoreSynchGetString->V();
}

void SynchConsole::SynchPutInt(int n)
{
	char buffer[MAX_STRING_SIZE];
	snprintf(buffer, MAX_STRING_SIZE, "%d", n);
	SynchPutString(buffer);
}

void SynchConsole::SynchGetInt(int *n)
{
	char buffer[MAX_STRING_SIZE];
	SynchGetString(buffer, MAX_STRING_SIZE);
	sscanf(buffer, "%d", n);
}

#endif // CHANGED
