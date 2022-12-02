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
// 	Create an address space to run a user program.
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
AddrSpace::AddrSpace(AddrSpace* parentSpace) {
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

AddrSpace::AddrSpace(OpenFile *executable, const char * filename)
{
    if (Swap == NULL) {
        Swap = fileSystem->Open("SWAP");
        MemoryNewADD = new NewPageSys;
    }
    #ifdef VM
        strcpy (executable, filename)
    #endif

    NoffHeader noffH;
    unsigned int i, size;

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) && 
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);
        noffHSwapped = noffH;

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size 
			+ UserStackSize;	// we need to increase the size
						// to leave room for the stack
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;
    #ifndef VM
    ASSERT(numPages <= NumPhysPages);		// check we're not trying
    #endif
						// to run anything too big --
						// at least until we have
						// virtual memory

    DEBUG('a', "Initializing address space, num pages %d, size %d\n", 
					numPages, size);
// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++) {
        pageTable[i].virtualPage = i;
	    //pageTable[i].virtualPage = MiMapa->Find();	// for now, virtual page # = phys page #
        #ifndef VM
            pageTable[i].physicalPage = MiMapa->Find();	// for now, virtual page # = phys page #
            //Lock MiCandado("MiCandado");
	        //pageTable[i].physicalPage = i;
	        pageTable[i].valid = true;
        #else
            pageTable[i].valid = false;
            pageTable[i].physicalPage = -1
        #endif

	    pageTable[i].use = false;
	    pageTable[i].dirty = false;
	    pageTable[i].readOnly = false;  // if the code segment was entirely on 
					// a separate page, we could set its 
					// pages to be read-only
    }
    
// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
    //bzero(machine->mainMemory, size);


