/*
 User: Redskaber
 Date: 2023/12/14
 Time: 15:15
*/

#pragma once
#ifndef OPERATORSYSTEM_MEMORY_FILE_H
#define OPERATORSYSTEM_MEMORY_FILE_H

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>
#include "../memory/memory_allocator.h"

typedef struct File {
    int total;
    int used;
    int remain;

    _Bool (*getFileHandler)(struct File *file, int member);

    _Bool (*returnFileHandler)(struct File *file, int remember);

    void (*display)(const struct File *file);
} File;

extern File *createFile(int total, Allocator *allocator);

extern void destroyFile(File *file, Allocator *allocator);


#endif //OPERATORSYSTEM_MEMORY_FILE_H
