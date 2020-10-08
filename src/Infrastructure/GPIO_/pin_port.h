#ifndef INFRASTRUCTURE_GPIO__PIN_PORT_H_
#define INFRASTRUCTURE_GPIO__PIN_PORT_H_

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_conf.h"

#include "../../application_interfaces.h"

APP_RESULT gpio_init_analog(uint32_t pin, GPIO_TypeDef *port);
APP_RESULT gpio_init_output_pp(uint32_t pin, GPIO_TypeDef *port);

#endif /* INFRASTRUCTURE_GPIO__PIN_PORT_H_ */
