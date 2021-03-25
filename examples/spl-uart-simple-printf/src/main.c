/**
  ******************************************************************************
  * @file UART1_Printf\main.c
  * @brief This file contains the main function for: retarget the C library printf
  *        /scanf functions to the UART1 example.
  * @author  MCD Application Team
  * @version V2.0.4
  * @date     26-April-2018
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_it.h"    /* SDCC patch: required by SDCC for interrupts */
#include "stdio.h"
/**
  * @addtogroup UART1_Printf
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef _RAISONANCE_
  #define PUTCHAR_PROTOTYPE int putchar (char c)
  #define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
  #define PUTCHAR_PROTOTYPE char putchar (char c)
  #define GETCHAR_PROTOTYPE char getchar (void)
#elif defined (_SDCC_)         /* SDCC patch: ensure same types as stdio.h */
  #if SDCC_VERSION >= 30605      // declaration changed in sdcc 3.6.5 (officially with 3.7.0)
    #define PUTCHAR_PROTOTYPE int putchar(int c)
    #define GETCHAR_PROTOTYPE int getchar(void)
  #else
    #define PUTCHAR_PROTOTYPE void putchar(char c)
    #define GETCHAR_PROTOTYPE char getchar(void)
  #endif 
#else /* _IAR_ */
  #define PUTCHAR_PROTOTYPE int putchar (int c)
  #define GETCHAR_PROTOTYPE int getchar (void)
#endif /* _RAISONANCE_ */
/* Private macro -------------------------------------------------------------*/
/* Some chips have UART1, but other chips only have UART2 and not UART1. 
 * We want this example to work on both types of chips, so we 
 * macro-define all the correct SPL functions to the default UART of the device. 
 * 
 * UART1 devices: STM8S208, STM8S207, STM8S007, STM8S103, STM8S003, STM8S001, STM8S903
 * STM8AF52Ax, STM8AF62Ax
 * UART2 devices (which do not have UART1): STM8S105, STM8S005, STM8AF626x
 * 
 * For the TX and RX pins, see chip datasheet.
 * For STM8S103 devices, this is e.g. TX=PD5, RX=PD6.
*/
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
#define UART_NAME "UART2"
#define UART_INIT UART2_Init
#define UART_DEINIT UART2_DeInit
#define UART_SENDDATA8 UART2_SendData8
#define UART_RECEIVEDATA8 UART2_ReceiveData8
#define UART_GETFLAGSTATUS UART2_GetFlagStatus
#define UART_FLAG_RXNE UART2_FLAG_RXNE
#define UART_FLAG_TXE UART2_FLAG_TXE
#define UART_WORDLENGTH_8D UART2_WORDLENGTH_8D
#define UART_STOPBITS_1 UART2_STOPBITS_1
#define UART_PARITY_NO UART2_PARITY_NO
#define UART_SYNCMODE_CLOCK_DISABLE UART2_SYNCMODE_CLOCK_DISABLE
#define UART_MODE_TXRX_ENABLE UART2_MODE_TXRX_ENABLE
#else
/* other boards have normal UART 1*/
#define UART_NAME "UART1"
#define UART_INIT UART1_Init
#define UART_DEINIT UART1_DeInit
#define UART_SENDDATA8 UART1_SendData8
#define UART_RECEIVEDATA8 UART1_ReceiveData8
#define UART_GETFLAGSTATUS UART1_GetFlagStatus
#define UART_FLAG_RXNE UART1_FLAG_RXNE
#define UART_FLAG_TXE UART1_FLAG_TXE
#define UART_WORDLENGTH_8D UART1_WORDLENGTH_8D
#define UART_STOPBITS_1 UART1_STOPBITS_1
#define UART_PARITY_NO UART1_PARITY_NO
#define UART_SYNCMODE_CLOCK_DISABLE UART1_SYNCMODE_CLOCK_DISABLE
#define UART_MODE_TXRX_ENABLE UART1_MODE_TXRX_ENABLE
#endif

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /*High speed internal clock prescaler: 1*/
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    
  UART_DEINIT();
  /* Configure either UART1 or UART2 per board definitions above */
  /* UART configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Receive and transmit enabled
        - UART1 Clock disabled
  */
  UART_INIT((uint32_t)115200, UART_WORDLENGTH_8D, UART_STOPBITS_1, UART_PARITY_NO,
              UART_SYNCMODE_CLOCK_DISABLE, UART_MODE_TXRX_ENABLE);

  /* Output a message on Hyperterminal using printf function */
  printf("\n" UART_NAME " Example :retarget the C library printf()/getchar() functions to the UART\n");

  while (1)
  {
    printf("Hello, world!\n");

    // simple wait ~1000ms @ 16MHz
    for (uint32_t i=0; i<1600000L; i++)
      __asm__("nop");

    /* you may also use blocking getchar() to get input */ 
    //char ans = getchar();
    //printf("%c", ans);  
  }
}

/**
  * @brief Retargets the C library printf function to the UART.
  * @param c Character to send
  * @retval char Character sent
  */
PUTCHAR_PROTOTYPE
{
  /* Write a character to the UART1 */
  UART_SENDDATA8(c);
  /* Loop until the end of transmission */
  while (UART_GETFLAGSTATUS(UART_FLAG_TXE) == RESET);

  return (c);
}

/**
  * @brief Retargets the C library scanf function to the USART.
  * @param None
  * @retval char Character to Read
  */
GETCHAR_PROTOTYPE
{
#ifdef _COSMIC_
  char c = 0;
#else
  int c = 0;
#endif
  /* Loop until the Read data register flag is SET */
  while (UART_GETFLAGSTATUS(UART_FLAG_TXE) == RESET);
    c = UART_RECEIVEDATA8();
  return (c);
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  (void) file;
  (void) line;
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
