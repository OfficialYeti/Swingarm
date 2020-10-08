#ifndef COMPONENTS_ADXL001__ADXL001_INTERNAL_H_
#define COMPONENTS_ADXL001__ADXL001_INTERNAL_H_

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_conf.h"

#include "../RING_BUFFER_/ring_buffer.h"

/* ## ADXL MEASUREMENT CONSTANTS ## */

#define MAXIMUM_DEVICES 2
#define V_REF 3.3f
#define BIT_RANGE 65534
#define ADC_mV_RESOLUTION 0.050356f /* V_REF * 1000 / BIT_RANGE */
#define ACC_SENSITIVITY 16.0f

#define USE_AVERAGING
#define AVERAGING_OVER 8

float adxl_get_gforce(int16_t adc_value);

/* ## ADXL MODELS ## */

typedef struct adxl
{
    uint8_t channel;

    float calib_a;
    float calib_b;

    uint32_t samples_to_process;
    uint16_t treshold;
} ADXL001_DeviceTypeDef;

typedef struct adxl_sample_t
{
    int16_t value;
    uint8_t channel;
} adxl_sample_t;

/* ## ADXL DEVICES HARDWARE MAPPING ## */

/*	PD10    ->	SDADC3_AIN4P	*/
/*	PD11    ->	SDADC3_AIN4M	*/
/*	PD8     ->	SDADC3_AIN6P	*/
/*	PD9     ->	SDADC3_AIN6M	*/

#define ADXL_DEVICES_SDADC_INSTANCE SDADC3

#define ADXL_DEVICE_SWINGARM_SDADC_CHANNEL SDADC_CHANNEL_6
#define ADXL_DEVICE_SWINGARM_SIGNAL_P GPIO_PIN_8
#define ADXL_DEVICE_SWINGARM_SIGNAL_N GPIO_PIN_9
#define ADXL_DEVICE_SWINGARM_PORT GPIOD

#define ADXL_DEVICE_STEERING_SDADC_CHANNEL SDADC_CHANNEL_4
#define ADXL_DEVICE_STEERING_SIGNAL_P GPIO_PIN_10
#define ADXL_DEVICE_STEERING_SIGNAL_N GPIO_PIN_11
#define ADXL_DEVICE_STEERING_PORT GPIOD

#endif /* COMPONENTS_ADXL001__ADXL001_INTERNAL_H_ */
