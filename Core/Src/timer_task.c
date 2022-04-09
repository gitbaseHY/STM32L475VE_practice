
#include <stdio.h>
#include "timer_task.h"

static volatile T_TIMER_TASK Timer_task_array[TIMER_TASK_MAX_LEN];

TIMER_OPT_RES timer_isUpdata(TIMER_ID id)
{
	if(Timer_task_array[id].update == TIMER_UPDATED)
	{
		Timer_task_array[id].update = TIMER_CLEARED;
		return TIMER_UPDATED;
	}
	return TIMER_CLEARED;
}

TIMER_ID timer_task_register(uint8_t mode, uint16_t delay_ms, timer_handle reg_handler)
{
    TIMER_ID index = 0x00;

    for(index = 0; index < TIMER_TASK_MAX_LEN; index++)
    {
        if(Timer_task_array[index].take != TIMER_ID_TAKEN)
        {
            Timer_task_array[index].timer_loop = mode;
            Timer_task_array[index].total_delay = delay_ms;
			Timer_task_array[index].func = reg_handler;
			Timer_task_array[index].take = TIMER_ID_TAKEN;
			Timer_task_array[index].update = TIMER_CLEARED;
			return index;
        }
    }
	return TIMER_ID_INVALID;
}

TIMER_OPT_RES timer_task_start(TIMER_ID id)
{
	if(id > TIMER_TASK_MAX_LEN || Timer_task_array[id].take != TIMER_ID_TAKEN)
		return OPT_FAILED;

	if(Timer_task_array[id].state == TIMER_ACTIVE)
		return OPT_FAILED;
	
	Timer_task_array[id].state = TIMER_ACTIVE;
	return OPT_SUCCESS;
}

TIMER_OPT_RES timer_task_stop(TIMER_ID id)
{
	if(id > TIMER_TASK_MAX_LEN)
		return OPT_FAILED;	

	if(Timer_task_array[id].state == TIMER_INACTIVE)
		return OPT_FAILED;

	Timer_task_array[id].state = TIMER_INACTIVE;
	Timer_task_array[id].take = TIMER_ID_AVALIABLE;
	return OPT_SUCCESS;
}

TIMER_OPT_RES timer_task_pause(TIMER_ID id)
{
	if(id > TIMER_TASK_MAX_LEN)
		return OPT_FAILED;	

	if(Timer_task_array[id].state == TIMER_PAUSE)
		return OPT_FAILED;

	Timer_task_array[id].state = TIMER_INACTIVE;
	return OPT_SUCCESS;	
}

TIMER_OPT_RES timer_task_resume(TIMER_ID id)
{
	if(id > TIMER_TASK_MAX_LEN)
		return OPT_FAILED;	

	if(Timer_task_array[id].state != TIMER_PAUSE)
		return OPT_FAILED;

	Timer_task_array[id].state = TIMER_ACTIVE;
	return OPT_SUCCESS;	
}

void timer_task_operation(void)
{
    uint8_t index = 0x00;

    for(index = 0; index < TIMER_TASK_MAX_LEN; index++)
    {
        if(Timer_task_array[index].state == TIMER_INACTIVE)
        {
			continue;
        }

		Timer_task_array[index].cur_delay++;

		if(Timer_task_array[index].cur_delay >= Timer_task_array[index].total_delay)
		{
			if(Timer_task_array[index].timer_loop == RUN_ONCE)
			{
				Timer_task_array[index].state = TIMER_INACTIVE;		
			}
			else if(Timer_task_array[index].timer_loop == RUN_LOOP)
			{
				Timer_task_array[index].cur_delay = 0x0000;
			}		
			if(Timer_task_array[index].func)
			{
				Timer_task_array[index].func(index, Timer_task_array[index].total_delay);
			}
			else
			{
				Timer_task_array[index].update = TIMER_UPDATED;
			}
		}
    }	
}

void timer_task_test(TIMER_ID id, uint16_t delay)
{
	printf("In NO.%d: %s\n", id, __func__);
}


