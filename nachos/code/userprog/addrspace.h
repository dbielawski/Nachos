// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "translate.h"

#define UserStacksAreaSize    1024  // increase this as necessary!


#ifdef CHANGED
#include "bitmap.h"
#define ThreadSize      256     // Espace entre chaque thread
#endif // CHANGED

#ifdef CHANGED
class Semaphore;
#endif // CHANGED

class AddrSpace:dontcopythis
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 

#ifdef CHANGED
    int AllocateUserStack();    // Retourne un adresse valide pour un nouveau thread
    int GetNbThread();          // Retourne le nombre de thread
    void AddThread();           // Incremente le nb de thread
    void RemoveThread();        // Decremente le nb de thread
    int GetNewId();             // Va chercher dans la bitmap un emplacement libre
    void RemoveId();            // Va supprimer le thread courant de la bitmap
#endif // CHANGED

  private:
      TranslationEntry * pageTable; // Assume linear page table translation
    // for now!
    unsigned int numPages;  // Number of pages in the virtual 
    // address space


#ifdef CHANGED
    int nbThread;                       // nombre de thread cree dans l'esapce d'adressage
    Semaphore* semaphoreNbThread;       // Modifer nbThread en exclusion mutuelle
    Semaphore* semaphoreClearBM;
    Semaphore* semaphoreGetId;
    Semaphore* semaphoreAttendAutresThreads;    // Des qu il n y a plus d emplacement
    // dans la bitmap, on bloque, quand un thread termine, on deblque
    
    BitMap* bitmap;

    void ClearAllPages();
#endif // CHANGED

};

#endif // ADDRSPACE_H
