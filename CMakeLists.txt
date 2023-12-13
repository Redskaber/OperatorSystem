cmake_minimum_required(VERSION 3.26)
project(OperatorSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(OperatorSystem main.c
        process/process_scheduling.h
        process/process_scheduling.c
        process/test/process_scheduling/test_process_scheduling.c
        allocation/allocator.c
        allocation/allocator.h
        allocation/test/test_allocator.h
        allocation/test/test_allocator.c
        allocator/memory.c
        allocator/memory.h
        allocation/teacher/teacher_template.cpp
        allocation/teacher/teacher_template.h
        allocator/memory/memory_allocator.c
        allocator/memory/memory_allocator.h
        allocator/cpu/memory_cpu.c
        allocator/cpu/memory_cpu.h
        allocator/gpu/memory_gpu.c
        allocator/gpu/memory_gpu.h
        allocator/swap/memory_swap.c
        allocator/swap/memory_swap.h
)
