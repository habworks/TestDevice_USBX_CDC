/** ****************************************************************************************************
 * @file            UART.h
 * @brief           This is the Header file used to support UART.h
 * ****************************************************************************************************
 * @author original Hab Collector (habco)\n
 *
 * @version         See Repo Branch notes
 *
 * @param Development_Environment \n
 * Hardware:        ARM Cortex M4 compatible with: STM32L4 devices\n
 * IDE:             STMCubeIDE\n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            Functions for general use in ACI ARM Cortex M4 in accessing UART
 *                  Intended for use as a submodule
 *
 * @copyright       Applied Concepts, Inc
 ********************************************************************************************************/
#ifndef INC_THREADX_UART_H_
#define INC_THREADX_UART_H_

#ifdef __cplusplus
extern"C" {
#endif

#include "usart.h"
#include <main.h>
#include <stdint.h>
#include <stdbool.h>

// DEFINES
#define UART_RX_BUFFER_SIZE     256U


// TYPEDEFS AND ENUMS
typedef uint32_t (*Function_UART_Tx)(uint8_t *DataBuffer, uint32_t DataSize);
typedef uint32_t (*Function_UART_Rx)(uint8_t *DataBuffer, uint32_t *DataSize);
typedef struct
{
    UART_HandleTypeDef *    Handle;
    bool                    Enable;
    uint8_t                 Rx_Buffer[UART_RX_BUFFER_SIZE];
    uint32_t                Rx_BufferTailPointer;
    volatile bool					Is_Transmitting;
//    Function_UART_Tx        UART_Tx;
//    Function_UART_Rx        UART_Rx;
}UART;


// FUNCTION PROTOTYPES
void Init_UART_DMA(UART * Bus, UART_HandleTypeDef *UART_Handle);
uint32_t UART_DMA_Receive(UART *Bus, uint8_t *DataBuffer, uint32_t *DataLength);
uint16_t UART_DMA_Transmit(UART *Bus, uint8_t *DataBuffer, uint16_t DataLength);
uint16_t UART_Transmit_String(UART * Bus, const char * str);

#ifdef __cplusplus
}
#endif
#endif /* INC_THREADX_UART_H_ */
