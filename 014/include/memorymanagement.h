#ifndef __WYOOS__MEMORYMANAGEMENT_H
#define __WYOOS__MEMORYMANAGEMENT_H

#include "common/types.h"

namespace wyoos
{
    struct MemoryChunk {
        MemoryChunk* next;
        MemoryChunk* prev;
        bool allocated;
        wyoos::common::size_t size;
    };

    class MemoryManager {
    public:
        MemoryManager(wyoos::common::size_t start, wyoos::common::size_t size);
        ~MemoryManager();

        void *malloc(wyoos::common::size_t size);
        void free(void* ptr);

        static MemoryManager* activeMemoryManager;
    private:
        MemoryChunk* first;
    };
}

void* operator new(wyoos::common::size_t size);
void* operator new[](wyoos::common::size_t size);

void* operator new(wyoos::common::size_t size, void* ptr);
void* operator new[](wyoos::common::size_t size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);


#endif