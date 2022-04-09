
#include <stdio.h>
#include "queue.h"

OPT_RES isQueueFull(QUEUE *evt_queue)
{
	return ((evt_queue->tail + 1) % QUEUE_MAX_SIZE == evt_queue->head);
}

OPT_RES isQueueEmpty(QUEUE *evt_queue)
{
	return (evt_queue->head == evt_queue->tail);
}

OPT_RES EnQueue(QUEUE *evt_queue, EVENT_PARAM *param)
{
	if(isQueueFull(evt_queue))
	{
		return QOPT_FAILED;
	}

	evt_queue->tail = (evt_queue->tail + 1) % QUEUE_MAX_SIZE;
	evt_queue->param[evt_queue->tail] = *param;
	return QOPT_SUCCESS;
}

OPT_RES PeekQueue(QUEUE *evt_queue, EVENT_PARAM *param)
{
	if(isQueueEmpty(evt_queue))
	{
		return QOPT_FAILED;
	}

	*param = evt_queue->param[evt_queue->head + 1];
	return QOPT_SUCCESS;		
}

OPT_RES DeQueue(QUEUE *evt_queue, EVENT_PARAM *param)
{
	if(PeekQueue(evt_queue, param))
	{
		evt_queue->head = (evt_queue->head + 1) % QUEUE_MAX_SIZE;
		return QOPT_SUCCESS;	
	}
	
	return QOPT_FAILED;
}


QueueHandle_t QueueCreate(uint16_t QueueLength, uint16_t ItemSize)
{
	uint32_t TotalQueueSize = 0x00;
	uint8_t * QueueStroge = NULL;
	Queue_t * pNewQueue = NULL;

	if(QueueLength == 0x00 || ItemSize == 0x00)
	{
		return NULL;
	}

	TotalQueueSize = QueueLength * ItemSize;
	pNewQueue = (Queue_t* )malloc(sizeof(Queue_t) + TotalQueueSize);
	if(pNewQueue == NULL)
	{
		return NULL;
	}
	
	QueueStroge = (uint8_t* )pNewQueue + (sizeof(Queue_t));
	pNewQueue->uLength = QueueLength;
	pNewQueue->uItemSize = ItemSize;
	pNewQueue->pHead = QueueStroge;
	pNewQueue->pTail = pNewQueue->pHead + TotalQueueSize;
	pNewQueue->uxMessageWaitting = 0x00;

	pNewQueue->pWriteTo = pNewQueue->pHead;
	pNewQueue->pReadFrom = pNewQueue->pHead + (pNewQueue->uLength - 1) * pNewQueue->uItemSize;
	
	return pNewQueue;
}

OPT_RES QueueGenericSend(QueueHandle_t xQueue, const void * const pvItemToQueue, uint16_t xCopyPosition)
{
	if(xQueue == NULL || pvItemToQueue == NULL || xCopyPosition != xPosition_Back || xCopyPosition != xPosition_Front)  /* param is valid */
	{
		return QOPT_FAILED;
	}
	if(xQueue->uxMessageWaitting > xQueue->uLength)  /* Queue is full */
	{
		return QOPT_FAILED;
	}

	if(xCopyPosition == xPosition_Back)
	{
		memcpy(xQueue->pWriteTo, pvItemToQueue, xQueue->uItemSize);
		xQueue->pWriteTo += xQueue->uItemSize;
		if(xQueue->pWriteTo >= xQueue->pTail)
		{
			xQueue->pWriteTo = xQueue->pHead;
		}
		xQueue->uxMessageWaitting += 1;
	}
	else if(xCopyPosition == xPosition_Front)
	{
		memcpy(xQueue->pReadFrom, pvItemToQueue, xQueue->uItemSize);
		xQueue->pReadFrom -= xQueue->uItemSize;
		if(xQueue->pReadFrom < xQueue->pHead)
		{
			xQueue->pReadFrom = (xQueue->pTail - xQueue->uItemSize);
		}
		xQueue->uxMessageWaitting += 1;
	}

	return QOPT_SUCCESS;
}

OPT_RES QueueGenericReceive(QueueHandle_t xQueue, void * const pvItemToQueue)
{
	if(xQueue == NULL || pvItemToQueue == NULL)  /* param is valid */
	{
		return QOPT_FAILED;
	}
	if(xQueue->uxMessageWaitting <= 0)    /* Queue is empty */
	{
		return QOPT_FAILED;
	}	

	xQueue->pReadFrom += xQueue->uItemSize;
	if(xQueue->pReadFrom >= xQueue->pTail)
	{
		xQueue->pReadFrom = xQueue->pHead;
	}
	memcpy(pvItemToQueue, xQueue->pReadFrom, xQueue->uItemSize);
	xQueue->uxMessageWaitting -= 1;

	return QOPT_SUCCESS;
}

OPT_RES QueuePeek(QueueHandle_t xQueue, void * const pvBuffer)
{
	
}

void QueueDelete(QueueHandle_t xQueue)
{
	free(xQueue);
}





