// addrspace.cc 
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option 
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "openfile.h"
//#include "noff.h"
#include "synch.h"

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the 
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------



static void 
SwapHeader (NoffHeader *noffH)
{
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address memSpace to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical 
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

/*
* Constructor de la clase AddrSpace que acepta un bitmap padre
* @param executable Archivo que contiene el codigo del programa a ejecutar
*/
int iter = 0;
AddrSpace::AddrSpace(AddrSpace *parentSpace) {
    numPages = parentSpace->numPages; 
    pageTable = new TranslationEntry[parentSpace->numPages];
    unsigned int i = 0;
    for (i; i <= (parentSpace->numPages - 8); i++) {
        pageTable[i].virtualPage = i;
        pageTable[i].physicalPage = parentSpace->pageTable[i].physicalPage;
        pageTable[i].valid = parentSpace->pageTable[i].valid;
        pageTable[i].use = parentSpace->pageTable[i].use;
        pageTable[i].dirty = parentSpace->pageTable[i].dirty;
        pageTable[i].readOnly = parentSpace->pageTable[i].readOnly;
    }
    for(i; i < parentSpace->numPages; i++) {
        pageTable[i].virtualPage = i;
        pageTable[i].physicalPage = MiMapa->Find();
        pageTable[i].valid = true;
        pageTable[i].use = false;
        pageTable[i].dirty = false;
        pageTable[i].readOnly = false;
    }
}

AddrSpace::AddrSpace(OpenFile *executable, const char *filename) {
    if (Swap == NULL) {
        Swap = fileSystem->Open("test");
        ipt = new TPI;
    }
    #ifdef VM
        strcpy (executableFile, filename);
    #endif
    NoffHeader noffH;
    unsigned int i, size;

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) && (WordToHost(noffH.noffMagic) == NOFFMAGIC)) SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);
        noffH1 = noffH;
    
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;	

        #ifndef VM
            ASSERT(numPages <= NumPhysPages);
        #endif

    pageTable = new TranslationEntry[numPages];
    for (int i = 0; i < numPages; i++) {
        pageTable[i].virtualPage = i;
        #ifndef VM
            pageTable[i].physicalPage = MiMapa->Find();
            pageTable[i].valid = true;
        #else
            pageTable[i].physicalPage = -1;
            pageTable[i].valid = false;
        #endif
        pageTable[i].use = false;
        pageTable[i].dirty = false;
        pageTable[i].readOnly = false;

    }

    #ifndef VM
        int numPages2 = divRoundUp(noffH.code.size, numPages);
        int memoryDirectory = noffH.code.inFileAddr;
        for (int j = 0; j < numPages2; j++) {
            executable->ReadAt(&(machine->mainMemory[pageTable[j].physicalPage * 128]), PageSize, memoryDirectory);
            memoryDirectory += 128;
        }

        int numPages3 = divRoundUp(noffH.initData.size, numPages);
        memoryDirectory = noffH.initData.inFileAddr;
        for (int j = numPages2; j < numPages3; j++) {
            executable->ReadAt(&(machine->mainMemory[pageTable[j].physicalPage * 128]), PageSize, memoryDirectory);
            memoryDirectory += 128;
        }
    #endif

}



//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address memSpace.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
   delete pageTable;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);	

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

   // Set the stack register to the end of the address memSpace, where we
   // allocated the stack; but subtract off a bit, to make sure we don't
   // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address memSpace, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState() 
{
    for (int i = 0; i < 4 ; i++) {
        pageTable[machine->tlb[i].virtualPage].dirty = machine->tlb[i].dirty;
        pageTable[machine->tlb[i].virtualPage].valid = machine->tlb[i].valid;
    }
}



//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address memSpace can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState() 
{
    #ifndef VM
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
    #endif
}

void AddrSpace::copyMemory(int indexPageTable, int indexOfMem) {
    machine->tlb[indexOfMem].virtualPage = pageTable[indexPageTable].virtualPage;
    machine->tlb[indexOfMem].physicalPage = pageTable[indexPageTable].physicalPage;
    machine->tlb[indexOfMem].valid = pageTable[indexPageTable].valid;
    machine->tlb[indexOfMem].use = pageTable[indexPageTable].use;
    machine->tlb[indexOfMem].dirty = pageTable[indexPageTable].dirty;
    machine->tlb[indexOfMem].readOnly = pageTable[indexPageTable].readOnly;

}

