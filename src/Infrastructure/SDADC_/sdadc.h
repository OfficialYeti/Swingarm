#ifndef INFRASTRUCTURE_SDADC_SDADC_H_
#define INFRASTRUCTURE_SDADC_SDADC_H_

#include "../../application_interfaces.h"
#include "../GPIO_/pin_port.h"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_conf.h"

// TODO: [RK] Add initialized property
// TODO: [RK] Add getter for location which check if struct has been initialized
typedef struct {
	uint32_t		    channel_id;
	uint8_t				is_differential;

	PinPort				*location;
} SDADC_SlotTypeDef;

SDADC_HandleTypeDef* SDADC3_InitInstance(void);
APP_RESULT SDADC_InitDifferentialConfiguration(SDADC_HandleTypeDef* handle);
APP_RESULT SDADC_InitChannelsDifferentialy(SDADC_HandleTypeDef* handle, uint32_t channels);

#define DIFFERENTIAL_CONF_INDEX		SDADC_CONF_INDEX_0

#endif /* INFRASTRUCTURE_SDADC_SDADC_H_ */
