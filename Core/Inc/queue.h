
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_MAX_SIZE  16

#define QOPT_FAILED   0x00
#define QOPT_SUCCESS  0x01

typedef uint8_t OPT_RES;

typedef enum
{
	Event_Resever = 0x00,
	Event_Timer = 0x01,

	Event_Max = 0xFF,
}EVENT_ID;

typedef struct
{
	EVENT_ID evt_id;
	uint16_t evt_owner;
}EVENT_PARAM;

typedef struct
{
	EVENT_PARAM param[QUEUE_MAX_SIZE];
	uint16_t head;
	uint16_t tail;
}QUEUE;

OPT_RES isQueueFull(QUEUE *evt_queue);
OPT_RES isQueueEmpty(QUEUE *evt_queue);
OPT_RES EnQueue(QUEUE *evt_queue, EVENT_PARAM *param);
OPT_RES DeQueue(QUEUE *evt_queue, EVENT_PARAM *param);
OPT_RES PeekQueue(QUEUE *evt_queue, EVENT_PARAM *param);


enum
{
	xPosition_Front = 0x01,
	xPosition_Back  = 0x02,
};

typedef struct QueueBasic
{
	uint8_t *pHead;
	uint8_t *pTail;

	uint8_t *pWriteTo;
	uint8_t *pReadFrom;

	uint16_t uLength;
	uint16_t uItemSize;

	volatile uint16_t uxMessageWaitting;
}Queue_t;

typedef struct QueueBasic * QueueHandle_t;

#define QueueSend(xQueue, pvItemToQueue) \
		QueueGenericSend(xQueue, pvItemToQueue, xPosition_Back)

#define QueueSendFront(xQueue, pvItemToQueue) \
		QueueGenericSend(xQueue, pvItemToQueue, xPosition_Front)


QueueHandle_t QueueCreate(uint16_t QueueLength, uint16_t ItemSize);
OPT_RES QueueGenericSend(QueueHandle_t xQueue, const void * const pvItemToQueue, uint16_t xCopyPosition);
OPT_RES QueueReceive(QueueHandle_t xQueue, void * const pvItemToQueue);
OPT_RES QueuePeek(QueueHandle_t xQueue, void * const pvBuffer);
void QueueDelete(QueueHandle_t xQueue);

#endif


