/******************************************************************************************************
 * @file            Init_App.c
 * @brief           Functions that handle the basic init for the application
 * ****************************************************************************************************
 * @author original Hab Collector (habco)\n
 *
 * @version         MAJOR_VERSION, MINOR_VERSION, SUB_VERSION, TEST_VERSION
 *
 * @param Development_Environment \n
 * Hardware:        STM32L4R5VGTx\n
 * IDE:             STMCubeIDE \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            Init of ThreadX, FileX, USBX file services, hardware, etc.
 *
 *                  This is an embedded application
 *                  It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *                    Schematic 002-5791-00
 *                    Spokane_Application
 *                    Design Document
 *
 * @copyright       Applied Concepts, Inc
 ******************************************************************************************************/
#include "Init_App.h"
#include "TestApp_USB_CDC.h"
#include "UART.h"
#include "DebugPort.h"
#include "TerminalEmulatorSupport.h"
#include <stdio.h>


// TASKS:
// Spokane Task
uint8_t TestAppTaskStack[3 * THREAD_STACK_SIZE];
TX_THREAD TestAppTaskHandler;
// Debug Console Task
uint8_t DebugConsoleTaskStack[4 * THREAD_STACK_SIZE];
TX_THREAD DebugConsoleHandler;

// TRACE X SUPPORT:
#ifdef USE_TRACEX
#define TRACEX_BUFFER_SIZE 40000
uint8_t tracex_buffer[40000] __attribute__ ((section (".trace")));
#warning "TraceX is defined for use - Make sure to enabled in the .ioc file"
#else
#warning "TraceX not defined for use - Make sure to disable in the .ioc file"
#endif


/**
* @brief Init of hardware and the start the tasks and queues that are required for this application.
*
* @author original: Hab Collector \n
*
* @param void
*
* @return: void
*
* STEP 1: Init Spokane, device drivers and processor hardware
* STEP 2: Init Tasks
* STEP 3: Init Queues
* STEP 4: Init Block Pools
* STEP 5: Check for critical errors
*/
void startAppTasks(void)
{
    uint16_t InitStatus = 0;

    // STEP 1: Init Spokane, device drivers and processor hardware
    Init_TestAppHandle();
    InitStatus = (Init_TestAppHardware() != HARDWARE_INIT_OK)? (InitStatus | 0x0001) : HARDWARE_INIT_OK;

    // STEP 2: Init Tasks
    InitStatus = (tx_thread_create(&TestAppTaskHandler, "TestApp", testAppMainTask, 0x0001, TestAppTaskStack, sizeof(TestAppTaskStack), 13, 13, 1, TX_AUTO_START) != TX_SUCCESS)? (InitStatus | 0x0002) : HARDWARE_INIT_OK;
    InitStatus = (tx_thread_create(&DebugConsoleHandler, "Debug Console", debugConsoleTask, 0x0002, DebugConsoleTaskStack, sizeof(DebugConsoleTaskStack), 15, 15, 1, TX_AUTO_START) != TX_SUCCESS)? (InitStatus | 0x0004) : HARDWARE_INIT_OK;

    // STEP 3: Init Queues

    // STEP 4: Init Block Pools

    // STEP 5: Check for critical errors
    if (InitStatus != HARDWARE_INIT_OK)
        _Error_Handler(__FILE__, __LINE__, InitStatus);

#ifdef USE_TRACEX
    tx_trace_enable(&tracex_buffer, TRACEX_BUFFER_SIZE, 30);
#endif

} // END OF startAppTasks



/**
* @brief This is the redefinition of the write output function used by printf.  Which is defined weak elsewhere
* in syscalls.c and redefined here to target the UART for output.
*
* @author original: Hab Collector \n
*
* @note The full implementation is not used - hence file is ignored.  This is simply to use printf on UART
*
* @param ptr: Pointer to the print message
* @param len: Size of the print message
*
* @return: The number of characters printed
*/
int _write(int file, char *ptr, int len)
{
	UART_DMA_Transmit(&TestApp.Hardware.UART_2, (uint8_t *)ptr, (uint16_t)len);
	tx_thread_sleep(1);

    return(len);

} // END OF _write





