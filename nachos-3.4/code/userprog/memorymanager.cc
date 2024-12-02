// memorymanager.cc
// 	Implementation of the MemoryManager, which manages physical memory pages for Nachos.
//
//	The MemoryManager handles allocation and deallocation of memory pages, keeping track
//	of which pages are free using a bitmap.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved. See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "memorymanager.h"
#include "system.h"

MemoryManager::MemoryManager() {
    bitmap = new BitMap(NumPhysPages);
}

MemoryManager::~MemoryManager() {
    delete bitmap;
}

int MemoryManager::AllocatePage() {
    int freePage = bitmap->Find();
    return freePage;
}

int MemoryManager::DeallocatePage(int which) {
    if (which >= 0 && which < NumPhysPages) {
        bitmap->Clear(which);
        return 0;
    }
    return -1;
}

unsigned int MemoryManager::GetFreePageCount() {
    return bitmap->NumClear();
}

void MemoryManager::Join(int pid) {
    PCB* pcb = pcbManager->GetPCB(pid);
    if (pcb != nullptr) {
        pcb->Join();
    }
}