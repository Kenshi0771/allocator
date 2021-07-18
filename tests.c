#include "allocator2.h"
#include <stdio.h>

#define HEAP_SIZE 1000
uint8_t pool[HEAP_SIZE];

int main()
{
    initHeap((uint8_t *)pool, HEAP_SIZE); // HEAP_SIZE - HEADER

    size_t waitingFreeMem = 0;
    //CASE1:
    waitingFreeMem = HEAP_SIZE - HEADER_SIZE;
    if (getFreeMem() != waitingFreeMem)
        printf("FAILED: CASE1\n");
    else
        printf("SUCCEED: CASE1\n");

    //CASE2:
    waitingFreeMem -= 100 + HEADER_SIZE;
    uint8_t *buf = (uint8_t *)ma(100);
    if (!buf || (getFreeMem() != waitingFreeMem))
        printf("FAILED: CASE2\n");
    else
        printf("SUCCEED: CASE2\n");

    //CASE3:
    uint8_t *buf2 = (uint8_t *)ma(200);
    waitingFreeMem -= 200 + HEADER_SIZE;
    if (!buf2 || (getFreeMem() != waitingFreeMem))
        printf("FAILED: CASE3\n");
    else
        printf("SUCCEED: CASE3\n");

    //CASE4:
    uint8_t *buf3 = (uint8_t *)ma(500);
    waitingFreeMem -= 500 + HEADER_SIZE;
    if (!buf3 || (getFreeMem() != waitingFreeMem))
        printf("FAILED: CASE4\n");
    else
        printf("SUCCEED: CASE4\n");

    //CASE5:
    fr(buf2); //free + 200 cause in middle
    waitingFreeMem += 200;
    if (getFreeMem() != waitingFreeMem)
        printf("FAILED: CASE5\n");
    else
        printf("SUCCEED: CASE5\n");

    //CASE6:
    buf2 = (uint8_t *)ma(10);
    waitingFreeMem -= 12 + HEADER_SIZE; // cause aligment
    if (!buf2 || (getFreeMem() != waitingFreeMem))
        printf("FAILED: CASE6\n");
    else
        printf("SUCCEED: CASE6\n");

    //CASE7:
    buf3 = (uint8_t *)re(buf3, 201);
    waitingFreeMem = waitingFreeMem + 500 - 204;
    if (!buf3 || (getFreeMem() != waitingFreeMem))
        printf("FAILED: CASE7\n");
    else
        printf("SUCCEED: CASE7\n");

    //CASE8:
    buf = (uint8_t *)re(buf, 100);
    if (!buf || (getFreeMem() != waitingFreeMem))
        printf("FAILED: CASE8\n");
    else
        printf("SUCCEED: CASE8\n");

    //CASE9:
    buf = (uint8_t *)re(buf, 52);
    waitingFreeMem = waitingFreeMem + 100 - 52 - HEADER_SIZE;
    if (!buf || (getFreeMem() != waitingFreeMem))
        printf("FAILED: CASE9\n");
    else
        printf("SUCCEED: CASE9\n");

    //CASE10:
    fr(buf3);
    waitingFreeMem += 204 + 2 * HEADER_SIZE; // cause in middle
    if (getFreeMem() != waitingFreeMem)
        printf("FAILED: CASE10\n");
    else
        printf("SUCCEED: CASE10\n");

    //CASE11:
    fr(buf);
    waitingFreeMem += 52 + HEADER_SIZE; // cause next is used
    if (getFreeMem() != waitingFreeMem)
        printf("FAILED: CASE11\n");
    else
        printf("SUCCEED: CASE11\n");

    //CASE12:
    fr(buf2);
    waitingFreeMem += 12 + 2 * HEADER_SIZE; // cause in middle and aligment
    if (getFreeMem() != waitingFreeMem)
        printf("FAILED: CASE12\n");
    else
        printf("SUCCEED: CASE12\n");

    return 0;
}