
#include "memory/SystemMemoryManager.hpp"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
namespace mllm {

static inline void **align(void **ptr, size_t alignment) {
    return (void **)((intptr_t)((unsigned char *)ptr + alignment - 1) & -alignment);
}

void SystemMemoryManager::alloc(void **ptr, size_t size,size_t alignment){
    assert(size > 0);
    // allocate a block of memory, void* is used to store the original pointer
    void **origin = (void **)malloc(size + sizeof(void *) + alignment-1);
    assert(origin != nullptr);
    if (origin == nullptr) {
        *ptr = nullptr;
    }
    void **aligned = (void**)(((size_t)(origin) + sizeof(void*) +  alignment - 1) & (~(alignment - 1)));
    aligned[-1]    = origin;
    *ptr = aligned;
}

void SystemMemoryManager::free(void *ptr){
    ::free(((void**)ptr)[-1]);
}

} // namespace mllm