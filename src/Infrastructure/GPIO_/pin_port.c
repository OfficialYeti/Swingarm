#include "pin_port.h"

static void enable_gpiox_clock(GPIO_TypeDef  *gpiox);

APP_RESULT GPIO_InitAnalog(PinPort *pin_port)
{
	enable_gpiox_clock(pin_port->port);

	GPIO_InitTypeDef gpio;

	gpio.Mode  = GPIO_MODE_ANALOG;
	gpio.Pin   = pin_port->pin;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	gpio.Pull  = GPIO_NOPULL;
	HAL_GPIO_Init(pin_port->port, &gpio);

	if (pin_port->coupled != NULL)
		return GPIO_InitAnalog(pin_port->coupled);

	return APP_OK;
}

static void enable_gpiox_clock(GPIO_TypeDef  *gpiox)
{
	if (gpiox == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	if (gpiox == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	if (gpiox == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	if (gpiox == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	if (gpiox == GPIOE)
		__HAL_RCC_GPIOE_CLK_ENABLE();
	if (gpiox == GPIOF)
		__HAL_RCC_GPIOF_CLK_ENABLE();
}

// PINPORTS

PinPort PD10 = {
		.pin = GPIO_PIN_10,
		.port = GPIOD,

		.coupled = &PD11
};
PinPort PD11 = {
		.pin = GPIO_PIN_11,
		.port = GPIOD,

		.coupled = NULL
};

PinPort PD9 = {
		.pin = GPIO_PIN_9,
		.port = GPIOD,

		.coupled = NULL
};
PinPort PD8 = {
		.pin = GPIO_PIN_8,
		.port = GPIOD,

		.coupled = &PD9
};
