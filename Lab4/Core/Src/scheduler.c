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
	// NO TASK IN LIST
	if (SCH_TASK_LIST.numofTask == 0) {
		SCH_TASK_LIST.TASK_QUEUE[0] = task;
		SCH_TASK_LIST.numofTask += 1;
		return;
	} else {
	// 1+ TASK(S) IN LIST
		int pos = 0;
		int prev = SCH_TASK_LIST.TASK_QUEUE[0].Delay;
		int sum = SCH_TASK_LIST.TASK_QUEUE[0].Delay;
		// LOOP UNTIL FIND THE RIGHT POSITION
		while (pos < SCH_TASK_LIST.numofTask && task.Delay > sum) {
			pos++;
			if (pos < SCH_TASK_LIST.numofTask) {
				prev = sum;
				sum += SCH_TASK_LIST.TASK_QUEUE[pos].Delay;
			}
		}
		if (pos == SCH_TASK_LIST.numofTask) {
			// ADD TO REAR -> update delay of new task only
			task.Delay -= sum;
			SCH_TASK_LIST.TASK_QUEUE[pos] = task;
			SCH_TASK_LIST.numofTask += 1;
			return;
		}
		else {
			// SHIFT RIGHT ELEMENTS
			for (int i = SCH_TASK_LIST.numofTask; i > pos; i--) {
				SCH_TASK_LIST.TASK_QUEUE[i] = SCH_TASK_LIST.TASK_QUEUE[i - 1];
			}
			if (pos == 0) {
				// ADD TO FRONT
				SCH_TASK_LIST.TASK_QUEUE[pos] = task;
				sum = 0;
				prev = task.Delay;
			} else {
				// ADD TO MIDDLE
				task.Delay -= prev;
				sum = prev;
				prev += task.Delay;
				SCH_TASK_LIST.TASK_QUEUE[pos] = task;
			}
			// update delay of [pos + 1]
			sum += SCH_TASK_LIST.TASK_QUEUE[pos + 1].Delay;
			SCH_TASK_LIST.TASK_QUEUE[pos + 1].Delay = sum - prev;

			SCH_TASK_LIST.numofTask += 1;
		}
	}
}
void SCH_Add_Task(void (*pFunction)(), uint32_t delay, uint32_t period) {
	// CHECK IF OVERFLOW
	if (SCH_TASK_LIST.numofTask >= SCH_MAX_TASKS) {
		return;
	}
	// CREATE A NEW TASK AND INSERT IT INTO LIST QUEUE
	sTask temp;
	temp.pTask = pFunction;
	temp.Delay = delay;
	temp.Period = period;
	temp.RunMe = 0;
	insert_to_list(temp);
}

void SCH_Update() {
	// CHECK IF LIST IS EMPTY
	if (!SCH_TASK_LIST.TASK_QUEUE[0].pTask) {
		return;
	}
	else {
		if (SCH_TASK_LIST.TASK_QUEUE[0].Delay == 0) {
			SCH_TASK_LIST.TASK_QUEUE[0].RunMe += 1;
			if (SCH_TASK_LIST.TASK_QUEUE[0].Period) {
				SCH_TASK_LIST.TASK_QUEUE[0].Delay = SCH_TASK_LIST.TASK_QUEUE[0].Period;
			}
		}
		else SCH_TASK_LIST.TASK_QUEUE[0].Delay -= 1;
	}
}

void SCH_Delete_Task() {
	int index = 0;
	int add_back_flag = 0;
	sTask temp;
	// CHECK TASK IS NOT ONE-SHOT
	if (SCH_TASK_LIST.TASK_QUEUE[index].Period) {
		add_back_flag = 1;
		temp = SCH_TASK_LIST.TASK_QUEUE[index];
	}
	// SHIFT LEFT ALL TASKS
	for (; index < (SCH_TASK_LIST.numofTask - 1); index++) {
		SCH_TASK_LIST.TASK_QUEUE[index] = SCH_TASK_LIST.TASK_QUEUE[index + 1];
	}
	// DELETE TASK REAR AFTER SHIFT LEFT
	SCH_TASK_LIST.TASK_QUEUE[index].pTask = 0x0000;
	SCH_TASK_LIST.TASK_QUEUE[index].Delay = 0;
	SCH_TASK_LIST.TASK_QUEUE[index].Period = 0;
	SCH_TASK_LIST.TASK_QUEUE[index].RunMe = 0;
	SCH_TASK_LIST.numofTask -= 1;
	if (add_back_flag == 1) {
		insert_to_list(temp);
	}
}

void SCH_Dispatch_Tasks() {
	for(int index = 0; index < SCH_MAX_TASKS; index++) {
		if (SCH_TASK_LIST.TASK_QUEUE[index].RunMe > 0) {
			(*SCH_TASK_LIST.TASK_QUEUE[index].pTask)();
			SCH_TASK_LIST.TASK_QUEUE[index].RunMe -= 1;
			SCH_Delete_Task();
		}
	}
}
