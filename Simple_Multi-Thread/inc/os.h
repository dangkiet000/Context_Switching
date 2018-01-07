#ifndef OS_H
#define OS_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "os_config.h"

typedef enum {
	OS_TASK_STATUS_IDLE = 1,
	OS_TASK_STATUS_ACTIVE
} os_task_status_t;

typedef struct {
	/* The stack pointer (sp) has to be the first element as it is located
	   at the same address as the structure itself (which makes it possible
	   to locate it safely from assembly implementation of PendSV_Handler).
	   The compiler might add padding between other structure elements. */
	volatile uint32_t sp;
	void (*handler)(void);
	volatile os_task_status_t status;
} os_task_t;

static struct {
	os_task_t tasks[OS_CONFIG_MAX_TASKS];
	volatile uint32_t current_task;
	uint32_t size;
} m_task_table;


typedef uint32_t os_stack_t;

static volatile uint32_t gul_NoOfSysTick;

extern volatile os_task_t *os_curr_task;
extern volatile os_task_t *os_next_task;

void os_init(void);
bool os_task_init(void (*handler)(void), os_stack_t *p_stack, uint32_t stack_size);
bool os_start(uint32_t systick_ticks);
static inline uint32_t Get_NoOfSysTick(void);
void SysTickDelay(uint32_t NoOfTickDelay);

#endif