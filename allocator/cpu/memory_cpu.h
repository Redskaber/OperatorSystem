/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/

#pragma once

#ifndef OPERATORSYSTEM_MEMORY_CPU_H
#define OPERATORSYSTEM_MEMORY_CPU_H

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Cpu {
    int total;
    int used;
    int remain;

    _Bool (*getRunningSpace)(struct Cpu *cpu, int member);

    _Bool (*freeRunningSpace)(struct Cpu *cpu, int remember);

    void (*display)(const struct Cpu *cpu);

} Cpu;

extern Cpu *createCpu(int total);

extern void destroyCpu(Cpu *cpu);

#endif //OPERATORSYSTEM_MEMORY_CPU_H
