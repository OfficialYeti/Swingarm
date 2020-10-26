#ifndef COMPONENTS_ADXL001__ADXL001_H_
#define COMPONENTS_ADXL001__ADXL001_H_

#include "../application_conf.h"
#include "../application_interfaces.h"

#include "../Infrastructure/GPIO_/pin_port.h"
#include "../Infrastructure/SDADC_/sdadc.h"
#include "../Infrastructure/SYSTEM_/error_handler.h"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_conf.h"
#include "string.h"

typedef enum ADXL_MEASURE_MODE
{
	ADXL_MEASURE_SINGLE = 0,
	ADXL_MEASURE_AVERAGE,
	ADXL_MEASURE_CONTINOUOS,
	ADXL_MEASURE_THRESHOLD
} ADXL_MEASURE_MODE;

typedef struct adxl_result_t
{
	float gforce;
	uint8_t channel;
} adxl_result_t;

APP_RESULT adxl_init();
APP_RESULT adxl_start(ADXL_MEASURE_MODE mode);
APP_RESULT adxl_stop();
APP_RESULT adxl_run();

#endif /* COMPONENTS_ADXL001__ADXL001_H_ */
