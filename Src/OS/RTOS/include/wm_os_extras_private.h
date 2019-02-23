#ifndef WM_OS_EXTRAS_PRIVATE_H
#define WM_OS_EXTRAS_PRIVATE_H

#ifndef INC_FREERTOS_H
	#error "include FreeRTOS.h must appear in source files before include wm_os_extras.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "list.h"
#include "mpu_wrappers.h"

typedef struct QueueDefinition
{
	signed char *pcHead;				/*< Points to the beginning of the queue storage area. */
	signed char *pcTail;				/*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */

	signed char *pcWriteTo;				/*< Points to the free next place in the storage area. */
	signed char *pcReadFrom;			/*< Points to the last place that a queued item was read from. */

	xList xTasksWaitingToSend;				/*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
	xList xTasksWaitingToReceive;			/*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

	volatile unsigned portBASE_TYPE uxMessagesWaiting;/*< The number of items currently in the queue. */
	unsigned portBASE_TYPE uxLength;		/*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
	unsigned portBASE_TYPE uxItemSize;		/*< The size of each items that the queue will hold. */

	signed portBASE_TYPE xRxLock;			/*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
	signed portBASE_TYPE xTxLock;			/*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */

} xQUEUE;

typedef xQUEUE * xQueueHandle;

#ifdef __cplusplus
}
#endif
#endif /* WM_OS_EXTRAS_PRIVATE_H */



