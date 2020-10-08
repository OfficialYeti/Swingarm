#include "pin_port.h"

static void enable_gpiox_clock(GPIO_TypeDef *gpiox);

APP_RESULT gpio_init_analog(uint32_t pin, GPIO_TypeDef *port)
{
	enable_gpiox_clock(port);

	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pin = pin;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	gpio.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(port, &gpio);

	return APP_OK;
}

APP_RESULT gpio_init_output_pp(uint32_t pin, GPIO_TypeDef *port)
{
	enable_gpiox_clock(port);

	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin = pin;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	gpio.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(port, &gpio);

	return APP_OK;
}

static void enable_gpiox_clock(GPIO_TypeDef *gpiox)
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