// then, copy in the code and data segments into memory
    /*if (noffH.code.size > 0) {
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", 
			noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]),
			noffH.code.size, noffH.code.inFileAddr);
    }
    if (noffH.initData.size > 0) {
        DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", 
			noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAt(&(machine->mainMemory[noffH.initData.virtualAddr]),
			noffH.initData.size, noffH.initData.inFileAddr);
    }*/
    #ifndef VM
        int numPages2 = divRoundUp(noffH.code.size, numPages);
		int addrsMem = noffH.code.inFileAddr;
		for (int j = 0 ; j < numPages2; ++j) {
			executable->ReadAt(&(machine->mainMemory[pageTable[j].physicalPage*128]), 
                                        PageSize, addrsMem);
			addrsMem = addrsMem + 128;
		}
        int numPages3 = divRoundUp(noffH.initData.size, numPages);
		addrsMem = noffH.initData.inFileAddr;
		for (int j = numPages2; j < numPages3; ++j) {
			executable->ReadAt(&(machine->mainMemory[pageTable[j].physicalPage*128]),
                                        PageSize, addrsMem);
			addrsMem = addrsMem + 128;
		}
    #endif


}
int pageIterator = 0;
void AddrSpace::moveMemory(int vpn) {
    for (int i = 0; i < 4; i++) {
        pageTable[machine->tlb[i].virtualPage].dirty = machine->tlb[i].dirty;
        pageTable[machine->tlb[i].virtualPage].use = machine->tlb[i].use;
    }
    OpenFile *execPrg = fileSystem->Open(programExecutable);
    int numPages1 = divRoundUp(noffHSwapped.code.size, PageSize);
    int numPages2 = divRoundUp(noffHSwapped.initData.size, PageSize);
    bool isValid = pageTable[vpn].valid;
    bool isDirty = pageTable[vpn].dirty;
    int physPagNum = 0;
    if(vpn<numPages1) {
        printf("Code: \n" );
        if(!isValid) {
            physPagNum = MiMapa->Find();
            if(physPagNum == -1) {
                physPagNum = searchVictim(vpn);
            }
            pageTable[vpn].physicalPage = physPagNum;
            pageTable[vpn].virtualPage = vpn;
            pageTable[vpn].valid = true;
            pageTable[vpn].use = true;
            pageTable[vpn].dirty = false;
            pageTable[vpn].readOnly = true;
            copyMemory(vpn, pageIterator);
            pageIterator = ++pageIterator % 4;
            execPrg->ReadAt(&(machine->mainMemory[(pageTable[vpn].physicalPage)*PageSize]),
                            PageSize,sizeof(NoffHeader)+vpn*PageSize);
            MemoryNewADD[physPagNum].vpn = vpn;
        } else {
            copyMemory(vpn, pageIterator);
            pageIterator = ++pageIterator % 4;
        }
    } else if(vpn < numPages2) {
        printf("Inicial Data: \n" );
        if(!isDirty && !isValid) {
            physPagNum = MiMapa->Find();
            if(physPagNum == -1) {
                physPagNum = searchVictim(vpn);
                if(pageTable[machine->tlb[pageIterator].virtualPage].dirty) {
                    int space = swapMap->Find();
                    Swap->WriteAt(&(machine->mainMemory[(pageTable[pageIterator].physicalPage)]),
                                    PageSize,space*PageSize);
                    MiMapa->Clear(machine->tlb[pageIterator].physicalPage);
                    pageTable[machine->tlb[pageIterator].virtualPage].physicalPage = space;
                    pageTable[machine->tlb[pageIterator].virtualPage].valid = false;
                }
            }
            pageTable[vpn].physicalPage = physPagNum;
            pageTable[vpn].virtualPage = vpn;
            pageTable[vpn].valid = true;
            pageTable[vpn].use = true;
            pageTable[vpn].dirty = false;
            pageTable[vpn].readOnly = false;
            copyMemory(vpn, pageIterator);
            pageIterator = ++pageIterator % 4;
            execPrg->ReadAt(&(machine->mainMemory[(pageTable[vpn].physicalPage)*PageSize]),
                                PageSize, sizeof(NoffHeader)+(vpn*PageSize));
            MemoryNewADD[NumPhysPages].vpn = vpn;
        } else if(isValid) {
            copyMemory(vpn, pageIterator);
            pageIterator = ++pageIterator % 4;
        } else {
            int oldMemory = pageTable[vpn].physicalPage;
            physPagNum = MiMapa->Find();
            if(physPagNum == -1) {
                physPagNum = searchVictim(vpn);
                if(pageTable[machine->tlb[pageIterator].virtualPage].dirty) {
                    int space = swapMap->Find();
                    Swap->WriteAt(&(machine->mainMemory[(pageTable[pageIterator].physicalPage)]),
                                    PageSize,space*PageSize);
                    MiMapa->Clear(machine->tlb[pageIterator].physicalPage);
                    pageTable[machine->tlb[pageIterator].virtualPage].physicalPage = space;
                    pageTable[machine->tlb[pageIterator].virtualPage].valid = false;
                }
            }
            pageTable[vpn].physicalPage = physPagNum;
            pageTable[vpn].virtualPage = vpn;
            pageTable[vpn].valid = true;
            pageTable[vpn].use = true;
            pageTable[vpn].dirty = true;
            pageTable[vpn].readOnly = false;
            copyMemory(vpn, pageIterator);
            pageIterator = ++pageIterator % 4;
            Swap->ReadAt(&(machine->mainMemory[(pageTable[vpn].physicalPage)*128]),
                            PageSize, oldMemory*PageSize);
            MiMapa->Clear(oldMemory);
            MemoryNewADD[physPagNum].vpn = vpn;
        }

    } else {
        printf("Data was not initialized: \n" );
        if (!isValid && !isDirty) {
            physPagNum = MiMapa->Find();
            if(physPagNum == -1) {
                physPagNum = searchVictim(vpn);
                if(pageTable[machine->tlb[pageIterator].virtualPage].dirty) {
                    int space = swapMap->Find();
                    Swap->WriteAt(&(machine->mainMemory[(pageTable[pageIterator].physicalPage)]),
                                    PageSize,space*PageSize);
                    MiMapa->Clear(machine->tlb[pageIterator].physicalPage);
                    pageTable[machine->tlb[pageIterator].virtualPage].physicalPage = space;
                    pageTable[machine->tlb[pageIterator].virtualPage].valid = false;
                }
            }
            pageTable[vpn].physicalPage = physPagNum;
            pageTable[vpn].virtualPage = vpn;
            pageTable[vpn].valid = true;
            pageTable[vpn].use = true;
            pageTable[vpn].dirty = false;
            pageTable[vpn].readOnly = false;
            copyMemory(vpn, pageIterator);
            pageIterator = ++pageIterator % 4;
            MemoryNewADD[physPagNum].vpn = vpn;
        } else if(isValid) {
            copyMemory(vpn, pageIterator);
            pageIterator = ++pageIterator % 4;
        } else {
            int oldMemory = pageTable[vpn].physicalPage;
            physPagNum = MiMapa->Find();
            if(physPagNum == -1) {
                physPagNum = searchVictim(vpn);
                if(pageTable[machine->tlb[pageIterator].virtualPage].dirty) {
                    int space = swapMap->Find();
                    Swap->WriteAt(&(machine->mainMemory[(pageTable[pageIterator].physicalPage)]),
                                    PageSize,space*PageSize);
                    MiMapa->Clear(machine->tlb[pageIterator].physicalPage);
                    pageTable[machine->tlb[pageIterator].virtualPage].physicalPage = space;
                    pageTable[machine->tlb[pageIterator].virtualPage].valid = false;
                }
            }
            pageTable[vpn].physicalPage = physPagNum;
            pageTable[vpn].virtualPage = vpn;
            pageTable[vpn].valid = true;
            pageTable[vpn].use = true;
            pageTable[vpn].dirty = true;
            pageTable[vpn].readOnly = false;
            copyMemory(vpn, pageIterator);
            pageIterator = ++pageIterator % 4;
            Swap->ReadAt(&(machine->mainMemory[(pageTable[vpn].physicalPage)*128]),
                            PageSize, oldMemory*PageSize);
            swapMap->Clear(oldMemory);
            MemoryNewADD[physPagNum].vpn = vpn;
        }
    }
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
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

   // Set the stack register to the end of the address space, where we
   // allocated the stack; but subtract off a bit, to make sure we don't
   // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState() {
    for (int i = 0; i < 4; i++) {
        pageTable[machine->tlb[i].virtualPage].dirty = machine->tlb[i].dirty;
        pageTable[machine->tlb[i].virtualPage].valid = machine->tlb[i].valid;
    }
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
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


void AddrSpace::copyMemory(int indexPage, int indexTable) {
    machine->tlb[indexTable].virtualPage = pageTable[indexPage].virtualPage;
    machine->tlb[indexTable].physicalPage = pageTable[indexPage].physicalPage;
    machine->tlb[indexTable].valid = pageTable[indexPage].valid;
    machine->tlb[indexTable].use = pageTable[indexPage].use;
    machine->tlb[indexTable].dirty = pageTable[indexPage].dirty;
    machine->tlb[indexTable].readOnly = pageTable[indexPage].readOnly;
}

int AddrSpace::searchVictim(int vpn) {
    int victim = -69;
    bool jobsFinished = false;// una gran referencia a Kobe Bryant si me lo preguntan
    while (!jobsFinished) {//job's not finished
        for (int index = 1; index < 32; index++) {
            if(!pageTable[MemoryNewADD[index].vpn].use) {
                victim = pageTable[MemoryNewADD[index].vpn].physicalPage;
                jobsFinished = true;
                break;
            } else {
                pageTable[MemoryNewADD[index].vpn].use = false;
            }
        }
        return victim;
    }

}