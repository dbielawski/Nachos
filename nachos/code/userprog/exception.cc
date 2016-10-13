// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED
#include "synchconsole.h"
#include "userthread.h"
#endif // end CAHANGED

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);

    if (MAX_STRING_SIZE < 2)
    {
    	printf ("La taille du buffer doit etre superieure a 1\n");
    	ASSERT(FALSE);
    }

    switch (which)
      {
	case SyscallException:
          {
	    switch (type)
	      {
		case SC_Halt:
		  {
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
		    interrupt->Halt ();
		    break;
		  }
#ifdef CHANGED
		  case SC_Exit:
		  {
		  	DEBUG ('s', "Exit, initiated by user program.\n");
		  	// On recupere l'aguement dans le registre 4
		  	int code = machine->ReadRegister(4);
	  		printf("Le programme s'est terminé avec le code de retour: %i\n", code);
		  	interrupt->Halt();
		  	break;
		  }

		  case SC_PutChar:
		  {
		    DEBUG ('s', "PutChar, initiated by user program.\n");
		    int c = machine->ReadRegister (4);
		    synchconsole->SynchPutChar(c);
		    break;
		  }
		  case SC_PutString:
		  {
		    DEBUG ('s', "PutString, initiated by user program.\n");

		    if (MAX_STRING_SIZE < 2)
		    {
		    	printf("PutString, le taille du tampon doit etre superieure a 1 !");
		    	ASSERT(FALSE);
		    }

		    char* buffer = (char*)malloc(MAX_STRING_SIZE * sizeof(char));

		    int c = machine->ReadRegister (4);

		    int nbCharCopie = 0;
		    int i = 0;

		    do
		    {
			    nbCharCopie = copyStringFromMachine(i * MAX_STRING_SIZE + c, buffer, MAX_STRING_SIZE);
			    synchconsole->SynchPutString(buffer);
			    ++i;
		    } while (nbCharCopie == MAX_STRING_SIZE);

		    free(buffer);
		    break;
		  }
		  case SC_GetChar:
		  {
		    DEBUG ('s', "GetChar, initiated by user program.\n");
		    int c = synchconsole->SynchGetChar();
		    machine->WriteRegister(2, c);
		    break;
		  }
		  case SC_GetString:
		  {
		    DEBUG ('s', "GetString, initiated by user program.\n");

		    int s = machine->ReadRegister(4);
		    int taille = machine->ReadRegister(5);

		    char* buffer = (char*)malloc(sizeof(char) * MAX_STRING_SIZE);

		    synchconsole->SynchGetString(buffer, taille);
		    int nbCharCopie = copyStringToMachine(buffer, s, taille);

		    free(buffer);
		    break;
		  }

		  case SC_PutInt:
		  {
		  	DEBUG ('s', "PutInt, initiated by user program.\n");

		  	int entier = machine->ReadRegister(4);
		  	synchconsole->SynchPutInt(entier);
		  	break;
		  }
		  case SC_GetInt:
		  {
		  	DEBUG ('s', "GetInt, initiated by user program.\n");

		  	int n;
		  	synchconsole->SynchGetInt(&n);
		  	int s = machine->ReadRegister(4);
		  	machine->WriteMem(s, sizeof(int), n);
		  	break;
		  }
#endif // end CHANGED

		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE);
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }

	case PageFaultException:
	  if (!type) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", type, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }

	default:
	  printf ("Unexpected user mode exception %d %d at PC %x\n", which, type, machine->registers[PCReg]);
	  ASSERT (FALSE);
      }
}



#ifdef CHANGED
// Copie une chaine du mode user vers un tampon dans lespace kernel.
// from : adresse de lespace user
// to : buffer dans lespace kernel
// size : taille du buffer du mode kernel
// return le nombre delements reelement copie dans to
int copyStringFromMachine(int from, char* to, unsigned size)
{
	if (!to || size < 2)
		return 0;

   else
    {
		int v;
	    unsigned i = 0;

	    // Tant que nous ne sommes pas en fin de chaine et que nous n'avons pas
	    // ecris plus que la taille limite du buffer, size,
	    // ecrire dans le buffer du kernel
	    do
	    {
	    	if (machine->ReadMem(from + i, sizeof(char), &v))
		        to[i] = v;
	        ++i;

	    } while (v != '\0' && i < size);

	    to[i] = '\0';
    	return i;
    }
}

// Copie une chaine du mode kernel dans un tampon du mode user
// s: la chaine a copier
// to: pointeur sur adresse (mode user)
// size: taille du tampon
int copyStringToMachine(char* s, int to, unsigned size)
{
	// Traite les cas non valides (pointeur null, buffer trop petit)
	if (!to || size < 2)
		return 0;

	else
	{
		// Compteur du nombre de caracteres copies
		unsigned i = 0;

		// Copie les caracteres dans to
		for (i = 0; i < size && s[i]; ++i)
		{
			machine->WriteMem(to, sizeof(char), s[i]);
			++to;
		}


		// On s'assure que to fini par un retour a la ligne
		// pour ne pas que les caracteres tapes soient concatenes
		machine->WriteMem(to, sizeof(char), '\n');

		return i;		
	}
}

int do_CreateThread(int f, int arg)
{
	
}
#endif // end CHAGNED