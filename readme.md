-> Src
--> Infrastructure
--> Component
--> main.c
--> main.h
--> startup_stm32f373xc.s
--> stm32f3xx_it.c
--> stm32f3xx_it.h
--> stm32f3xx_hal_conf.h
--> system_stm32f3xx.c
------------------------------------------------------------

Infrastructure::
SDADC_ -> { Config, Init, ReadChannel }
SDIO_ -> { FatFs, SDDriver }

Component::
ADXL001_ -> component { , Init, Calculate }



##UART TEST
	char msg[16];
	int16_t counter = -16000;
	while (1)
	{
		sprintf(msg, "%d\r\n", counter);
		while (HAL_UART_GetState(&UART_Handle) != HAL_UART_STATE_READY) {}
		if (HAL_UART_Transmit_DMA(&UART_Handle, (uint8_t*) msg, strlen(msg)) != HAL_OK)
		{
			Error_Handler();
		}
		counter++;
	}