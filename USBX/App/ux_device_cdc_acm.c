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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
UX_SLAVE_CLASS_CDC_ACM *cdc_acm;
extern TX_EVENT_FLAGS_GROUP USB_EventFlag;
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
  /* USER CODE END USBD_CDC_ACM_ParameterChange */

  return;
}

/* USER CODE BEGIN 1 */
VOID usbx_cdc_acm_write_thread_entry(ULONG thread_input)
{
      /* Private Variables */
      ULONG tx_actual_length;
      const uint8_t message[] = "USBX Application Running!\r\n";
      while(1)
      {
             ux_device_class_cdc_acm_write(cdc_acm, (UCHAR *)(message), sizeof(message), &tx_actual_length);
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
                   }
             }
      }
}
/* USER CODE END 1 */
