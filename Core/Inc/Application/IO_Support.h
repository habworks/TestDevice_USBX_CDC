/** ****************************************************************************************************
 * @file 			IO_Support.h
 * @brief			This is the Header file used to support IO_Support.h.c
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
 *                    Spokane_Application
 *				      Design Document
 *
 * @copyright		Applied Concepts, Inc
 ****************************************************************************************************** */

#ifndef INC_IO_SUPPORT_H_
#define INC_IO_SUPPORT_H_

#ifdef __cplusplus
extern"C" {
#endif

#include "main.h"

#define STATUS_LED_ON()         HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET)
#define STATUS_LED_OFF()        HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET)
#define STATUS_LED_TOGGLE()     HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin)
#define STATUS_LED_STATUS()     HAL_GPIO_ReadPin(STATUS_LED_GPIO_Port, STATUS_LED_Pin)

#define TP9_SET()               HAL_GPIO_WritePin(TP9_GPIO_Port, TP9_Pin, GPIO_PIN_SET)
#define TP9_CLR()               HAL_GPIO_WritePin(TP9_GPIO_Port, TP9_Pin, GPIO_PIN_RESET)
#define TP9_TOGGLE()            HAL_GPIO_TogglePin(TP9_GPIO_Port, TP9_Pin)
#define TP9_STATUS()            HAL_GPIO_ReadPin(TP9_GPIO_Port, TP9_Pin)

#define TP10_SET()              HAL_GPIO_WritePin(TP10_GPIO_Port, TP10_Pin, GPIO_PIN_SET)
#define TP10_CLR()              HAL_GPIO_WritePin(TP10_GPIO_Port, TP10_Pin, GPIO_PIN_RESET)
#define TP10_TOGGLE()           HAL_GPIO_TogglePin(TP10_GPIO_Port, TP10_Pin)
#define TP10_STATUS()           HAL_GPIO_ReadPin(TP10_GPIO_Port, TP10_Pin)

#define TP11_SET()              HAL_GPIO_WritePin(TP11_GPIO_Port, TP11_Pin, GPIO_PIN_SET)
#define TP11_CLR()              HAL_GPIO_WritePin(TP11_GPIO_Port, TP11_Pin, GPIO_PIN_RESET)
#define TP11_TOGGLE()           HAL_GPIO_TogglePin(TP11_GPIO_Port, TP11_Pin)
#define TP11_STATUS()           HAL_GPIO_ReadPin(TP11_GPIO_Port, TP11_Pin)

#define VBUS_ON()               HAL_GPIO_WritePin(USB_OTG_VBUS_EN_GPIO_Port, USB_OTG_VBUS_EN_Pin, GPIO_PIN_SET)
#define VBUS_OFF()              HAL_GPIO_WritePin(USB_OTG_VBUS_EN_GPIO_Port, USB_OTG_VBUS_EN_Pin, GPIO_PIN_RESET)
#define VBUS_TOGGLE()           HAL_GPIO_TogglePin(USB_OTG_VBUS_EN_GPIO_Port, USB_OTG_VBUS_EN_Pin)
#define VBUS_STATUS()           HAL_GPIO_ReadPin(USB_OTG_VBUS_EN_GPIO_Port, USB_OTG_VBUS_EN_Pin)

#ifdef __cplusplus
}
#endif
#endif /* INC_IO_SUPPORT_H_ */
