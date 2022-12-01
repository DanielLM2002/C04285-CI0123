// addrspace.h 
//	Data structures to keep track of executing user programs 
//	(address spaces).
//
//	For now, we don't keep any information about address spaces.
//	The user level CPU state is saved and restored in the thread
//	executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "bitmap.h"
#include "filesys.h"
#include "noff.h"
//#include "system.h"


#define UserStackSize		1024 	// increase this as necessary!

class AddrSpace {
  public:
    TranslationEntry *pageTable;	// Assume linear page table translation

    AddrSpace(AddrSpace* parentSpace);	// Create an address space,
          // initializing it with the program
          // stored in the file "executable"
    AddrSpace(OpenFile *executable, const char * filename = "");	// Create an address space,
					// initializing it with the program
					// stored in the file "executable"
    ~AddrSpace();			// De-allocate an address space

    void InitRegisters();		// Initialize user-level CPU registers,
					// before jumping to user code

    void SaveState();			// Save/restore address space-specific
    void RestoreState();		// info on a context switch 
    void moveMemory(int vpn);
    void copyMemory(int indexPage, int indexTable);
    int searchVictim(int vpn);
    struct MemoryNewADD{
      int vpn = -1;
      int pid = -1;
      int use = 0;
      int dirtyMemory = 0;
    }MemoryNewADD[32];

    typedef struct MemoryNewADD NewPageSys;

    NewPageSys *MemoryNewADD;

  private:
    //BitMap *MiMapa;			// Bitmap of free physical pages
    //TranslationEntry *pageTable;	//Borrar si compila y funciona
					// for now!
    unsigned int numPages;		// Number of pages in the virtual 
					// address space
  char programExecutable[128];
  NoffHeader noffHSwapped;
  OpenFile *Swap = NULL;
};

#endif // ADDRSPACE_H
