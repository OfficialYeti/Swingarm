#include "adxl001.h"
#ifdef ADXL001_ENABLED

static int16_t to_int16(uint32_t val);
static ADXL001_DeviceTypeDef *get_device(uint8_t id);
static int8_t calculate_linear(ADXL001_DeviceTypeDef *dev, int16_t adc_value);

static ADXL001_DeviceTypeDef *devices[MAXIMUM_DEVICES];
static uint8_t devices_count;

extern UART_HandleTypeDef UART_Handle;

/*
 * Add configuration to device collection. This collection will be used in ADXL001_InitComponents(*)
 */
APP_RESULT ADXL001_AddDevice(ADXL001_DeviceTypeDef *device, PinPort *location)
{
	if (devices_count < MAXIMUM_DEVICES)
	{
		if (location->is_occupied == 1)
			return APP_OPERATION_ERROR;
		device->location = location;
		location->is_occupied = 1;
		devices[devices_count++] = device;
	} else
		return APP_OPERATION_ERROR;
	return APP_OK;
}

APP_RESULT ADXL001_InitComponents(void)
{
	if (devices_count < 0)
		return APP_INIT_ERROR;

	for (int i = 0; i < devices_count; i++)
	{
		if (devices[i] != NULL)
			GPIO_InitAnalog(devices[i]->location);
	}
	return APP_OK;
}

static ADXL001_DeviceTypeDef *get_device(uint8_t id)
{
	for (int n = 0; n < MAXIMUM_DEVICES - 1; n++)
	{
		if (devices[n]->channel == id)
			return devices[n];
	}
	// TODO: [RK] throw
	return NULL;
}

/*
 * Returns value calculated from converter function f(x)=ax+b, needs calibrate for each device before use
 */
static int8_t calculate_linear(ADXL001_DeviceTypeDef *dev, int16_t adc_value)
{
	return (dev->calib_a) * adc_value + (dev->calib_b);
}

static float calculate_voltage(int16_t adc_value)
{
	return adc_value * V_REF / (2 * CONVERSION_RANGE);
}

static void process(int16_t adc_value, uint32_t device_channel)
{
	float voltage = calculate_voltage(adc_value);
	int8_t gforce = voltage/SENSITIVITY;

#ifdef UART_LOGGER
	char msg[20];
	sprintf(msg, "%lu,%d", device_channel, gforce);
	while (HAL_UART_GetState(&UART_Handle) != HAL_UART_STATE_READY) {}
	if (HAL_UART_Transmit_DMA(&UART_Handle, (uint8_t *) msg, strlen(msg)) != HAL_OK)
	{
		Error_Handler();
	}
#endif
}

/*
 * Called by `HAL_SDADC_IRQHandler(*)` for given in "~/stm32f3xx_it.c" SDADC_HandleTypeDef pointer
 * Channel informations are in hsdadc->Instance property -> SDADC1 | SDADC2 | SDADC3
 * TODO: [RK] Consider what to do when another component will want to use this Callback
 * 			  Should somehow invert control.
 */
void HAL_SDADC_InjectedConvCpltCallback(SDADC_HandleTypeDef *hsdadc)
{
	/* fs = 16,6 kHz */
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
	if (hsdadc->Instance != ADXL001_SDADC)
		return;

	uint32_t device_channel = 0;
	int16_t adc_value = to_int16(HAL_SDADC_InjectedGetValue(hsdadc, &device_channel));

#ifdef CONTINOUOS_PIPELINE
	process(adc_value, device_channel);
#endif

#ifdef THRESHOLD_PIPELINE
	if (dev->samples_to_process > 0)
	{
		process(dev);
	}
	else if (temp > dev->treshold)
	{
		set_samples_to_process(dev);
		process(dev);
	}
#endif
}

// WIP
static void set_samples_to_process(ADXL001_DeviceTypeDef *dev)
{
	if (dev->samples_to_process > 0)
		return;
	ADXL001_DeviceTypeDef *tmp = dev->first;
	while (tmp != NULL)
	{
		tmp->samples_to_process = 10000;
		tmp = tmp->next;
	}
}
//

// TODO: [RK] Create separate module if number of such utility methods increase
// TODO: [RK] implement throwing
static int16_t to_int16(uint32_t val)
{
	if (val < INT16_MAX)
		return (int16_t) val;
	else
		return 0;
}

#endif
