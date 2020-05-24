#ifndef COMPONENTS_ADXL001__ADXL001_H_
#define COMPONENTS_ADXL001__ADXL001_H_

// TODO: [RK] Component should be independent from infrastructure. (initialization in ADC driver? Passing parameter?)
// TODO: [RK] Consider automating calibration function
// TODO: [RK] Consider if i should separate initialization and operation structure

#include "../../application_conf.h"
#include "../../application_interfaces.h"

#include "../../Infrastructure/GPIO_/pin_port.h"
#include "../../Infrastructure/SYSTEM_/error_handler.h"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_conf.h"
#include "string.h"

typedef struct adxl {
	uint8_t 			channel;
	struct adxl			*first;
	struct adxl			*next;

	float				calib_a;
	float				calib_b;
	uint16_t			treshold;	/* TODO: [RK] decide if it should be bit value or gforce value */
	uint32_t			samples_to_process;

	PinPort				*location;
} ADXL001_DeviceTypeDef;

APP_RESULT ADXL001_AddDevice(ADXL001_DeviceTypeDef *device, PinPort *location);
APP_RESULT ADXL001_InitComponents(void);

/* ##  MEASURE PIPELINES ## */
#define CONTINOUOS_PIPELINE

/* ## MEASURE CONFIG ## */
#define MAXIMUM_DEVICES				2
#define ADXL001_SDADC				SDADC3
#define V_REF						3.3f
#define SENSITIVITY					16.0f
#define CONVERSION_RANGE			32767

//#define LCD_LOGGER
#define UART_LOGGER

#endif /* COMPONENTS_ADXL001__ADXL001_H_ */
