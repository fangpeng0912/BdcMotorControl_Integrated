#ifndef _RS232Rx_CANTx_CANRx_RS232Tx_H_
#define _RS232Rx_CANTx_CANRx_RS232Tx_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_usart.h"
#include "stdio.h"
#include "APP_Desired_RPM.h"

#define USART1_PORT GPIOA
#define USART1_TX_PIN GPIO_Pin_9
#define USART1_RX_PIN GPIO_Pin_10

extern volatile double desired_rpm;

void USART1_Init(void);
int fputc(int ch, FILE *file);

#endif
