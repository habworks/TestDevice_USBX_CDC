/** ****************************************************************************************************
 * @file 			UART.c
 * @brief			Functions for general use in ACI ARM Cortex M4 in accessing UART
 * ****************************************************************************************************
 * @author original	Hab Collector (habco)\n
 *
 * @version       	See Repo Branch notes
 *
 * @param Development_Environment \n
 * Hardware:		ARM Cortex M4 compatible with: STM32L4 devices\n
 * IDE:             STMCubeIDE\n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            Functions for general use in ACI ARM Cortex M4 in accessing UART
 * 					Intended for use as a submodule
 *
 * @copyright		Applied Concepts, Inc
 ********************************************************************************************************/

#include "UART.h"
#include <stdlib.h>
#include <string.h>
#include "tx_api.h"

#define MAX_UARTS			10
static UART * Registerd_UARTs[MAX_UARTS];
static int Number_of_UARTs = 0;


/*******************************************************************************************************
* @brief Ready the desire UART port for use.  By definition the UART is configured for DMA on a circular
* buffer.
*
* @author original: Hab Collector \n
*
* @param UART_Handle: The UART specific handle
* @param UART_TransmitFunction: The function pointer associated with transmission on said port
* @param UART_ReceiveFunction: The function pointer associated with receiving on said port
*
* @return A pointer to the UART handle on success or NULL on failure
*
* STEP 1: Allocate memory for the device handler
* STEP 2: Make ready the handler for use
********************************************************************************************************/
void Init_UART_DMA(UART * Bus, UART_HandleTypeDef *UART_Handle)
{
	Bus->Handle = UART_Handle;
	Bus->Enable = true;
	Bus->Rx_BufferTailPointer = 0;
	Bus->Is_Transmitting = false;

	if(Number_of_UARTs < MAX_UARTS)
		Registerd_UARTs[Number_of_UARTs++] = Bus;

	// Ready DMA to receive
	HAL_UART_Receive_DMA(Bus->Handle, Bus->Rx_Buffer, UART_RX_BUFFER_SIZE);
} // END OF Init_UART_DMA



/*******************************************************************************************************
* @brief Enable the UART for use.  If the UART has been disabled this function must be called before the
* UART can be used again.
*
* @author original: Hab Collector \n
*
* @param Bus: The UART specific handler
*
* @return void
*
* STEP 1: Init the UART and read the DMA to receive
********************************************************************************************************/
void UART_Enable(UART *Bus)
{
    // STEP 1: Init the UART and read the DMA to receive
    if (!Bus->Enable)
    {
        HAL_UART_MspInit(Bus->Handle);
        Bus->Enable = true;
        HAL_UART_Receive_DMA(Bus->Handle, Bus->Rx_Buffer, UART_RX_BUFFER_SIZE);
    }

} // END OF UART_Enable



/*******************************************************************************************************
* @brief Disable the UART.
*
* @author original: Hab Collector \n
*
* @param Bus: The UART specific handler
*
* @return void
*
* STEP 1: Disable UART if enabled
********************************************************************************************************/
void UART_Disable(UART *Bus)
{
    if (Bus->Enable)
    {
        HAL_UART_MspDeInit(Bus->Handle);
        Bus->Enable = true;
    }

} // END OF UART_Disable



/*******************************************************************************************************
* @brief UART Receive from DMA function, Receive Data via the UART DMA circular buffer
*
* @author original: Hab Collector \n
*
* @note: This essence of this function is copied from work originally done by Devin K. for use in the Devin OS.
* It is generalize and adapted here. Mostly just variable name changes
*
* @param Bus: The UART specific handler
* @param DataBuffer: Receive Buffer - user must allocated as necessary
* @param DataBufferLength: Number of chars received - if 0 there is nothing
*
* @return The number of bytes received
*
* STEP 1: See work done by Devin K. in handling Rx DMA circular buffer
********************************************************************************************************/
uint32_t UART_DMA_Receive(UART *Bus, uint8_t *DataBuffer, uint32_t *DataBufferLength)
{
    // STEP 1: See work done by Devin K. in handling Rx DMA circular buffer
    *DataBufferLength = 0;
    if(Bus->Handle->RxState == HAL_UART_STATE_BUSY_RX)
    {
        while(Bus->Rx_BufferTailPointer != (UART_RX_BUFFER_SIZE - Bus->Handle->hdmarx->Instance->CNDTR))
        {
            DataBuffer[(*DataBufferLength)++] = Bus->Rx_Buffer[Bus->Rx_BufferTailPointer++];

            if(Bus->Rx_BufferTailPointer >= UART_RX_BUFFER_SIZE)
                Bus->Rx_BufferTailPointer = 0;
        }
        return (int32_t)*DataBufferLength;
    }

    return(*DataBufferLength);

} // END OF UART_DMA_Receive


uint16_t UART_DMA_Transmit(UART *Bus, uint8_t *DataBuffer, uint16_t DataLength)
{
	if (Bus->Is_Transmitting)
	{
	    return(0);
	}

	uint16_t ReturnValueDataLength = 0;
	HAL_StatusTypeDef Status = HAL_UART_Transmit_DMA(Bus->Handle, DataBuffer, DataLength);
	if (Status == HAL_OK)
	{
		Bus->Is_Transmitting = true;
		ReturnValueDataLength = DataLength;
	}

	return(ReturnValueDataLength);
}

uint16_t UART_Transmit_String(UART * Bus, const char * str)
{
	return UART_DMA_Transmit(Bus, (uint8_t *)str, strlen(str));
}



// QUICK ACCESS FUNCTIONS:
bool is_UART_Enable(UART *Bus)
{
    return(Bus->Enable);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	// Find the UART structure based on the handle
	for(int i = 0; i < Number_of_UARTs; i++)
	{
		if(Registerd_UARTs[i]->Handle == huart)
		{
			Registerd_UARTs[i]->Is_Transmitting = false;
		}
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	// Find the UART structure based on the handle
	for(int i = 0; i < Number_of_UARTs; i++)
	{
		if(Registerd_UARTs[i]->Handle == huart)
		{
			// We have found the correct UART, reset the bus
			Registerd_UARTs[i]->Rx_BufferTailPointer = 0;
			Registerd_UARTs[i]->Is_Transmitting = false;
			HAL_DMA_Abort_IT(huart->hdmarx);
			HAL_UART_DMAStop(huart);
			HAL_UART_Receive_DMA(huart, Registerd_UARTs[i]->Rx_Buffer, UART_RX_BUFFER_SIZE);
		}
	}
}




