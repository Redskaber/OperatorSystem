cmake_minimum_required(VERSION 3.26)
project(OperatorSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(OperatorSystem main.c
    process/process_scheduling.h process/process_scheduling.c
        process/test/process_scheduling/test_process_scheduling.c
        allocation/allocator.c
        allocation/allocator.h
        allocation/test/test_allocator.h
        allocation/test/test_allocator.c
        allocator/memory.c
        allocator/memory.h
)
