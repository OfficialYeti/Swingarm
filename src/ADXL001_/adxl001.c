#include "adxl001.h"
#include "adxl_internal.h"
#include "../Infrastructure/USB_/usb_device.h"

#ifdef ADXL001_ENABLED

static ring_buffer_descriptor_t _adxl_bd;
static adxl_sample_t _buff_mem[256];
static char result[10];

APP_RESULT adxl_init()
{
	/* init adc buffer */
	ring_buffer_attr_t buff_attr = {
		.item_size = sizeof(_buff_mem[0]),
		.length = sizeof(_buff_mem) / sizeof(_buff_mem[0]),
		.data = &_buff_mem};
	ring_buffer_init(&_adxl_bd, &buff_attr);

	/* init adc analog imput */
	gpio_init_analog(ADXL_DEVICE_SWINGARM_SIGNAL_P, ADXL_DEVICE_SWINGARM_PORT);
	gpio_init_analog(ADXL_DEVICE_SWINGARM_SIGNAL_N, ADXL_DEVICE_SWINGARM_PORT);
	gpio_init_analog(ADXL_DEVICE_STEERING_SIGNAL_P, ADXL_DEVICE_STEERING_PORT);
	gpio_init_analog(ADXL_DEVICE_STEERING_SIGNAL_N, ADXL_DEVICE_STEERING_PORT);

	/* init adc channels (binded to inputs) */
	sdadc_init(ADXL_DEVICE_SWINGARM_SDADC_CHANNEL | ADXL_DEVICE_STEERING_SDADC_CHANNEL);

	/* init pc comunnication */
	//	uart_init();
	USB_DEVICE_Init();

	return APP_OK;
}

APP_RESULT adxl_start(ADXL_MEASURE_MODE mode)
{
	/* TODO: setup measure mode
	 * 		 handle comunication from pc to board */
	sdadc_start();

	return APP_OK;
}

/**
 * @brief	Main loop execution. Get sample from adxl buffer, calculate gforce, pass it to uart
 */
APP_RESULT adxl_run()
{
	/* get sample from adc buffer */
	adxl_sample_t sample = {.value = -1, .channel = 0};
	if (ring_buffer_pop(_adxl_bd, &sample) == -1)
		return APP_IDLE;

	/* send value converted to gforce */
	/* consider sending binary data */
	float gforce = adxl_get_gforce(sample.value);
	size_t length = sprintf(result, "%d:%3.3f", sample.channel, gforce);
	if (CDC_Transmit_FS(result, length) == USBD_BUSY)
		return APP_BUSY;

	return APP_OK;
}

/**
 * @brief	Called by `HAL_SDADC_IRQHandler(*)` for given in "~/stm32f3xx_it.c" SDADC_HandleTypeDef pointer
 *		  	Instance informations are in hsdadc->Instance property -> SDADC1 | SDADC2 | SDADC3
 *			TODO: Use dma also for adc interrupt
 * @note	fs = 16,6kHz
 */
void HAL_SDADC_InjectedConvCpltCallback(SDADC_HandleTypeDef *hsdadc)
{
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10); /* measure frequency */

	uint32_t device_channel = 0;
	int16_t value = HAL_SDADC_InjectedGetValue(hsdadc, &device_channel);

	/** NOTE - Running Average with down counter for defined constant
	 * 		   Move to separate function
	**/
	static uint8_t count = AVERAGING_OVER;
	static int32_t first_channel_sum = 0;
	static int32_t second_channel_sum = 0;

	if (count > 0)
	{
		if (device_channel == 4)
			first_channel_sum += value;
		else
			second_channel_sum += value;

		count--;
	}
	else
	{
		int16_t value_avg = device_channel == 4 ? (first_channel_sum / AVERAGING_OVER) : (second_channel_sum / AVERAGING_OVER);
		adxl_sample_t sample = {.value = value_avg, .channel = device_channel};
		ring_buffer_put(_adxl_bd, &sample);

		count = AVERAGING_OVER;
		first_channel_sum = 0;
		second_channel_sum = 0;
	}
}
#endif
