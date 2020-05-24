#include "sdadc.h"

SDADC_HandleTypeDef SDADC3_Handle;
SDADC_ConfParamTypeDef differential_config;

SDADC_HandleTypeDef* SDADC3_InitInstance(void)
{
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
	/*##-1- Enable SDADC clock #################################################*/
	/* Configure Clock prescaler : The SDADC should run @6MHz */
	/* If Sysclk is 72MHz, SDADC divider should be 12 */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDADC;
	PeriphClkInitStruct.SdadcClockSelection  = RCC_SDADCSYSCLK_DIV12;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
	}

	__HAL_RCC_SDADC3_CLK_ENABLE();

	/*##-2- Power On SDADC  ####################################################*/
	HAL_PWREx_EnableSDADC(PWR_SDADC_ANALOG3);

	/*##-3- Enable the SDADC interrupt  ########################################*/
	HAL_NVIC_SetPriority(SDADC3_IRQn, 0x01, 0);
	HAL_NVIC_EnableIRQ(SDADC3_IRQn);

	SDADC3_Handle.Instance = SDADC3;
	SDADC3_Handle.Init.ReferenceVoltage = SDADC_VREF_EXT;
	SDADC3_Handle.Init.IdleLowPowerMode = SDADC_LOWPOWER_NONE;
	SDADC3_Handle.Init.FastConversionMode = SDADC_FAST_CONV_DISABLE;
	SDADC3_Handle.Init.SlowClockMode = SDADC_SLOW_CLOCK_DISABLE;

	if (HAL_SDADC_Init(&SDADC3_Handle) != HAL_OK)
	{
	}

	return &SDADC3_Handle;
}

APP_RESULT SDADC_InitDifferentialConfiguration(SDADC_HandleTypeDef* handle)
{
	differential_config.CommonMode = SDADC_COMMON_MODE_VSSA;
	differential_config.Gain = SDADC_GAIN_1;
	differential_config.InputMode = SDADC_INPUT_MODE_DIFF;
	differential_config.Offset = 0;

	if (HAL_SDADC_PrepareChannelConfig(handle, DIFFERENTIAL_CONF_INDEX, &differential_config) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	return APP_OK;
}

// TODO: [RK] Provide logic for single-ended mode
APP_RESULT SDADC_InitChannelsDifferentialy(SDADC_HandleTypeDef* handle, uint32_t channels)
{
	if (HAL_SDADC_SelectInjectedTrigger(handle, SDADC_SOFTWARE_TRIGGER) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}
	if (HAL_SDADC_AssociateChannelConfig(handle, channels, DIFFERENTIAL_CONF_INDEX) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}
	if (HAL_SDADC_InjectedConfigChannel(handle, channels, SDADC_CONTINUOUS_CONV_OFF) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}
	/*## SEQ_1 for conf1 SEQ_2 for conf1, conf2 etc.. #######################*/
	if (HAL_SDADC_CalibrationStart(handle, SDADC_CALIBRATION_SEQ_1) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}
	if (HAL_SDADC_PollForCalibEvent(handle, HAL_MAX_DELAY) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	if (HAL_SDADC_InjectedConfigChannel(handle, channels, SDADC_CONTINUOUS_CONV_ON) != HAL_OK)
	{
		return APP_INIT_ERROR;
	}

	return APP_OK;
}

