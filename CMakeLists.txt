cmake_minimum_required(VERSION 3.26)
project(OperatorSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(OperatorSystem main.c
        process/process_scheduling.h
        process/process_scheduling.c
        process/test/process_scheduling/test_process_scheduling.c
        allocation/banker.c
        allocation/banker.h
        allocation/test/test_allocator.h
        allocation/test/test_allocator.c
        allocator/systemResource.c
        allocator/systemResource.h
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
        allocator/network/memory_network.c
        allocator/network/memory_network.h
        allocator/file/memory_file.c
        allocator/file/memory_file.h
        allocation/base/resource_allocate.c
        allocation/base/resource_allocate.h
)
