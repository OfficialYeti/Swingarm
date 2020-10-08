#include "uart.h"

UART_HandleTypeDef UART_Handle;

static ring_buffer_descriptor_t _uart_bd;
static adxl_result_t _uart_rb_mem[128]; /* Consider allocation this buffer in adxl module and passing only buffer descriptor. */
__IO static uint8_t _uart_rb_lock = 0;  /* 0 - free, 1 - locked. Used to ommit HAL flag which requires interrupt disable when reading. */


uint8_t aTxStartMessage[] = "\n\r ****UART-Hyperterminal communication based on DMA****\n\r Enter 10 characters using keyboard :\n\r";



/**
 * @brief  Initialize buffer and put the USART peripheral in the Asynchronous mode (UART Mode).
 * @note   UART configured as follows:
 *           - Word Length = 9 Bits (8 data bit + 1 parity bit) :
 *		           BE CAREFUL : Program 8 data bits + 1 parity bit in PC HyperTerminal
 *           - Stop Bit    = One Stop bit
 *	         - Parity      = ODD parity
 *	         - BaudRate    = 115200 baud -- testing more
 *	         - Hardware flow control disabled (RTS and CTS signals)
 */
void uart_init(void)
{
  /* buffer init */
  ring_buffer_attr_t _uart_rb_attr = {
      .data = &_uart_rb_mem,
      .item_size = sizeof(_uart_rb_mem[0]),
      .length = (sizeof(_uart_rb_mem) / sizeof(_uart_rb_mem[0]))};
  ring_buffer_init(&_uart_bd, &_uart_rb_attr);

  /* uart init */
  UART_Handle.Instance = USARTx;

  UART_Handle.Init.BaudRate = 9600;
  UART_Handle.Init.WordLength = UART_WORDLENGTH_8B;
  UART_Handle.Init.StopBits = UART_STOPBITS_1;
  UART_Handle.Init.Parity = UART_PARITY_NONE;
  UART_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UART_Handle.Init.Mode = UART_MODE_TX_RX;

  /**
   * NOTE: Ue either 16 or 8 oversampling
   *       Max frequency for:
   *          - 8bit oversampling -> 9Mhz (900kB/s)
   *          - 16bit oversampling -> 4,5Mhz (450kB/s)
  */
  UART_Handle.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&UART_Handle) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  /*##-0- Declare models #################################*/
  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;

  GPIO_InitTypeDef GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /* Enable DMA clock */
  DMAx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the DMA ##################################################*/
  /* Configure the DMA handler for Transmission process */
  hdma_tx.Instance = USARTx_TX_DMA_CHANNEL;
  hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode = DMA_NORMAL;
  hdma_tx.Init.Priority = DMA_PRIORITY_LOW;

  HAL_DMA_Init(&hdma_tx);

  /* Associate the initialized DMA handle to the UART handle */
  __HAL_LINKDMA(huart, hdmatx, hdma_tx);

  /* Configure the DMA handler for reception process */
  hdma_rx.Instance = USARTx_RX_DMA_CHANNEL;
  hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_rx.Init.Mode = DMA_NORMAL;
  hdma_rx.Init.Priority = DMA_PRIORITY_HIGH;

  HAL_DMA_Init(&hdma_rx);

  /* Associate the initialized DMA handle to the the UART handle */
  __HAL_LINKDMA(huart, hdmarx, hdma_rx);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
  HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);

  /* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
  HAL_NVIC_SetPriority(USARTx_DMA_RX_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USARTx_DMA_RX_IRQn);

  /* NVIC configuration for USART, to catch the TX complete */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

void uart_transmit_start_msg(void)
{
	while (1)
	{
		if(HAL_UART_Transmit_DMA(&UART_Handle, (uint8_t*)aTxStartMessage, TXSTARTMESSAGESIZE)!= HAL_OK)
		{
			/* Transfer error in transmission process */
			Error_Handler();
		}

		while (HAL_UART_GetState(&UART_Handle) != HAL_UART_STATE_READY)
		{
		}
	}
}

/**
 * @brief  Put data into uart buffer and if uart is not busy start transmitting.
 * @note   "Also, you may want to disable the UART interrupt while you check if it is busy,
 *         and load the buffer, so that you avoid the reace condition
 *         where the interrupt gets triggered on the last char going out while you are updating the buffer."
 * @note   ring_buffer_get_tail_ptr(_uart_bd, tail) is used, becouse when transmitting by DMA
 *         our function is not waiting for transmission complete. It's like fire and forget.
 *         So temp variable will be destroyed at the end of function scope and memory could be overwrite.
 */
HAL_StatusTypeDef uart_put_dma(void *data)
{
  ring_buffer_put(_uart_bd, data);
  if (_uart_rb_lock == 0)
  {
    _uart_rb_lock = 1;
    void *tail = ring_buffer_get_tail_ptr(_uart_bd);
    if (tail != NULL)
      HAL_UART_Transmit_DMA(&UART_Handle, tail, sizeof(_uart_rb_mem[0]));
  }

  return HAL_OK;
}

/**
 * @brief
 *
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  ring_buffer_skip(_uart_bd);
  void *tail = ring_buffer_get_tail_ptr(_uart_bd);
  if (tail != NULL)
    HAL_UART_Transmit_DMA(&UART_Handle, tail, sizeof(_uart_rb_mem[0]));
  else
    _uart_rb_lock = 0;

  BSP_LED_Toggle(LED_GREEN);
}

/**
 * @brief
 *
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  BSP_LED_On(LED_ORANGE);
}

/**
 * @brief
 *
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  BSP_LED_On(LED_RED);
}
