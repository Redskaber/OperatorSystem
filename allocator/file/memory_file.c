/*
 User: Redskaber
 Date: 2023/12/14
 Time: 15:15
*/
#include "memory_file.h"

static _Bool getFileHandler(File *file, int member);

static _Bool returnFileHandler(File *file, int remember);

static void display(const File *file);

File *createFile(int total, Allocator *allocator) {

    File *newFile = NULL;
    newFile = allocator->allocate(allocator, sizeof(File));
    assert(newFile != NULL);

    newFile->total = total;
    newFile->used = 0;
    newFile->remain = total;

    newFile->getFileHandler = getFileHandler;
    newFile->returnFileHandler = returnFileHandler;
    newFile->display = display;
    return newFile;
}


static _Bool getFileHandler(File *file, int member) {
    if (file->used + member <= file->total) {
        file->used += member;
        file->remain -= member;
        return true;
    }
    return false;
}

static _Bool returnFileHandler(File *file, int remember) {
    if (file->remain + remember <= file->total) {
        file->used -= remember;
        file->remain += remember;
        return true;
    }
    return false;
}

static void display(const File *file) {
    printf_s("########################################\n");
    printf_s("File:\n");
    printf_s("\ttotal: %d\n", file->total);
    printf_s("\tused: %d\n", file->used);
    printf_s("\tremain: %d\n", file->remain);
    printf_s("########################################\n");
}

void destroyFile(File *file, Allocator *allocator) {
    if (file != NULL) {
        allocator->deallocate(allocator, file, sizeof(File));
        file = NULL;
    }
}