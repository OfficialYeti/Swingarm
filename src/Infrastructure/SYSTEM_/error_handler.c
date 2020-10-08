#include "error_handler.h"

void Error_Handler(void)
{
	while (1)
	{
		BSP_LED_Toggle(LED_BLUE);
		HAL_Delay(500);
	}
}
