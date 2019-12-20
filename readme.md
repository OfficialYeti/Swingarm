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
