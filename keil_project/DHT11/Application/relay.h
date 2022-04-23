#ifndef _RELAY_H
#define _RELAY_H
#include "main.h"
#include "gpio.h"

#define Relay_Port      GPIOC
#define WET_UP_Pin      GPIO_PIN_6
#define WET_DOWN_Pin    GPIO_PIN_7
#define TEMP_UP_Pin     GPIO_PIN_8
#define TEMP_DOWN_Pin   GPIO_PIN_9

#define WET_UP_OFF		  HAL_GPIO_WritePin(Relay_Port, WET_UP_Pin, GPIO_PIN_SET) 
#define WET_UP_ON	      HAL_GPIO_WritePin(Relay_Port, WET_UP_Pin, GPIO_PIN_RESET)

#define WET_DOWN_OFF		HAL_GPIO_WritePin(Relay_Port, WET_DOWN_Pin, GPIO_PIN_SET)
#define WET_DOWN_ON   	HAL_GPIO_WritePin(Relay_Port, WET_DOWN_Pin, GPIO_PIN_RESET)

#define TEMP_UP_OFF		  HAL_GPIO_WritePin(Relay_Port, TEMP_UP_Pin, GPIO_PIN_SET)
#define TEMP_UP_ON	    HAL_GPIO_WritePin(Relay_Port, TEMP_UP_Pin, GPIO_PIN_RESET)
 
#define TEMP_DOWN_OFF		HAL_GPIO_WritePin(Relay_Port, TEMP_DOWN_Pin, GPIO_PIN_SET)
#define TEMP_DOWN_ON  	HAL_GPIO_WritePin(Relay_Port, TEMP_DOWN_Pin, GPIO_PIN_RESET)





#endif
