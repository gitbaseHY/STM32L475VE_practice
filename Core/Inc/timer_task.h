
#ifndef __TIMER_TASK_H__
#define __TIMER_TASK_H__

#include <stdint.h>

#define OPT_FAILED   0x01
#define OPT_SUCCESS  0x00

#define TIMER_TASK_MAX_LEN  16

#define TIMER_ID_AVALIABLE  0x00
#define TIMER_ID_TAKEN  0x01

#define TIMER_UPDATED  0x01
#define TIMER_CLEARED  0x00


typedef uint8_t TIMER_ID;
typedef uint8_t TIMER_OPT_RES;
typedef void (*timer_handle)(TIMER_ID id, uint16_t delay);

enum
{
	TIMER_INACTIVE = 0x00,
	TIMER_ACTIVE   = 0x01,
	TIMER_PAUSE    = 0x02,
	
	TIMER_ID_INVALID = 0xFF,
};

enum
{
    RUN_ONCE = 0x00,
    RUN_LOOP = 0x01,
};

typedef struct 
{
    uint8_t timer_id;
	union
	{
		volatile uint8_t timer_ctrl;
		struct
		{
			volatile uint8_t timer_loop: 2;
			volatile uint8_t state     : 2;
			volatile uint8_t take      : 2;
			volatile uint8_t update    : 2;
		};
	};
    volatile uint16_t cur_delay;
    uint16_t total_delay;
    timer_handle func;
}T_TIMER_TASK;


void timer_task_operation(void);
TIMER_ID timer_task_register(uint8_t mode, uint16_t delay_ms, timer_handle);
TIMER_OPT_RES timer_isUpdata(TIMER_ID id);
TIMER_OPT_RES timer_task_start(TIMER_ID id);
TIMER_OPT_RES timer_task_stop(TIMER_ID id);
TIMER_OPT_RES timer_task_pause(TIMER_ID id);
TIMER_OPT_RES timer_task_resume(TIMER_ID id);

void timer_task_test(TIMER_ID id, uint16_t delay);


#endif
