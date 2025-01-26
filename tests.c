#include "allocator2.h"
#include <stdio.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"

#define HEAP_SIZE 1000
uint8_t pool[HEAP_SIZE];

enum TestCase
{
    CASE1 = 1,
    CASE2,
    CASE3,
    CASE4,
    CASE5,
    CASE6,
    CASE7,
    CASE8,
    CASE9,
    CASE10,
    CASE11,
    CASE12,
    CASE_MAX,
};

int main()
{
    initHeap((uint8_t *)pool, HEAP_SIZE);

    size_t waitingFreeMem = 0;
    uint8_t* buf1;
    uint8_t* buf2 = NULL;
    uint8_t* buf3 = NULL;

    const size_t AL_SIZE_1 = 100;
    const size_t AL_SIZE_2 = 200;
    const size_t AL_SIZE_3 = 500;
    const size_t RE_SIZE_1 = 52;
    const size_t RE_SIZE_2 = 10;
    const size_t RE_SIZE_3 = 201;

    for(int currentTestCase = CASE1; currentTestCase < CASE_MAX;currentTestCase++)
    {
        bool err = false;
        switch (currentTestCase)
        {
            case CASE1:
            {
                waitingFreeMem = HEAP_SIZE - HEADER_SIZE;
                err = getFreeMem() != waitingFreeMem;
                break;
            }

            case CASE2:
            {
                size_t alSize = AL_SIZE_1;
                if(alSize % ALIGMENT)
                    alSize += ALIGMENT - alSize % ALIGMENT;
                waitingFreeMem -= alSize + HEADER_SIZE;
                buf1 = (uint8_t *)ma(alSize);
                err = (!buf1 || (getFreeMem() != waitingFreeMem));
                break;
            }

            case CASE3:
            {
                size_t alSize = AL_SIZE_2;
                buf2 = (uint8_t *)ma(alSize);
                waitingFreeMem -= alSize + HEADER_SIZE;
                if(alSize % ALIGMENT)
                    waitingFreeMem -= ALIGMENT - alSize % ALIGMENT;

                err = (!buf2 || (getFreeMem() != waitingFreeMem));
                break;
            }

            case CASE4:
            {
                size_t alSize = AL_SIZE_3;
                buf3 = (uint8_t *)ma(alSize);
                if(alSize % ALIGMENT)
                    alSize += ALIGMENT - alSize % ALIGMENT;
                waitingFreeMem -= alSize + HEADER_SIZE;
                err = (!buf3 || (getFreeMem() != waitingFreeMem));
                break;
            }

            case CASE5:
            {
                fr(buf2);
                waitingFreeMem += AL_SIZE_2;
                err = (getFreeMem() != waitingFreeMem);
                break;
            }

            case CASE6:
            {
                size_t alSize = 10;
                buf2 = (uint8_t *)ma(alSize);
                if(alSize % ALIGMENT)
                    alSize += ALIGMENT - alSize % ALIGMENT;
                waitingFreeMem -= alSize + HEADER_SIZE; 
                err = (!buf2 || (getFreeMem() != waitingFreeMem));
                break;
            }

            case CASE7:
            {
                size_t reSize = RE_SIZE_3;
                buf3 = (uint8_t *)re(buf3, reSize);
                size_t realFrSize = AL_SIZE_3;
                if(realFrSize % ALIGMENT)
                    realFrSize += ALIGMENT - realFrSize % ALIGMENT;

                if(reSize % ALIGMENT)
                    reSize += ALIGMENT - reSize % ALIGMENT;
                waitingFreeMem = waitingFreeMem + realFrSize - reSize;
                
                err = (!buf3 || (getFreeMem() != waitingFreeMem));
                break;
            }

            case CASE8:
            {
                size_t reSize = 100;
                buf1 = (uint8_t *)re(buf1, reSize);
              
                err = (!buf1 || (getFreeMem() != waitingFreeMem));
                break;
            }

            case CASE9:
            {
                size_t reSize = RE_SIZE_1;
                buf1 = (uint8_t *)re(buf1, reSize);
                size_t realFrSize = AL_SIZE_1;
                if(realFrSize % ALIGMENT)
                    realFrSize += ALIGMENT - realFrSize % ALIGMENT;

                if(reSize % ALIGMENT)
                    reSize += ALIGMENT - reSize % ALIGMENT;
                waitingFreeMem = waitingFreeMem + realFrSize - reSize - HEADER_SIZE;
                
                err = (!buf1 || (getFreeMem() != waitingFreeMem));
                break;
            }

            case CASE10:
            {
                size_t realFrSize = RE_SIZE_3;
                if(realFrSize % ALIGMENT)
                    realFrSize += ALIGMENT - realFrSize % ALIGMENT;
                waitingFreeMem += realFrSize + 2*HEADER_SIZE;
                fr(buf3);
                
                err =  getFreeMem() != waitingFreeMem;
                break;
            }

            case CASE11:
            {
                size_t realFrSize = RE_SIZE_1;
                if(realFrSize % ALIGMENT)
                    realFrSize += ALIGMENT - realFrSize % ALIGMENT;
                waitingFreeMem += realFrSize + HEADER_SIZE;
                fr(buf1);

                err =  getFreeMem() != waitingFreeMem;
                break;
            }

            case CASE12:
            {
                size_t realFrSize = RE_SIZE_2;
                if(realFrSize % ALIGMENT)
                    realFrSize += ALIGMENT - realFrSize % ALIGMENT;
                waitingFreeMem += realFrSize + 2*HEADER_SIZE;
                fr(buf2);
              
                err =  getFreeMem() != waitingFreeMem;
                break;
            }

            
            default: return 0;
        }


        if(err)
            printf("%sFAILED: CASE%d%s\n",KRED , currentTestCase, KNRM);
        else
            printf("%sSUCCEED: CASE%d%s\n",KGRN, currentTestCase, KNRM);
    }

    return 0;
}