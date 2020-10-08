#include "adxl_internal.h"

static int8_t calculate_linear(ADXL001_DeviceTypeDef *dev, int16_t adc_value)
{
	return (dev->calib_a) * adc_value + (dev->calib_b);
}

static float calculate_milivoltage(int16_t adc_value)
{
	return adc_value * ADC_mV_RESOLUTION;
}

float adxl_get_gforce(int16_t adc_value)
{
	float m_voltage, gforce;

	m_voltage = calculate_milivoltage(adc_value);
	gforce = m_voltage / ACC_SENSITIVITY;

	return gforce;
}