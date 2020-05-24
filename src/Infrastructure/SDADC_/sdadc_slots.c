#include "sdadc_slots.h"
#include "../GPIO_/pin_port.h"

SDADC_SlotTypeDef SDADC3_CH4 = {
		.is_differential = 1,
		.channel_id = SDADC_CHANNEL_4,

		.location = &PD10
};

SDADC_SlotTypeDef SDADC3_CH6 = {
		.is_differential = 1,
		.channel_id = SDADC_CHANNEL_6,

		.location = &PD8
};

