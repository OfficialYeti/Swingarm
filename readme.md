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


Injected conversions are always executed in scan mode, which means that each of the
selected channels are converted in series. The lowest selected channel is converted first,
followed immediately by the next higher selected channel until all the channels selected by
JCHG[8:0] have been converted.

Injected conversions can be launched by software or by a trigger. They are never
interrupted by regular conversions.
The regular channel is a selection of just one of the 9 channels. RCH[3:0] in the
SDADC_CR2 register indicates the selected channel.
Regular conversions can be launched only by software (not by a trigger). A sequence of
continuous regular conversions is temporarily interrupted when a injected conversion is
requested.

If just a single channel is selected in continuous mode (either by executing a regular
conversion or by executing a injected conversion with only one channel selected), the
sampling rate can be increased three fold by setting the FAST bit in the SDADC_CR2
register. The conversion of each channel normally requires 360 SDADC clock cycles (60 µs
at 6 MHz). In fast continuous mode (FAST=1), the first conversion takes still 360 SDADC
clocks, but then each subsequent conversion finishes in 120 SDADC clocks.
(tzn... wykorzystujac dwa sdadc oraz timer do synchronizacji mogę być trzykrotnie szybszy)


Przeczytaj w domku o odbiciu przy niezgodnej impedancji



BALL JOINT

in mcpherson lower ball joint is unloaded - follower