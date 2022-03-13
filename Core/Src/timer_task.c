
#include "timer_task.h"
#include <stdio.h>

// static uint16_t timer_base_delay;

static volatile T_TIMER_TASK Timer_task_array[TIMER_TASK_MAX_LEN];

T_TIMER_ID timer_task_register(uint8_t mode, uint16_t delay_ms, timer_handle reg_handler)
{
    uint8_t index = 0x00;

    for(index = 0; index < TIMER_TASK_MAX_LEN; index++)
    {
        if(Timer_task_array[index].state == TIMER_INACTIVE)
        {
            Timer_task_array[index].timer_loop = mode;
            Timer_task_array[index].total_delay = delay_ms;
			Timer_task_array[index].func = reg_handler;
			return index;
        }
    }
	return TIMER_ID_INVALID;
}

T_TIMER_OPT_RES timer_task_start(T_TIMER_ID id)
{
	if(id > TIMER_TASK_MAX_LEN)
		return OPT_FAILED;

	if(Timer_task_array[id].state == TIMER_ACTIVE)
		return OPT_FAILED;
	
	Timer_task_array[id].state = TIMER_ACTIVE;
	return OPT_SUCCESS;
}

T_TIMER_OPT_RES timer_task_stop(T_TIMER_ID id)
{
	if(id > TIMER_TASK_MAX_LEN)
		return OPT_FAILED;	

	if(Timer_task_array[id].state == TIMER_INACTIVE)
		return OPT_FAILED;

	Timer_task_array[id].state = TIMER_INACTIVE;
	return OPT_SUCCESS;
}

T_TIMER_OPT_RES timer_task_pause(T_TIMER_ID id)
{
	if(id > TIMER_TASK_MAX_LEN)
		return OPT_FAILED;	

	if(Timer_task_array[id].state == TIMER_PAUSE)
		return OPT_FAILED;

	Timer_task_array[id].state = TIMER_PAUSE;
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
			if(Timer_task_array[index].func)
			{
				Timer_task_array[index].func(index, Timer_task_array[index].total_delay);
			}
			if(Timer_task_array[index].timer_loop == RUN_ONCE)
			{
				Timer_task_array[index].state = TIMER_INACTIVE;		
			}
			else if(Timer_task_array[index].timer_loop == RUN_LOOP)
			{
				Timer_task_array[index].cur_delay = 0x0000;
			}
		}
    }	
}

void timer_task_test(uint8_t id, uint16_t delay)
{
	printf("In NO.%d: %s\r\n", id, __func__);
}


