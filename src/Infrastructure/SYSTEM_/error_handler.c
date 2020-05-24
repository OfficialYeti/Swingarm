/*
 * error_handler.c
 *
 *  Created on: 24.05.2020
 *      Author: Rafa³
 */

#include "error_handler.h"

void Error_Handler(void)
{
	while (1)
	{
		BSP_LED_Toggle(LED_BLUE);
		HAL_Delay(500);
	}
}
