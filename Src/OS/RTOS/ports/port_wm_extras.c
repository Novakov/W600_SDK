#include <stdlib.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"


__attribute__ ((naked)) void portDISABLE_INTERRUPTS_WM( void )
{
	__asm volatile(
					"push { r0 }				 \n"
					"mrs r0,PRIMASK              \n"
					"CPSID I                     \n"
					"pop { r0 }                  \n"
					"bx r14                      \n"
				);
}

// /*-----------------------------------------------------------*/

__attribute__ ((naked)) void portENABLE_INTERRUPTS_WM( void )
{
	__asm volatile (
					" push { r0 }			\n"
					" MRS r0,PRIMASK        \n"
					" CPSIE I               \n"
					" pop { r0 }            \n"
					" bx r14                \n"
				);
}

int portGET_IPSR(void)
{
	int result;
	
  __asm volatile ("MRS %0, ipsr" : "=r" (result) );
  
  return (result);
}

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskame, signed char prio )
{
	printf("\ntask[%s] prio[%d]stack over flow\n", pcTaskame, prio);
}
