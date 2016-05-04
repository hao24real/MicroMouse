#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"

#define LED1_OFF    GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define LED1_ON   GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define LED2_OFF    GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define LED2_ON   GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define LED3_OFF    GPIO_SetBits(GPIOB, GPIO_Pin_14)
#define LED3_ON   GPIO_ResetBits(GPIOB, GPIO_Pin_14)
#define LED4_OFF    GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define LED4_ON   GPIO_ResetBits(GPIOB, GPIO_Pin_15)
#define LED5_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_6)
#define LED5_ON   GPIO_ResetBits(GPIOC, GPIO_Pin_6)
#define LED6_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_7)
#define LED6_ON   GPIO_ResetBits(GPIOC, GPIO_Pin_7)


#define FH_EM_ON   GPIO_SetBits(GPIOA, GPIO_Pin_6); \
                   GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define FH_EM_OFF  GPIO_ResetBits(GPIOA, GPIO_Pin_6); \
                   GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define FL_EM_ON   GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define FL_EM_OFF  GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define LD_EM_ON   GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define LD_EM_OFF  GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define RD_EM_ON   GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define RD_EM_OFF  GPIO_ResetBits(GPIOA, GPIO_Pin_5)

#define ALL_LED_OFF LED1_OFF; \
                    LED2_OFF; \
                    LED3_OFF; \
										LED4_OFF; \
										LED5_OFF; \
										LED6_OFF
										
										
#define ALL_LED_ON  LED1_ON; \
                    LED2_ON; \
										LED3_ON; \
										LED4_ON; \
										LED5_ON; \
										LED6_ON

void LED_Configuration(void);

#endif /* LED_H */
