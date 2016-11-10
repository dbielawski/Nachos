#ifdef CHANGED

#include "userthread.h"
#include "thread.h"

#include "system.h"


// Structure de donnees, ce sont les parametres du thread utilisateur
struct userThreadParams
{
	int f;
	int arg;
};

// Initialise tous les registres lorsque le thread est active par l'ordonnanceur
static void StartUserThread(void *schmurtz)
{
	int f = ((int)((userThreadParams*)schmurtz)->f);
	int arg = ((int)((userThreadParams*)schmurtz)->arg);

	machine->WriteRegister(PCReg, f);		// Adresse sur le debut de la fonction
	machine->WriteRegister(4, arg);			// Donne le parametre de la fonction
	// Instruction suivante
	machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);

	// DEBUG ('x', "Initializing register to PCReg %d arg %d NextPCReg %d\n",
		// f, arg, machine->ReadRegister(PCReg) + 4);

	int addr = currentThread->space->AllocateUserStack();

	if (addr < 0)		// Une mauvaise adresse est renvoyée (depassement de la zone d'adressage)
	{
		printf("Erreur lors de l'allocation de la pile\n");
		ASSERT(FALSE);
	}

	// Mise a jour du pointeur de pile
	machine->WriteRegister(StackReg, addr);
    DEBUG ('x', "Initializing stack register to %d\n", addr);

	// Lance le programme utilisateur
	machine->Run(); 
}

// Cree un nouveau thread, l'initialise et le place dans la file d'attente
// f: fonction a executer
// arg: parametre de la fonction a executer
// retourne -1 si la creation a echouee, 0 sinon
int do_CreateThread(int f, int arg)
{
	Thread* newThread = new Thread("newThread");

	// Si la creation a echouee
	if (!newThread)
		return -1;

	userThreadParams* params = new userThreadParams;
	params->f = f;
	params->arg = arg;

	DEBUG('x', "Nouveau thread cree\n");
	//DEBUG('x', "Adresse fonction %d valeur parametre %d\n", f, arg);

	currentThread->space->AddThread();

	// On place le thread dans la file d'attente
	newThread->Start(StartUserThread, (void*)params);

	return 0;
}

// Detruit le thread actif
void do_ThreadExit()
{
	DEBUG('x', "Terminaison du thread: %s id: %d\n", currentThread->getName(), currentThread->id);
	currentThread->space->RemoveThread();
	currentThread->space->RemoveId();


	// Si c est le dernier thread qui termine, alors on quitte NachOS
	if (currentThread->space->GetNbThread() == 0)
	{
		DEBUG('x', "Appel: interrupt->Halt()\n");
		interrupt->Halt();
	}

	currentThread->Finish();
}
#endif // CHANGED