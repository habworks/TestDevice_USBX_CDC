
#/** ****************************************************************************************************
 * @file 			TestApp_USB_MSC.c
 * @brief			<Add a descrition of what the file does>
 * ****************************************************************************************************
 * @author original	Hab Collector (habco)\n
 *
 * @version       	See Main_Support.h: FIRMWARE_REV_MAJOR, FIRMWARE_REV_MINOR
 *
 * @param Development_Environment \n
 * Hardware:		<IC PN> \n
 * IDE:             STMCubeIDE VER 1.9.0 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            The associated header file provides MACRO functions for IO control
 *
 * 					This is an embedded application
 *		            It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *			          Schematic: <Schematic PN>
 *				      Test_USB_MSC
 *				      ACI Pittsburgh
 *
 * @copyright		Applied Concepts, Inc
 ********************************************************************************************************/

#include "TestApp_USB_CDC.h"
#include "IO_Support.h"
#include "DebugPort.h"
#include "TerminalEmulatorSupport.h"
#include "UART.h"
#include "app_usbx_device.h"
#include "tx_api.h"
#include "ux_api.h"
#include <stdio.h>


// Global Vars
Type_TestApp TestApp;
extern TX_EVENT_FLAGS_GROUP USB_EventFlag;

// DEBUG COMMANDS
// Power toggle test commands
static void TPx_Toggle(void *TestPoint);
static void LED_Toggle(void *NotUsed);
static void commDeviceClassDisable(void *NotUsed);
static void commDeviceClassEnable(void *NotUsed);

VOID testAppMainTask(ULONG InitValue)
{
    // STEP 1: Add debug commands associated with the Spokane Task
    debugConsoleCommandAdd(TestApp.DebugConsole, "Toggle ", "Toggle TP<x> (x: 9, 10, 11)", TPx_Toggle, PARTIAL);
    debugConsoleCommandAdd(TestApp.DebugConsole, "LED", "Toggle LED power", LED_Toggle, COMPLETE);
    debugConsoleCommandAdd(TestApp.DebugConsole, "CDC Off", "Disable CDC", commDeviceClassDisable, COMPLETE);
    debugConsoleCommandAdd(TestApp.DebugConsole, "CDC On", "Enable CDC", commDeviceClassEnable, COMPLETE);

    // STEP 2: Show start up message
    terminal_SetDefaultForegroundColor();
    terminal_ClearScreen();
    printf("ACI Stalker\r\n");
    printf("Hab testing of Azure RTOS USB Device CDC\r\n");
    printf("Build Notes: Hello Hab, time to make the donuts\r\n\n");
    fflush(stdout);

    // STEP 3: Main task
    while (1)
    {
        ULONG USB_CDC_EventFlag = 0;
        tx_event_flags_get(&USB_EventFlag, 0xFF, TX_OR_CLEAR, &USB_CDC_EventFlag, TX_WAIT_FOREVER);
        printf("USB_CDC_EventFlag = %d\r\n", (int)USB_CDC_EventFlag);
        if (USB_CDC_EventFlag == USB_EVENT_CDC_INSERTED)
        {
            printf("USB Drive was inserted\r\n");
        }
        else if (USB_CDC_EventFlag == USB_EVENT_CDC_REMOVED)
        {
            printf("USB Drive was removed\r\n");
        }
        else
        {
            DO_NOTHING();
        }

        // SLEEP ALLOW OTHER TASKS TO RUN:
        tx_thread_sleep(100);
        STATUS_LED_TOGGLE();
    }
}



void Init_TestAppHandle(void)
{
    DO_NOTHING();
}



