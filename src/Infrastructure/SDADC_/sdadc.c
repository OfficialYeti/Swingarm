#include "sdadc.h"

#define DIFF_CHANNEL_CONFIG_INDEX SDADC_CONF_INDEX_0
SDADC_HandleTypeDef sdadc3_handle;
static APP_RESULT _sdadc_init_diff_config(SDADC_HandleTypeDef *handle);
static APP_RESULT _sdadc_bind_channels(uint32_t channels);

APP_RESULT sdadc_init(uint32_t channels)
{
	/* -1- Enable SDADC clock **********************************/
	/* Configure Clock prescaler : The SDADC should run @6MHz **/
	/* If Sysclk is 72MHz, SDADC divider should be 12 **********/
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDADC;
	PeriphClkInitStruct.SdadcClockSelection = RCC_SDADCSYSCLK_DIV12;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	__HAL_RCC_SDADC3_CLK_ENABLE();

	/* -2- Power On SDADC **************************************/
	HAL_PWREx_EnableSDADC(PWR_SDADC_ANALOG3);

	/* -3- Enable the SDADC interrupt **************************/
	HAL_NVIC_SetPriority(SDADC3_IRQn, 0x01, 0);
	HAL_NVIC_EnableIRQ(SDADC3_IRQn);

	/* -4- Define SDADC Instance *******************************/
	sdadc3_handle.Instance = SDADC3;
	sdadc3_handle.Init.ReferenceVoltage = SDADC_VREF_EXT;
	sdadc3_handle.Init.IdleLowPowerMode = SDADC_LOWPOWER_NONE;
	sdadc3_handle.Init.FastConversionMode = SDADC_FAST_CONV_DISABLE;
	sdadc3_handle.Init.SlowClockMode = SDADC_SLOW_CLOCK_DISABLE;

	/* -5- Initialize SDADC Instance **************************/
	if (HAL_SDADC_Init(&sdadc3_handle) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	/* -6- Bind channels to Instance **************************/
	return _sdadc_bind_channels(channels);
}

APP_RESULT sdadc_start()
{
	if (HAL_SDADC_InjectedStart_IT(&sdadc3_handle) != HAL_OK)
	{
		return APP_OPERATION_ERROR;
	}
	return APP_OK;
}

static APP_RESULT _sdadc_bind_channels(uint32_t channels)
{
	if (_sdadc_init_diff_config(&sdadc3_handle) != APP_OK)
	{
		return APP_INIT_ERROR;
	}

	if (HAL_SDADC_SelectInjectedTrigger(&sdadc3_handle, SDADC_SOFTWARE_TRIGGER) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	if (HAL_SDADC_AssociateChannelConfig(&sdadc3_handle, channels, DIFF_CHANNEL_CONFIG_INDEX) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	if (HAL_SDADC_InjectedConfigChannel(&sdadc3_handle, channels, SDADC_CONTINUOUS_CONV_OFF) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	/*## SEQ_1 for conf1 SEQ_2 for conf1, conf2 etc.. #######################*/
	if (HAL_SDADC_CalibrationStart(&sdadc3_handle, SDADC_CALIBRATION_SEQ_1) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	if (HAL_SDADC_PollForCalibEvent(&sdadc3_handle, HAL_MAX_DELAY) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	if (HAL_SDADC_InjectedConfigChannel(&sdadc3_handle, channels, SDADC_CONTINUOUS_CONV_ON) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}


	return APP_OK;
}

static APP_RESULT _sdadc_init_diff_config(SDADC_HandleTypeDef *handle)
{
	SDADC_ConfParamTypeDef differential_config;

	differential_config.CommonMode = SDADC_COMMON_MODE_VSSA;
	differential_config.Gain = SDADC_GAIN_1;
	differential_config.InputMode = SDADC_INPUT_MODE_DIFF;
	differential_config.Offset = 0;

	if (HAL_SDADC_PrepareChannelConfig(handle, DIFF_CHANNEL_CONFIG_INDEX, &differential_config) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	return APP_OK;
}
