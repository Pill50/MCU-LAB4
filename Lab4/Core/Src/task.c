/*
 * task.c
 *
 *  Created on: Nov 14, 2022
 *      Author: DELL
 */

#include<task.h>
#include<main.h>

void initLED(void){ // Off
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 1);
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, 1);
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, 1);
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, 1);
	HAL_GPIO_WritePin(LED_PINK_GPIO_Port, LED_PINK_Pin, 1);
}

void task1(void){
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}

void task2(void){
	HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
}

void task3(void){
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}

void task4(void){
	HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
}

void task5(void){
	HAL_GPIO_TogglePin(LED_PINK_GPIO_Port, LED_PINK_Pin);
}
