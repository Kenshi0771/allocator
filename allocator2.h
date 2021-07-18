#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

struct Header
{
   void *prev;
   void *next;
   uint32_t size : 31;
   uint32_t used : 1;
} typedef header_t;

enum BlockState
{
   FREE = 0,
   USED
};

void initHeap(uint8_t *heap, size_t size);
void *ma(size_t size);
void *re(void *ptr, size_t size);
void fr(void *ptr);
size_t getFreeMem();

#define HEADER_SIZE sizeof(header_t)
#define ALIGMENT sizeof(size_t)