/**
* @brief Sets the default condition of outputs and the Init of the necessary processor hardware for use.
* Init of the Spokane: Hardware, DebugConsole, ...
* All Inits must be successful, and failure to init is considered a critical error
*
* @author original: Hab Collector \n
*
* @param void
*
* @return 0 if all hardware was init or a bit set value indicating the failure code(s)
*
* STEP 1: Set default IO conditions
* STEP 2: Init hardware
* STEP 3: Init API Handlers
* STEP 4: Return init status
*/
uint16_t Init_TestAppHardware(void)
{
    // STEP 1: Set default IO conditions
    TP9_CLR();
    TP10_CLR();
    TP11_CLR();
    STATUS_LED_OFF();

    // STEP 2: Init hardware
    uint16_t InitStatus = 0x0000;

    // UART 2: Debug Port
    Init_UART_DMA(&TestApp.Hardware.UART_2, &huart2);
    InitStatus = (&TestApp.Hardware.UART_2 == NULL)? InitStatus | 0x0001 : InitStatus;

    // STEP 3: Init API Handlers
    // DEBUG CONSOLE
    TestApp.DebugConsole = Init_DebugConsoleCommand();
    InitStatus = (TestApp.DebugConsole == NULL)? InitStatus | 0x0020 : InitStatus;

    // STEP 4: Return init status
    return(InitStatus);
}



/******************************************************************************************************
* @brief Error handler - just trap the fatal error by waiting here forever
*
* @author original: Hab Collector \n
*
* @note: To use call the function just like this... _Error_Handler(__FILE__, __LINE__);
*
* @param file: Pointer to file name
* @param line: Pointer to line number
* @param Error: Specific error code
*
* STEP 1: Print critical error and stay here to allow debug
* *****************************************************************************************************/
void _Error_Handler(char *file, int line, uint32_t Error)
{
    // STEP 1: Print critical error and stay here to allow debug
    terminal_SetRedForeground();
    printf("***CRITICAL ERROR: %d\r\n", (int)Error);
    printf("Trapped here so you can debug\r\n");
    fflush(stdout);
    __disable_irq();
    while(1);

} // END OF _Error_Handler




/*****************************************DEBUG COMMANDS***************************************/
/**
 * @brief Toggle Test ponts
 * @param void pointer: user console input
 * @return void
 */
static void TPx_Toggle(void *TestPoint)
{
    bool CommandArgumentValid = false;
    GPIO_PinState PinState;
    char *CommandArgument = (char *)TestPoint;
    if (strcmp(CommandArgument, "TP9") == 0)
    {
        TP9_TOGGLE();
        PinState = TP9_STATUS();
        CommandArgumentValid = true;
    }
    if (strcmp(CommandArgument, "TP10") == 0)
    {
        TP10_TOGGLE();
        PinState = TP10_STATUS();
        CommandArgumentValid = true;
    }
    if (strcmp(CommandArgument, "TP11") == 0)
    {
        TP11_TOGGLE();
        PinState = TP11_STATUS();
        CommandArgumentValid = true;
    }
    if (CommandArgumentValid)
    {
        printf("%s was toggled to ", CommandArgument);
        if (PinState == GPIO_PIN_SET)
            printf("high \r\n");
        else
            printf("low \r\n");
    }
}


static void LED_Toggle(void *NotUsed)
{
    (void)NotUsed;
    STATUS_LED_TOGGLE();
    GPIO_PinState PinState = STATUS_LED_STATUS();
    if (PinState == GPIO_PIN_SET)
        printf("LED is on\r\n");
    else
        printf("LED is off\r\n");
}


static void commDeviceClassDisable(void *NotUsed)
{
    USBX_APP_Device_UnInit();
    if (MX_USBX_Device_UnInit() == TX_SUCCESS)
        printf("USB CDC Turned off\r\n");
    else
        while(1);
}


extern TX_BYTE_POOL *Ux_HostAppBytePool;
static void commDeviceClassEnable(void *NotUsed)
{
    VOID *MemoryPointer;
    MemoryPointer = (VOID *)Ux_HostAppBytePool;
    if (MX_USBX_Device_Init(MemoryPointer) != UX_SUCCESS)
        while(1);
    printf("USB CDC Turned on\r\n");
}
