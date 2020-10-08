#include "adxl001.h"
#include "adxl_internal.h"

#ifdef ADXL001_ENABLED

static ring_buffer_descriptor_t _adxl_bd;
static adxl_sample_t _buff_mem[256];

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

	/* init adc channels (binded to inputsF) */
	sdadc_init(ADXL_DEVICE_SWINGARM_SDADC_CHANNEL | ADXL_DEVICE_STEERING_SDADC_CHANNEL);

	/* init uart */
	uart_init();

	return APP_OK;
}

APP_RESULT adxl_start(ADXL_MEASURE_MODE mode)
{
	uart_transmit_start_msg();

	/* TODO: setup measure mode */
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
		return 0;

	/* put value converted to gforce into uart dma buffer */
	float gforce = adxl_get_gforce(sample.value);
	adxl_result_t result = {.gforce = gforce, .channel = sample.channel};
	uart_put_dma(&result);

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
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);

	uint32_t device_channel = 0;
	/* REVIEW - make sure that casting from uint32_t to int16_t works! */
	int16_t value = HAL_SDADC_InjectedGetValue(hsdadc, &device_channel);

	/** NOTE - Here i use a running average
	 *	       with a down counter and a logical bitwise shifting
	 *
	 *		   With uart baudrate 921600 i have ~90kB/s transfer
	 *		   so with 16,6kHz sampling rate and 8byte sample size
	 *				-> 133kB/s
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
