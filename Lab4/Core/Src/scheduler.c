/*
 * scheduler.c
 *
 *  Created on: Nov 14, 2022
 *      Author: DELL
 */

#include "main.h"
#include "task.h"
#include "scheduler.h"

void SCH_Init() {
	SCH_TASK_LIST.numofTask = 0;
	for(int index = 0; index < SCH_MAX_TASKS; index++) {
		SCH_TASK_LIST.TASK_QUEUE[index].Delay = 0;
		SCH_TASK_LIST.TASK_QUEUE[index].Period = 0;
		SCH_TASK_LIST.TASK_QUEUE[index].RunMe = 0;
		SCH_TASK_LIST.TASK_QUEUE[index].pTask = 0;
	}
}

void insert_to_list(sTask task) {
	// check if list if empty
	if (SCH_TASK_LIST.numofTask == 0) {
		SCH_TASK_LIST.TASK_QUEUE[0] = task;
		SCH_TASK_LIST.numofTask += 1;
		return;
	} else {
		int pos = 0;
		int prev = SCH_TASK_LIST.TASK_QUEUE[0].Delay; // buffer save sum of delay of all tasks before new task
		int sum = SCH_TASK_LIST.TASK_QUEUE[0].Delay;  // buffer save sum of delay of all tasks before new task and new task
		// loof until find the position
		while (pos < SCH_TASK_LIST.numofTask && task.Delay > sum) {
			pos++;
			if (pos < SCH_TASK_LIST.numofTask) {
				prev = sum;
				sum += SCH_TASK_LIST.TASK_QUEUE[pos].Delay;
			}
		}
		// add to rear and update delay of new task
		if (pos == SCH_TASK_LIST.numofTask) {
			task.Delay -= sum;
			SCH_TASK_LIST.TASK_QUEUE[pos] = task;
			SCH_TASK_LIST.numofTask += 1;
			return;
		}
		// check if add to front or middle
		else {
			for (int i = SCH_TASK_LIST.numofTask; i > pos; i--) {
				SCH_TASK_LIST.TASK_QUEUE[i] = SCH_TASK_LIST.TASK_QUEUE[i - 1];
			}
			// add to front
			if (pos == 0) {
				SCH_TASK_LIST.TASK_QUEUE[pos] = task;
				sum = 0;
				prev = task.Delay;
			}
			// add to middle
			else {
				task.Delay -= prev;
				sum = prev;
				prev += task.Delay;
				SCH_TASK_LIST.TASK_QUEUE[pos] = task;
			}
			// update delay of index[POS+1]
			sum += SCH_TASK_LIST.TASK_QUEUE[pos + 1].Delay;
			SCH_TASK_LIST.TASK_QUEUE[pos + 1].Delay = sum - prev;
			SCH_TASK_LIST.numofTask += 1;
		}
	}
}
void SCH_Add_Task(void (*pFunction)(), uint32_t delay, uint32_t period) {
	// check if numofTask is overflow
	if (SCH_TASK_LIST.numofTask >= SCH_MAX_TASKS) {
		return;
	}
	// create new task and insert to queue
	sTask temp;
	temp.pTask = pFunction;
	temp.Delay = delay / TICK;
	temp.Period = period / TICK;
	temp.RunMe = 0;
	insert_to_list(temp);
}

void SCH_Update() {
	// check if the list is empty
	if (!SCH_TASK_LIST.TASK_QUEUE[0].pTask) {
		return;
	}
	else {
		if (SCH_TASK_LIST.TASK_QUEUE[0].Delay == 0) {
			// The task is due to run
			// Inc. the "RunMe" flag
			SCH_TASK_LIST.TASK_QUEUE[0].RunMe += 1;
			if (SCH_TASK_LIST.TASK_QUEUE[0].Period) {
				// Schedule periodic tasks to run again
				SCH_TASK_LIST.TASK_QUEUE[0].Delay = SCH_TASK_LIST.TASK_QUEUE[0].Period;
			}
		}
		// Not yet ready to run: just decrement the delay
		else SCH_TASK_LIST.TASK_QUEUE[0].Delay -= 1;
	}
}

void SCH_Delete_Task() {
	int index = 0;
	// "flag" check to add task into queue to run again if it has period
	int add_back_flag = 0;
	sTask temp;
	// check one-shot task
	if (SCH_TASK_LIST.TASK_QUEUE[index].Period) {
		add_back_flag = 1;
		temp = SCH_TASK_LIST.TASK_QUEUE[index];
	}
	// shift left all tasks
	for (; index < SCH_TASK_LIST.numofTask - 1; index++) {
		SCH_TASK_LIST.TASK_QUEUE[index] = SCH_TASK_LIST.TASK_QUEUE[index + 1];
	}
	// delete task rear after shift left
	SCH_TASK_LIST.TASK_QUEUE[index].pTask = 0x0000;
	SCH_TASK_LIST.TASK_QUEUE[index].Delay = 0;
	SCH_TASK_LIST.TASK_QUEUE[index].Period = 0;
	SCH_TASK_LIST.TASK_QUEUE[index].RunMe = 0;
	SCH_TASK_LIST.numofTask -= 1;
	// check flag to add back into queue
	if (add_back_flag == 1) {
		insert_to_list(temp);
	}
}

void SCH_Dispatch_Tasks() {
	// Dispatches (runs) the next task (if one is ready)
	for(int index = 0; index < SCH_MAX_TASKS; index++) {
		if (SCH_TASK_LIST.TASK_QUEUE[index].RunMe > 0) {
			// Run the task
			(*SCH_TASK_LIST.TASK_QUEUE[index].pTask)();
			// Reset / reduce RunMe flag
			SCH_TASK_LIST.TASK_QUEUE[index].RunMe -= 1;
			// schedule to delete task
			SCH_Delete_Task();
		}
	}
}
