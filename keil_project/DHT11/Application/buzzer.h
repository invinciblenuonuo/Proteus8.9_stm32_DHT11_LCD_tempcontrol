#ifndef _BUZZER_H
#define _BUZZER_H
#include "main.h"
#include "gpio.h"

#define BUZZ_Port      GPIOA
#define BUZZ_Pin       GPIO_PIN_11

#define BUZZ_ON				HAL_GPIO_WritePin(BUZZ_Port, BUZZ_Pin, GPIO_PIN_SET)
#define BUZZ_OFF			HAL_GPIO_WritePin(BUZZ_Port, BUZZ_Pin, GPIO_PIN_RESET)




#endif
