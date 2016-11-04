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

	machine->WriteRegister(PCReg, f);
	machine->WriteRegister(4, arg);
	machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 4);

	DEBUG ('x', "Initializing register to %d %d\n", arg, machine->ReadRegister(PCReg) + 4);

	machine->WriteRegister(StackReg, currentThread->space->AllocateUserStack());
    DEBUG ('x', "Initializing stack register to %d\n", currentThread->space->AllocateUserStack());

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
	DEBUG('x', "Valeures des paremetres %d %d\n", params->f, params->arg);

	// On place le thread dans la file d'attente
	newThread->Start(StartUserThread, (void*)params);

	return 0;
}

// Detruit le thread actif
void do_ThreadExit()
{
	currentThread->Finish();
}
#endif // CHANGED