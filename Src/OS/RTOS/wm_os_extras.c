#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void vApplicationStackOverflowHook( TaskHandle_t *pxTask, signed char *pcTaskame, signed char prio )
{
	printf("\ntask[%s] prio[%d]stack over flow\n", pcTaskame, prio);
}

void wmAssertCalled(const char * file, int line)
{
    printf("ASSERT in %s:%d\n", file, line);
}

void vApplicationMallocFailedHook()
{
    printf("Malloc failed\n");
}