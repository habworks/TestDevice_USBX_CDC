/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_cdc_acm.c
  * @author  MCD Application Team
  * @brief   USBX Device CDC ACM applicative source file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "ux_device_cdc_acm.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "TestApp_USB_CDC.h"
#include "IO_Support.h"
#include "TerminalEmulatorSupport.h"

extern UART_HandleTypeDef huart2;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* the minimum baudrate */
#define MIN_BAUDRATE     9600
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
UX_SLAVE_CLASS_CDC_ACM *cdc_acm;
extern TX_EVENT_FLAGS_GROUP USB_EventFlag;

UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER CDC_VCP_LineCoding =
{
  115200, /* baud rate */
  0x00,   /* stop bits-1 */
  0x00,   /* parity - none */
  0x08    /* nb. of bits 8 */
};
UART_HandleTypeDef *uart_handler;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  USBD_CDC_ACM_Activate
  *         This function is called when insertion of a CDC ACM device.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_Activate(VOID *cdc_acm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ACM_Activate */
//  UX_PARAMETER_NOT_USED(cdc_acm_instance);
    cdc_acm = (UX_SLAVE_CLASS_CDC_ACM *)cdc_acm_instance;
    if (tx_event_flags_set(&USB_EventFlag, USB_EVENT_CDC_INSERTED, TX_OR) != TX_SUCCESS)
    {
      Error_Handler();
    }

    uart_handler = &huart2;

    /* Get default UART parameters */
    CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_baudrate = uart_handler->Init.BaudRate;

    /* Set UART data bit to 8 */
    CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_data_bit = 8;

    /* Get UART Parity */
    CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_parity = uart_handler->Init.Parity;

    /* Get UART StopBits */
    CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_stop_bit = uart_handler->Init.StopBits;

    /* Set device class_cdc_acm with default parameters */
    if (ux_device_class_cdc_acm_ioctl(cdc_acm, UX_SLAVE_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING,
                                    &CDC_VCP_LineCoding) != UX_SUCCESS)
    {
    Error_Handler();
    }

  /* USER CODE END USBD_CDC_ACM_Activate */

  return;
}

/**
  * @brief  USBD_CDC_ACM_Deactivate
  *         This function is called when extraction of a CDC ACM device.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_Deactivate(VOID *cdc_acm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ACM_Deactivate */
  UX_PARAMETER_NOT_USED(cdc_acm_instance);
  if (tx_event_flags_set(&USB_EventFlag, USB_EVENT_CDC_REMOVED, TX_OR) != TX_SUCCESS)
    {
      Error_Handler();
    }

  /* Reset the cdc acm instance */
    cdc_acm = UX_NULL;

  /* USER CODE END USBD_CDC_ACM_Deactivate */

  return;
}

/**
  * @brief  USBD_CDC_ACM_ParameterChange
  *         This function is invoked to manage the CDC ACM class requests.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_ParameterChange(VOID *cdc_acm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ACM_ParameterChange */
    UX_PARAMETER_NOT_USED(cdc_acm_instance);

      ULONG request;
      UX_SLAVE_TRANSFER *transfer_request;
      UX_SLAVE_DEVICE *device;

      /* Get the pointer to the device */
      device = &_ux_system_slave -> ux_system_slave_device;

      /* Get the pointer to the transfer request associated with the control endpoint */
      transfer_request = &device -> ux_slave_device_control_endpoint.ux_slave_endpoint_transfer_request;

      request = *(transfer_request -> ux_slave_transfer_request_setup + UX_SETUP_REQUEST);

      switch (request)
      {
        case UX_SLAVE_CLASS_CDC_ACM_SET_LINE_CODING :

          /* Get the Line Coding parameters */
          if (ux_device_class_cdc_acm_ioctl(cdc_acm, UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_CODING,
                                            &CDC_VCP_LineCoding) != UX_SUCCESS)
          {
            Error_Handler();
          }

          /* Check if baudrate < 9600) then set it to 9600 */
          if (CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_baudrate < MIN_BAUDRATE)
          {
            CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_baudrate = MIN_BAUDRATE;

          }
          else
          {

          }

          break;

        case UX_SLAVE_CLASS_CDC_ACM_GET_LINE_CODING :

          /* Set the Line Coding parameters */
          if (ux_device_class_cdc_acm_ioctl(cdc_acm, UX_SLAVE_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING,
                                            &CDC_VCP_LineCoding) != UX_SUCCESS)
          {
            Error_Handler();
          }

          break;

        case UX_SLAVE_CLASS_CDC_ACM_SET_CONTROL_LINE_STATE :
        default :
          break;
      }

  /* USER CODE END USBD_CDC_ACM_ParameterChange */

  return;
}

/* USER CODE BEGIN 1 */
VOID usbx_cdc_acm_write_thread_entry(ULONG thread_input)
{
      /* Private Variables */
      ULONG tx_actual_length;
      uint16_t Count = 0;
      char Message[30] = {0}; //"USBX Application Running!\r\n";

      while(1)
      {
          sprintf(Message, "USBX Running VComm %d\r\n", Count);
             ux_device_class_cdc_acm_write(cdc_acm, (UCHAR *)(Message), strlen(Message), &tx_actual_length);
             Count++;
             tx_thread_sleep(100);
      }
}

VOID usbx_cdc_acm_read_thread_entry(ULONG thread_input)
{
      /* Private Variables */
      ULONG rx_actual_length;
      uint8_t UserRxBuffer[64];
      /* Infinite Loop */
      while(1)
      {
             if(cdc_acm != UX_NULL)
             {
                   ux_device_class_cdc_acm_read(cdc_acm, (UCHAR *)UserRxBuffer, 64, &rx_actual_length);
                   switch(UserRxBuffer[rx_actual_length-1])
                   {
                   case '1':
                   {
                       TP9_SET();
                       printf("TP 9 ON\r\n");
                   }
                   break;
                   case '0':
                   {
                       TP9_CLR();
                       printf("TP 9 OFF\r\n");
                   }
                   break;
                   default:
                   {
                       terminal_SetBrightYellowForeground();
                       printf("%c", UserRxBuffer[rx_actual_length-1]);
                       terminal_SetDefaultForegroundColor();
                   }
                   } // END OF SWITCH
             }
      }
}



/* USER CODE END 1 */
