/*
 User: Redskaber
 Date: 2023/12/14
 Time: 15:15
*/
#include "memory_file.h"

static _Bool getFileHandler(File *file, int member);

static _Bool returnFileHandler(File *file, int remember);

static void display(const File *file);


/**
 * @brief Creates a new File structure.
 *
 * This function allocates memory for a new File structure and initializes its fields.
 * It sets the total, used, and remaining fields to the provided total value, 0, and total value respectively.
 * It also assigns the getFileHandler, returnFileHandler, and display functions to the corresponding function pointers in the File structure.
 *
 * @param total The total amount of File resources.
 * @param allocator Pointer to the Allocator structure used for memory management.
 * @return Pointer to the newly created File structure.
 */
File *createFile(int total, Allocator *allocator) {

    File *newFile = NULL;
    newFile = allocator->allocate(allocator, sizeof(File));
    assert(newFile != NULL);
    assert(total >= 0);

    newFile->total = total;
    newFile->used = 0;
    newFile->remain = total;

    newFile->getFileHandler = getFileHandler;
    newFile->returnFileHandler = returnFileHandler;
    newFile->display = display;
    return newFile;
}

/**
 * @brief Attempts to allocate File resources.
 *
 * This function tries to allocate a specified amount of File resources.
 * It first checks if the requested amount of resources can be allocated without exceeding the total File resources.
 * If the allocation is possible, it increases the used resources by the requested amount, decreases the remaining resources by the same amount, and returns true.
 * If the allocation is not possible, it returns false without modifying the used or remaining resources.
 *
 * @param file Pointer to the File structure representing the File resources.
 * @param member The amount of File resources requested.
 * @return Boolean value indicating whether the allocation was successful.
 */
static _Bool getFileHandler(File *file, int member) {
    if (file->used + member <= file->total) {
        file->used += member;
        file->remain -= member;
        return true;
    }
    return false;
}

/**
 * @brief Attempts to free up File resources.
 *
 * This function tries to free up a specified amount of File resources.
 * It first checks if the requested amount of resources can be freed without exceeding the total File resources.
 * If the freeing up is possible, it decreases the used resources by the requested amount, increases the remaining resources by the same amount, and returns true.
 * If the freeing up is not possible, it returns false without modifying the used or remaining resources.
 *
 * @param file Pointer to the File structure representing the File resources.
 * @param remember The amount of File resources to be freed.
 * @return Boolean value indicating whether the freeing up was successful.
 */
static _Bool returnFileHandler(File *file, int remember) {
    if (file->remain + remember <= file->total) {
        file->used -= remember;
        file->remain += remember;
        return true;
    }
    return false;
}

/**
 * @brief Displays the details of a File structure.
 *
 * This function prints the details of the File structure to the console.
 * It displays the total, used, and remaining File resources.
 * Each resource is displayed in the format "total|used|remaining".
 *
 * @param file Pointer to the File structure to be displayed.
 */
static void display(const File *file) {
    printf_s("########################################\n");
    printf_s("File:\n");
    printf_s("\ttotal: %d\n", file->total);
    printf_s("\tused: %d\n", file->used);
    printf_s("\tremain: %d\n", file->remain);
    printf_s("########################################\n");
}

/**
 * @brief Destroys a File structure.
 *
 * This function deallocates the memory used by the File structure.
 * It first checks if the File pointer is not NULL.
 * If it is not NULL, it uses the provided Allocator to deallocate the memory used by the File structure and sets the File pointer to NULL.
 *
 * @param file Pointer to the File structure to be destroyed.
 * @param allocator Pointer to the Allocator structure used for memory management.
 */
void destroyFile(File *file, Allocator *allocator) {
    if (file != NULL) {
        allocator->deallocate(allocator, file, sizeof(File));
        file = NULL;
    }
}
