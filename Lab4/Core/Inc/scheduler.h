/*
 * scheduler.h
 *
 *  Created on: Nov 14, 2022
 *      Author: DELL
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

#define SCH_MAX_TASKS	40
#define TICK			10

typedef struct {
    // Pointer to the task (must be a 'void (void)' function)
	void ( * pTask)(void);
	// Delay (ticks) until the function will (next) be run
	uint32_t Delay;
	// Interval (ticks) between subsequent runs.
	uint32_t Period;
	// Incremented (by scheduler) when task is due to execute
	uint8_t RunMe;
	//This is a hint to solve the question below.
	uint32_t TaskID;
} sTask;

typedef struct {
	sTask TASK_QUEUE[SCH_MAX_TASKS];
	int numofTask;
} taskList;

taskList SCH_TASK_LIST;

void SCH_Init(void);
void SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
void SCH_Delete_Task(void);
void insert_to_list(sTask task);

#endif /* INC_SCHEDULER_H_ */
