/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/
#include "memory_cpu.h"

static _Bool getRunningSpace(Cpu *cpu, int member);

static _Bool freeRunningSpace(Cpu *cpu, int remember);

static void display(const Cpu *cpu);

Cpu *createCpu(int total) {

    Cpu *newCpu = NULL;
    newCpu = (Cpu *) malloc(sizeof(Cpu));
    assert(newCpu != NULL);

    newCpu->total = total;
    newCpu->used = 0;
    newCpu->remain = total;

    newCpu->getRunningSpace = getRunningSpace;
    newCpu->freeRunningSpace = freeRunningSpace;
    newCpu->display = display;
    return newCpu;
}


static _Bool getRunningSpace(Cpu *cpu, int member) {
    if (cpu->used + member <= cpu->total) {
        cpu->used += member;
        cpu->remain -= member;
        return true;
    }
    return false;
}

static _Bool freeRunningSpace(Cpu *cpu, int remember) {
    if (cpu->remain + remember <= cpu->total) {
        cpu->used -= remember;
        cpu->remain += remember;
        return true;
    }
    return false;
}

static void display(const Cpu *cpu) {
    printf_s("########################################\n");
    printf_s("Cpu:\n");
    printf_s("\ttotal: %d\n", cpu->total);
    printf_s("\tused: %d\n", cpu->used);
    printf_s("\tremain: %d\n", cpu->remain);
    printf_s("########################################\n");
}

void destroyCpu(Cpu *cpu) {
    if (cpu != NULL) {
        free(cpu);
    }
}