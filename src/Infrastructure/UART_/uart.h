/*
 * uart.h
 *
 *  Created on: 22.03.2020
 *      Author: Rafa³
 */

#ifndef INFRASTRUCTURE_UART__UART_H_
#define INFRASTRUCTURE_UART__UART_H_

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_conf.h"

/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_5
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_6
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Definition for USARTx's DMA */

#define USARTx_TX_DMA_CHANNEL             DMA1_Channel7
#define USARTx_RX_DMA_CHANNEL             DMA1_Channel6

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA1_Channel7_IRQn
#define USARTx_DMA_RX_IRQn                DMA1_Channel6_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA1_Channel7_IRQHandler
#define USARTx_DMA_RX_IRQHandler          DMA1_Channel6_IRQHandler

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

void UART_config(void);

#endif /* INFRASTRUCTURE_UART__UART_H_ */
