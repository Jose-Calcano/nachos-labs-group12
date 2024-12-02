// pcbmanager.cc
// 	Implementation of the PCBManager, which manages Process Control Blocks (PCBs) for Nachos.
//
//	The PCBManager is used to allocate, deallocate, and manage PCBs for different processes.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved. See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "pcbmanager.h"
#include "pcb.h"
#include "system.h"

PCBManager::PCBManager() {
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        pcbTable[i] = nullptr;
    }
}

PCBManager::~PCBManager() {
    for (int i = 0; i < MAX_PROCESSES; ++i) {
        if (pcbTable[i] != nullptr) {
            delete pcbTable[i];
            pcbTable[i] = nullptr;
        }
    }
}

PCB* PCBManager::AllocatePCB(int pid, int parentPid) {
    if (pid < 0 || pid >= MAX_PROCESSES) {
        return nullptr;
    }

    if (pcbTable[pid] == nullptr) {
        pcbTable[pid] = new PCB(pid, parentPid);
    }
    return pcbTable[pid];
}

PCB* PCBManager::GetPCB(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES) {
        return nullptr;
    }
    return pcbTable[pid];
}

void PCBManager::DeallocatePCB(int pid) {
    if (pid >= 0 && pid < MAX_PROCESSES && pcbTable[pid] != nullptr) {
        delete pcbTable[pid];
        pcbTable[pid] = nullptr;
    }
}

void PCBManager::JoinProcess(int childPid) {
    PCB* childPCB = GetPCB(childPid);
    if (childPCB != nullptr) {
        PCB* parentPCB = childPCB->parent;
        if (parentPCB != nullptr) {
            parentPCB->SetWaiting(true);
            childPCB->Join();
            parentPCB->SetWaiting(false);
        }
    }
}
