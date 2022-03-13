
#ifndef __TIMER_TASK_H__
#define __TIMER_TASK_H__

#include "main.h"

#define OPT_FAILED   0x01
#define OPT_SUCCESS  0x00

#define TIMER_TASK_MAX_LEN  16

#define TIMER_INACTIVE  0x00
#define TIMER_ACTIVE    0x01
#define TIMER_PAUSE     0x02

#define TIMER_ID_INVALID 0xFF

typedef void (*timer_handle)(uint8_t id, uint16_t delay);
typedef uint8_t T_TIMER_ID;
typedef uint8_t T_TIMER_OPT_RES;

enum
{
    RUN_ONCE = 0x00,
    RUN_LOOP = 0x01,
};

typedef struct 
{
    uint8_t timer_id;
    uint8_t timer_loop;
    uint8_t state;
    uint16_t cur_delay;
    uint16_t total_delay;
    timer_handle func;
}T_TIMER_TASK;



T_TIMER_ID timer_task_register(uint8_t mode, uint16_t delay_ms, timer_handle);
void timer_task_operation(void);
void timer_task_test(uint8_t id, uint16_t delay);


#endif
