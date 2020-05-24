#ifndef INFRASTRUCTURE_GPIO__PIN_PORT_H_
#define INFRASTRUCTURE_GPIO__PIN_PORT_H_

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_conf.h"

#include "../../application_interfaces.h"

typedef struct PinPort {
	uint32_t			pin;
	GPIO_TypeDef		*port;

	uint8_t				is_occupied;

	struct PinPort		*coupled;
} PinPort;

APP_RESULT GPIO_InitAnalog(PinPort *pin_port);

extern PinPort PD10;	/*		SDADC3_AIN4P	*/
extern PinPort PD11;	/*		SDADC3_AIN4M	*/

extern PinPort PD8;		/*		SDADC3_AIN6P	*/
extern PinPort PD9;		/*		SDADC3_AIN6M	*/


#endif /* INFRASTRUCTURE_GPIO__PIN_PORT_H_ */
