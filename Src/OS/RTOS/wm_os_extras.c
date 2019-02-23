#include "FreeRTOS.h"
#include "wm_os_extras_private.h"

#ifndef queueUNLOCKED
#define queueUNLOCKED					( ( signed portBASE_TYPE ) -1 )
#endif

signed char * xQueueGetStoragePointer(xQueueHandle queue)
{
    return queue->pcHead;
}


//The purpose of encapsulating the interface is that the above interface queue address is internally applied, but the previous ucos interface is externally applied.
xQueueHandle xQueueCreateExt( void *QueueStart, unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize )
{
xQUEUE *pxNewQueue;
//size_t xQueueSizeInBytes;
xQueueHandle xReturn = NULL;

	/* Allocate the new queue structure. */
	if( uxQueueLength > ( unsigned portBASE_TYPE ) 0 )
	{
		pxNewQueue = ( xQUEUE * ) pvPortMalloc( sizeof( xQUEUE ) );
		if( pxNewQueue != NULL )
		{
			/* Create the list of pointers to queue items.  The queue is one byte
			longer than asked for to make wrap checking easier/faster. */
	//		xQueueSizeInBytes = ( size_t ) ( uxQueueLength * uxItemSize ) + ( size_t ) 1;

			pxNewQueue->pcHead = ( signed char * ) QueueStart;//pvPortMalloc( xQueueSizeInBytes );
			if( pxNewQueue->pcHead != NULL )
			{
				/* Initialise the queue members as described above where the
				queue type is defined. */
				pxNewQueue->pcTail = pxNewQueue->pcHead + ( uxQueueLength * uxItemSize );
				pxNewQueue->uxMessagesWaiting = ( unsigned portBASE_TYPE ) 0U;
				pxNewQueue->pcWriteTo = pxNewQueue->pcHead;
				pxNewQueue->pcReadFrom = pxNewQueue->pcHead + ( ( uxQueueLength - ( unsigned portBASE_TYPE ) 1U ) * uxItemSize );
				pxNewQueue->uxLength = uxQueueLength;
				pxNewQueue->uxItemSize = uxItemSize;
				pxNewQueue->xRxLock = queueUNLOCKED;
				pxNewQueue->xTxLock = queueUNLOCKED;

				/* Likewise ensure the event queues start with the correct state. */
				vListInitialise( &( pxNewQueue->xTasksWaitingToSend ) );
				vListInitialise( &( pxNewQueue->xTasksWaitingToReceive ) );

				traceQUEUE_CREATE( pxNewQueue );
				xReturn = pxNewQueue;
			}
			else
			{
				traceQUEUE_CREATE_FAILED();
				vPortFree( pxNewQueue );
			}
		}
	}

	configASSERT( xReturn );

	return xReturn;
}


void vQueueDeleteExt( xQueueHandle pxQueue )
{
	configASSERT( pxQueue );

	traceQUEUE_DELETE( pxQueue );
	vQueueUnregisterQueue( pxQueue );
//	vPortFree( pxQueue->pcHead );	//External release
	vPortFree( pxQueue );
}