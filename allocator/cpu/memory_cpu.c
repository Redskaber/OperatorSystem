/*
 User: Redskaber
 Date: 2023/12/13
 Time: 21:44
*/
#include "memory_cpu.h"

static _Bool getRunningSpace(Cpu *cpu, int member);

static _Bool freeRunningSpace(Cpu *cpu, int remember);

static void display(const Cpu *cpu);


/**
 * @brief Creates a new Cpu structure.
 *
 * This function allocates memory for a new Cpu structure and initializes its fields.
 * It sets the total, used, and remaining fields to the provided total value, 0, and total value respectively.
 * It also assigns the getRunningSpace, freeRunningSpace, and display functions to the corresponding function pointers in the Cpu structure.
 *
 * @param total The total amount of CPU resources.
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created Cpu structure.
 */
Cpu *createCpu(int total, Allocator *allocator) {

    Cpu *newCpu = NULL;
    newCpu = allocator->allocate(allocator, sizeof(Cpu));
    assert(newCpu != NULL);
    assert(total >= 0);

    newCpu->total = total;
    newCpu->used = 0;
    newCpu->remain = total;

    newCpu->getRunningSpace = getRunningSpace;
    newCpu->freeRunningSpace = freeRunningSpace;
    newCpu->display = display;
    return newCpu;
}

/**
 * @brief Attempts to allocate CPU resources.
 *
 * This function tries to allocate a specified amount of CPU resources.
 * It first checks if the requested amount of resources can be allocated without exceeding the total CPU resources.
 * If the allocation is possible, it increases the used resources by the requested amount, decreases the remaining resources by the same amount, and returns true.
 * If the allocation is not possible, it returns false without modifying the used or remaining resources.
 *
 * @param cpu Pointer to the Cpu structure representing the CPU resources.
 * @param member The amount of CPU resources requested.
 * @return Boolean value indicating whether the allocation was successful.
 */
static _Bool getRunningSpace(Cpu *cpu, int member) {
    if (cpu->used + member <= cpu->total) {
        cpu->used += member;
        cpu->remain -= member;
        return true;
    }
    return false;
}

/**
 * @brief Attempts to free up CPU resources.
 *
 * This function tries to free up a specified amount of CPU resources.
 * It first checks if the requested amount of resources can be freed without exceeding the total CPU resources.
 * If the freeing up is possible, it decreases the used resources by the requested amount, increases the remaining resources by the same amount, and returns true.
 * If the freeing up is not possible, it returns false without modifying the used or remaining resources.
 *
 * @param cpu Pointer to the Cpu structure representing the CPU resources.
 * @param remember The amount of CPU resources to be freed.
 * @return Boolean value indicating whether the freeing up was successful.
 */
static _Bool freeRunningSpace(Cpu *cpu, int remember) {
    if (cpu->remain + remember <= cpu->total) {
        cpu->used -= remember;
        cpu->remain += remember;
        return true;
    }
    return false;
}

/**
 * @brief Displays the details of a Cpu structure.
 *
 * This function prints the details of the Cpu structure to the console.
 * It displays the total, used, and remaining CPU resources.
 * Each resource is displayed in the format "total|used|remaining".
 *
 * @param cpu Pointer to the Cpu structure to be displayed.
 */
static void display(const Cpu *cpu) {
    printf_s("########################################\n");
    printf_s("Cpu:\n");
    printf_s("\ttotal: %d\n", cpu->total);
    printf_s("\tused: %d\n", cpu->used);
    printf_s("\tremain: %d\n", cpu->remain);
    printf_s("########################################\n");
}

/**
 * @brief Destroys a Cpu structure.
 *
 * This function deallocates the memory used by the Cpu structure.
 * It first checks if the Cpu pointer is not NULL.
 * If it is not NULL, it deallocates the memory used by the Cpu structure and sets the Cpu pointer to NULL.
 *
 * @param cpu Pointer to the Cpu structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void destroyCpu(Cpu *cpu, Allocator *allocator) {
    if (cpu != NULL) {
        allocator->deallocate(allocator, cpu, sizeof(Cpu));
        cpu = NULL;
    }
}
