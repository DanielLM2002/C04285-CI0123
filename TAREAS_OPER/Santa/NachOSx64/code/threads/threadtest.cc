// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create several threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustrate the inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
//

#include <unistd.h>
#include "copyright.h"
#include "system.h"
#include "santa.cc"
#include "string.h"

//DiningPh * dp;
Santa * santa;


void santaTrabajo(void *p) {
    int taller = 0;
    int trabajo, standBy;
    long id = (long) p;

    std::vector<char> nombres = {'G','M','T','J'};
    currentThread->Yield();
    Lock lock("sem_con");
    for (int i = 0; i < 10; i++)  {
        lock.Acquire();
            printf("Round %d elf # %ld\n", i, id + 1);
            printf("El duende %ld quiere trabajar \n", id + 1);
        lock.Release();
        taller = santa->iniciarTrabajo(id);
        trabajo = Random() % 6;
        currentThread->Yield();
        sleep(trabajo);
        santa->terminarTrabajo(id, taller);
        standBy = Random() % 6;
        currentThread->Yield();
        sleep(standBy);
    }

}


//----------------------------------------------------------------------
// SimpleThread
// 	Loop 10 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"name" points to a string with a thread name, just for
//      debugging purposes.
//----------------------------------------------------------------------

void
SimpleThread(void* name)
{
    // Reinterpret arg "name" as a string
    char* threadName = (char*)name;
    
    // If the lines dealing with interrupts are commented,
    // the code will behave incorrectly, because
    // printf execution may cause race conditions.
    for (int num = 0; num < 10; num++) {
        //IntStatus oldLevel = interrupt->SetLevel(IntOff);
	printf("*** thread %s looped %d times\n", threadName, num);
	//interrupt->SetLevel(oldLevel);
        //currentThread->Yield();
    }
    //IntStatus oldLevel = interrupt->SetLevel(IntOff);
    printf(">>> Thread %s has finished\n", threadName);
    //interrupt->SetLevel(oldLevel);
}



//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between several threads, by launching
//	ten threads which call SimpleThread, and finally calling 
//	SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest()
{
    Thread * threadDuendes;
    std::cout<<"inicidano en el threadtest"<<std::endl;
    std::cout<< " "<< std::endl;

    DEBUG('t', "Entering SimpleTest");


    santa = new Santa();

    for ( long k = 0; k < 4; k++ ) {
        threadDuendes = new Thread( "santa" );
        threadDuendes->Fork( santaTrabajo, (void *) k );
    }

    return;
/*
    for ( int k=1; k<5; k++) {
      char* threadname = new char[100];
      sprintf(threadname, "Hilo %d", k);
      Thread* newThread = new Thread (threadname);
      newThread->Fork (SimpleThread, (void*)threadname);
    }
*/
    SimpleThread( (void*)"Hilo 0");
}

