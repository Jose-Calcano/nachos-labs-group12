// threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

#if defined(CHANGED) && defined(THREADS)

int numThreadsActive = 0;

#ifdef HW1_SEMAPHORES
Semaphore *mutex = new Semaphore("mutex", 1);
#elif HW1_LOCKS
Lock *mutex = new Lock("mutex");
#endif

int SharedVariable;
void SimpleThread(int which)
{
    int num, val;
    for (num = 0; num < 5; num++)
    {
        // Entry section
        #ifdef HW1_SEMAPHORES
        mutex->P(); // Wait for semaphore
        #elif HW1_LOCKS
        mutex->Acquire(); // Acquire lock
        #endif

        // Critical section
        val = SharedVariable;
        printf("*** thread %d sees value %d\n", which, val);
        currentThread->Yield();
        SharedVariable = val + 1;
    
        // Exit section
        #ifdef HW1_SEMAPHORES
        mutex->V(); // Signal sempahore
        #elif HW1_LOCKS
        mutex->Release(); // Release lock
        #endif

        currentThread->Yield();
    }

    numThreadsActive--;

    while (numThreadsActive > 0)
    {
        currentThread->Yield();
    }

    val = SharedVariable;
    printf("Thread %d sees final value %d\n", which, val);
}

#else

void SimpleThread(int which)
{
    int num;

    for (num = 0; num < 5; num++)
    {
        printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

#endif

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

#if defined(CHANGED) && defined(THREADS)

void ThreadTest(int n)
{
    DEBUG('t', "Entering ThreadTest");
    Thread *t;
    numThreadsActive = n;
    printf("NumthreadsActive = %d\n", numThreadsActive);

    for (int i = 1; i < n; i++)
    {
        t = new Thread("forked thread");
        t->Fork(SimpleThread, i);
    }
    SimpleThread(0);
}

#else

void ThreadTest()
{
    switch (testnum)
    {
    case 1:
        ThreadTest1();
        break;
    default:
        printf("No test specified.\n");
        break;
    }
}

#endif
