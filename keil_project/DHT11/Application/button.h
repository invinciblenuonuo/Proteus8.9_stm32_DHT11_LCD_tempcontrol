#ifndef _BUTTON_H
#define _BUTTON_H
#include "main.h"
#include "gpio.h"

#define TOP_UP_Port      GPIOA
#define TOP_DOWN_Port    GPIOA
#define TOP_UP_Pin       GPIO_PIN_15
#define TOP_DOWN_Pin     GPIO_PIN_14

#define BOTTOM_UP_Port     GPIOA
#define BOTTOM_DOWN_Port   GPIOA
#define BOTTOM_UP_Pin      GPIO_PIN_13
#define BOTTOM_DOWN_Pin    GPIO_PIN_12

#define TOP_UP_IN				 HAL_GPIO_ReadPin(TOP_UP_Port, TOP_UP_Pin) //���޼�
#define TOP_DOWN_IN			 HAL_GPIO_ReadPin(TOP_DOWN_Port, TOP_DOWN_Pin)//���޼�

#define BOTTOM_UP_IN				 HAL_GPIO_ReadPin(BOTTOM_UP_Port, BOTTOM_UP_Pin)//���޼�
#define BOTTOM_DOWN_IN			 HAL_GPIO_ReadPin(BOTTOM_DOWN_Port,BOTTOM_DOWN_Pin)//���޼�


#define MODE_Port    GPIOA
#define MODE_Pin     GPIO_PIN_10
#define MODE_IN			 HAL_GPIO_ReadPin(MODE_Port, MODE_Pin)




#endif
