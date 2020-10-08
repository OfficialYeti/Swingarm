#ifndef INFRASTRUCTURE_SDADC_SDADC_H_
#define INFRASTRUCTURE_SDADC_SDADC_H_

#include "../../application_interfaces.h"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_conf.h"

/**
 * Initialize SDADC3 Instance and bind to it given channels in differential mode
 */
APP_RESULT sdadc_init(uint32_t channels);

/**
 * Starts SDADC in injected mode, use HAL_SDADC_InjectedConvCpltCallback to handle end of conversion interrupt
 */
APP_RESULT sdadc_start(void);

#endif /* INFRASTRUCTURE_SDADC_SDADC_H_ */
