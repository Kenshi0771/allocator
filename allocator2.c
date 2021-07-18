#include <string.h>
#include "allocator2.h"

#define portSUSPEND_ALL //! dummy function for susspend all tasks
#define portRESUME_ALL  //! dummy function for resume all tasks

size_t freeMem = 0;
uint8_t *poolHeap;

void initHeap(uint8_t *heap, size_t size)
{
    poolHeap = heap;
    header_t *header = (header_t *)heap;
    header->prev = NULL;
    header->size = size - HEADER_SIZE;
    header->next = NULL;
    header->used = FREE;

    freeMem = size - HEADER_SIZE;
}

void *ma(size_t size)
{
    void *ptr = 0;
    if (!size)
        return ptr;

    if (size % ALIGMENT)
        size += (ALIGMENT - size % ALIGMENT);

    if (freeMem < size)
        return ptr;

    header_t *currentBlock = (header_t *)poolHeap;

    portSUSPEND_ALL;
    while (currentBlock)
    {
        if (currentBlock->used || (currentBlock->size < size + HEADER_SIZE))
        {
            currentBlock = (header_t *)currentBlock->next;
            continue;
        }

        header_t *newNextBlock = (header_t *)((uint8_t *)currentBlock + HEADER_SIZE + size);
        newNextBlock->prev = currentBlock;
        newNextBlock->next = currentBlock->next;
        newNextBlock->size = currentBlock->size - size - HEADER_SIZE;
        if (newNextBlock->next)
            ((header_t *)(newNextBlock->next))->prev = newNextBlock;

        currentBlock->next = newNextBlock;
        currentBlock->size = size;
        currentBlock->used = USED;
        freeMem -= HEADER_SIZE + size;
        ptr = (uint8_t *)currentBlock + HEADER_SIZE;
        break;
    }
    portRESUME_ALL;
    return ptr;
}

void *re(void *ptr, size_t size)
{
    if (!ptr)
        return ma(size);
    if (!size)
    {
        fr(ptr);
        return 0;
    }

    if (size % ALIGMENT)
        size += (ALIGMENT - size % ALIGMENT);

    header_t *currentBlock = (header_t *)(((uint8_t *)ptr) - HEADER_SIZE);
    if (size == currentBlock->size)
        return ptr;

    header_t *nextBlock = (header_t *)currentBlock->next;

    if (size < currentBlock->size)
    {
        header_t *newBlock = (header_t *)((uint8_t *)currentBlock + size + HEADER_SIZE);

        if (nextBlock && !nextBlock->used)
        {
            newBlock->prev = currentBlock;
            newBlock->next = nextBlock->next;
            newBlock->used = FREE;
            newBlock->size = nextBlock->size + currentBlock->size - size;
            freeMem += currentBlock->size - size;

            if (nextBlock->next)
            {
                header_t *nextNextBlock = (header_t *)nextBlock->next;
                nextNextBlock->prev = newBlock;
            }

            currentBlock->size = size;
            currentBlock->next = nextBlock;
        }
        else if (currentBlock->size - size >= HEADER_SIZE + ALIGMENT)
        {
            newBlock->prev = currentBlock;
            newBlock->next = nextBlock;
            newBlock->size = currentBlock->size - size - HEADER_SIZE;
            newBlock->used = FREE;
            freeMem += currentBlock->size - size - HEADER_SIZE;

            if (nextBlock)
                nextBlock->prev = newBlock;

            currentBlock->next = newBlock;
            currentBlock->size = size;
        }
    }
    else
    {
        if (nextBlock && !nextBlock->used && (nextBlock->size + HEADER_SIZE >= size - currentBlock->size))
        {
            if (nextBlock->size >= size - currentBlock->size + ALIGMENT)
            {
                header_t *newBlock = (header_t *)((uint8_t *)currentBlock + size + HEADER_SIZE);

                newBlock->used = FREE;
                newBlock->prev = currentBlock;
                newBlock->next = nextBlock->next;
                newBlock->size = size - currentBlock->size;
                freeMem -= size - currentBlock->size;

                if (nextBlock->next)
                {
                    header_t *nextNextBlock = (header_t *)nextBlock->next;
                    nextNextBlock->prev = newBlock;
                }

                currentBlock->next = nextBlock;
                currentBlock->size = size;
            }
            else
            {
                if (nextBlock->next)
                {
                    header_t *nextNextBlock = (header_t *)nextBlock->next;
                    nextNextBlock->prev = currentBlock;
                }

                freeMem -= nextBlock->size;

                currentBlock->next = nextBlock->next;
                currentBlock->size += nextBlock->size + HEADER_SIZE;
            }
        }
        else
        {
            void *newPtr = ma(size);
            if (!newPtr)
                return 0;
            memcpy(newPtr, ptr, currentBlock->size);
            fr(ptr);
            return newPtr;
        }
    }

    return ptr;
}

void fr(void *ptr)
{
    header_t *currentBlock = (header_t *)(((uint8_t *)ptr) - HEADER_SIZE);
    header_t *prevBlock = (header_t *)currentBlock->prev;
    header_t *nextBlock = (header_t *)currentBlock->next;

    portSUSPEND_ALL;

    currentBlock->used = FREE;
    freeMem += currentBlock->size;

    if (prevBlock && !prevBlock->used)
    {
        prevBlock->next = currentBlock->next;
        prevBlock->size += currentBlock->size + HEADER_SIZE;
        freeMem += HEADER_SIZE;
        currentBlock = prevBlock;
        nextBlock->next = currentBlock;
    }

    if (nextBlock && !nextBlock->used)
    {
        currentBlock->next = nextBlock->next;
        nextBlock->prev = currentBlock;
        currentBlock->size = nextBlock->size + HEADER_SIZE;
        freeMem += HEADER_SIZE;
    }

    portRESUME_ALL;
}

size_t getFreeMem()
{
    return freeMem;
}
