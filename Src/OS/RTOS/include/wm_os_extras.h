#ifndef WM_OS_EXTRAS_H
#define WM_OS_EXTRAS_H

#ifndef INC_FREERTOS_H
	#error "include FreeRTOS.h must appear in source files before include wm_os_extras.h"
#endif

//#include "portable.h"
//#include "list.h"
#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif

//void portDISABLE_INTERRUPTS(void);
//void portENABLE_INTERRUPTS(void);

//void freeRtosTickIrqHandler( void );

#define xTaskCreateExt( pvTaskCode, pcName, puxStackBuffer,usStackDepth, pvParameters, uxPriority, pxCreatedTask ) xTaskGenericCreate( ( pvTaskCode ), ( pcName ), ( usStackDepth ), ( pvParameters ), ( uxPriority ), ( pxCreatedTask ), ( puxStackBuffer ), ( NULL ) )

unsigned portBASE_TYPE vTaskDeleteByPriority(unsigned portBASE_TYPE prio);


xQueueHandle xQueueCreateExt( void *QueueStart, unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize );
signed char * xQueueGetStoragePointer(xQueueHandle queue);
void vQueueDeleteExt( xQueueHandle pxQueue );

#ifdef __cplusplus
}
#endif
#endif /* WM_OS_EXTRAS_H */