void AddrSpace::SwapMem(int vpn) { //metodo parcialmente inspirado en https://github.com/daviddwlee84/OperatingSystem/blob/master/Nachos/nachos-3.4/code/userprog/addrspace.cc
    for (int i = 0; i < 4; i++) {
        pageTable[machine->tlb[i].virtualPage].use = machine->tlb[i].use;
        pageTable[machine->tlb[i].virtualPage].dirty = machine->tlb[i].dirty;
        //pageTable[machine->tlb[i].virtualPage].valid = machine->tlb[i].valid;//remover si falla
    }
    OpenFile *swappedFile = fileSystem->Open("executableFile");
    int numPages1= divRoundUp(noffH1.code.size, PageSize);
    int numPages2 = divRoundUp(noffH1.initData.size, PageSize);
    int numPages3 = divRoundUp(noffH1.uninitData.size, PageSize);//remover si da problemas
    bool isValid = false;
    bool isDirty = false;
    int newVictim = 0;
    if (vpn < numPages1){
        printf("Code of vpn \n");
        if (!isValid) {
            //cargar la memoria y la tlb
            newVictim = MiMapa->Find();
            if (newVictim == -1) {
                newVictim = searchVictim(vpn);
            }
            pageTable[vpn].physicalPage = newVictim;
            pageTable[vpn].virtualPage = vpn;
            pageTable[vpn].valid = true;
            pageTable[vpn].use = true;
            pageTable[vpn].dirty = false;
            pageTable[vpn].readOnly = true;
            copyMemory(vpn,iter);
            iter = (iter + 1) % 4;
            swappedFile->ReadAt(&(machine->mainMemory[(pageTable[vpn].physicalPage) * PageSize]), PageSize, sizeof(NoffHeader)+vpn*PageSize);
            ipt[newVictim].vpn = vpn;
        } else {
            // pasar a tlb
            copyMemory(vpn,iter);
            iter = (iter + 1) % 4;
        }
    } else if (vpn < numPages2) {
        printf("Data was not inicialized \n");
        if (!isValid && isDirty) {
            //cargar la memoria y la tlb
            newVictim = MiMapa->Find();
            if (newVictim == -1) {
                newVictim = searchVictim(vpn);
                if(pageTable[machine->tlb[iter].virtualPage].dirty) {
                    int memSpace = MiMapa->Find();
                    Swap->WriteAt(&(machine->mainMemory[(pageTable[iter].physicalPage)]), PageSize, memSpace*PageSize);
                    MiMapa->Clear(machine->tlb[iter].physicalPage);
                    pageTable[machine->tlb[iter].virtualPage].physicalPage = memSpace;
                    pageTable[machine->tlb[iter].virtualPage].valid = false;
                }
            }
            pageTable[vpn].physicalPage = newVictim;
            pageTable[vpn].virtualPage = vpn;
            pageTable[vpn].valid = true;
            pageTable[vpn].use = true;
            pageTable[vpn].dirty = false;
            pageTable[vpn].readOnly = false;
            copyMemory(vpn,iter);
            iter = (iter + 1) % 4;
            swappedFile->ReadAt(&(machine->mainMemory[(pageTable[vpn].physicalPage) * PageSize]), PageSize, sizeof(NoffHeader)+numPages1*PageSize+vpn*PageSize);//quitar numpages1 y el rpimer pageSize
            ipt[newVictim].vpn = vpn;
        } else {
            // pasar a tlb la page table
            copyMemory(vpn,iter);
            iter = (iter + 1) % 4;
        }
    } else if (vpn < numPages2) {
        printf("Data was inicialized \n");
        if (!isDirty && isValid) {
            // cargar a la memoria el nuevo espacio asignado del TLB
            newVictim = MiMapa->Find();
            if (newVictim == -1) {
                newVictim = searchVictim(vpn);
                if(pageTable[machine->tlb[iter].virtualPage].dirty) {
                    int memSpace = MiMapa->Find();
                    Swap->WriteAt(&(machine->mainMemory[(pageTable[iter].physicalPage)]), PageSize, memSpace*PageSize);
                    MiMapa->Clear(machine->tlb[iter].physicalPage);
                    pageTable[machine->tlb[iter].virtualPage].physicalPage = memSpace;
                    pageTable[machine->tlb[iter].virtualPage].valid = false;
                }
            }
            pageTable[vpn].physicalPage = newVictim;
            pageTable[vpn].virtualPage = vpn;
            pageTable[vpn].valid = true;
            pageTable[vpn].use = true;
            pageTable[vpn].dirty = false;
            pageTable[vpn].readOnly = false;
            copyMemory(vpn,iter);
            iter = (iter + 1) % 4;
            swappedFile->ReadAt(&(machine->mainMemory[(pageTable[vpn].physicalPage) * 128]), PageSize, 128 * PageSize);//quitar numpages1 y el rpimer pageSize
            int memSpace = MiMapa->Find();
            MiMapa->Clear(memSpace);
            ipt[newVictim].vpn = vpn;
        }
    } else {
        printf("Data that wasnt inicialized \n");
        if(!isValid && isDirty) {
            //Cargar a la tage table y a la memoria
            newVictim = MiMapa->Find();
            if(newVictim == -1) {
                newVictim = searchVictim(vpn);
                if(pageTable[machine->tlb[iter].virtualPage].dirty){
                    int memSpace = MiMapa->Find();
                    Swap->WriteAt(&(machine->mainMemory[(pageTable[iter].physicalPage)]), PageSize, memSpace*PageSize);
                    MiMapa->Clear(machine->tlb[iter].physicalPage);
                    pageTable[machine->tlb[iter].virtualPage].physicalPage = memSpace;
                    pageTable[machine->tlb[iter].virtualPage].valid = false;
                }
            }
            pageTable[vpn].physicalPage = newVictim;
            pageTable[vpn].virtualPage = vpn;
            pageTable[vpn].valid = true;
            pageTable[vpn].use = true;
            pageTable[vpn].dirty = false;
            pageTable[vpn].readOnly = false;
            copyMemory(vpn,iter);
            iter = (iter + 1) % 4;
            ipt[newVictim].vpn = vpn;
        }else if(isValid) {
            // pasar a tlb
            copyMemory(vpn,iter);
            iter = (iter + 1) % 4;
        } else {
            //cargar a la memoria y a la tlb
            newVictim = MiMapa->Find();
            if(newVictim == -1) {
                newVictim = searchVictim(vpn);
                if(pageTable[machine->tlb[iter].virtualPage].dirty){
                    int memSpace = MiMapa->Find();
                    Swap->WriteAt(&(machine->mainMemory[(pageTable[iter].physicalPage)]), PageSize, memSpace*PageSize);
                    MiMapa->Clear(machine->tlb[iter].physicalPage);
                    pageTable[machine->tlb[iter].virtualPage].physicalPage = memSpace;
                    pageTable[machine->tlb[iter].virtualPage].valid = false;
                }
            }
            pageTable[vpn].physicalPage = newVictim;
            pageTable[vpn].virtualPage = vpn;
            pageTable[vpn].valid = true;
            pageTable[vpn].use = true;
            pageTable[vpn].dirty = false;
            pageTable[vpn].readOnly = false;
            copyMemory(vpn,iter);
            iter = (iter + 1) % 4;
            Swap->ReadAt(&(machine->mainMemory[(pageTable[vpn].physicalPage)*128]), PageSize, 128*PageSize);
            int memSpace = MiMapa->Find();
            MiMapa->Clear(memSpace);
            ipt[newVictim].vpn = vpn;
        }
    }

}

int AddrSpace::searchVictim(int vpn) {
    int index = -1;
    int min = 100000000;
    for (int i = 0; i < TLBSize; i++) {
        if (machine->tlb[i].valid == false) {
            index = i;
            break;
        }
        if (machine->tlb[i].use == false) {
            index = i;
            break;
        }
        if (machine->tlb[i].use == true) {
            machine->tlb[i].use = false;
            if (machine->tlb[i].dirty == true) {
                int vpn = machine->tlb[i].virtualPage;
                int ppn = machine->tlb[i].physicalPage;
                Swap->WriteAt(&(machine->mainMemory[ppn * PageSize]), PageSize, vpn * PageSize);
                pageTable[vpn].dirty = false;
            }
        }
    }
    if (index == -1) {
        index = rand() % TLBSize;
        if (machine->tlb[index].dirty == true) {
            int vpn = machine->tlb[index].virtualPage;
            int ppn = machine->tlb[index].physicalPage;
            Swap->WriteAt(&(machine->mainMemory[ppn * PageSize]), PageSize, vpn * PageSize);
            pageTable[vpn].dirty = false;
        }
    }
    return index;
}

