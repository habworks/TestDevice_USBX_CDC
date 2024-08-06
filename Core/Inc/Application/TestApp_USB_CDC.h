/** ****************************************************************************************************
 * @file 			TestApp_USB_MSC.h
 * @brief			This is the Header file used to support TestApp_USB_MSC.h.c
 * ****************************************************************************************************
 * @author original	Hab Collector (habco) \n
 *
 * @version       	See Main_Support.h: FIRMWARE_REV_MAJOR, FIRMWARE_REV_MINOR
 *
 * @param Development_Environment \n
 * Hardware:		<IC PN> \n
 * IDE:             STMCubeIDE VER 1.9.0  \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            See source file for notes
 *
 * 					This is an embedded application
 *		            It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *			          Schematic <Schematic PN>
 *                    Test_USB_MSC
 *				      Design Document
 *
 * @copyright		Applied Concepts, Inc
 ****************************************************************************************************** */

#ifndef INC_TESTAPP_USB_CDC_H_
#define INC_TESTAPP_USB_CDC_H_

#ifdef __cplusplus
extern"C" {
#endif

#include "main.h"
#include "app_threadx.h"
#include "DebugPort.h"
#include "UART.h"

// DEFINES
#define HARDWARE_INIT_OK                    (0U)


// TYPEDEFS AND ENUMS
typedef enum
{
    USB_EVENT_NO_EVENT = 0x00,
    USB_EVENT_CDC_INSERTED = 0x01,
    USB_EVENT_CDC_REMOVED = 0x02,
    USB_EVENT_CDC_RX = 0x04,
    USB_EVENT_CDC_TX = 0x08
}Type_USB_Event;

typedef struct
{
    UART        UART_2;     // Debug Port
}Type_TestAppHardware;

typedef struct
{
    Type_ConsoleCommandHandle *     DebugConsole;
    Type_TestAppHardware            Hardware;
}Type_TestApp;


// MACROS
#define NOT_USED(x)             (void)(x)
#define DO_NOTHING()            __NOP()


// FUNCTION PROTOTYPES
extern Type_TestApp TestApp;
VOID testAppMainTask(ULONG InitValue);
void Init_TestAppHandle(void);
uint16_t Init_TestAppHardware(void);
void _Error_Handler(char *file, int line, uint32_t Error);

#ifdef __cplusplus
}
#endif
#endif /* INC_TESTAPP_USB_CDC_H_ */
