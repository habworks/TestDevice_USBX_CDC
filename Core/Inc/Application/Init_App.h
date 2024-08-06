/******************************************************************************************************
 * @file            Init_App.h
 * @brief           This is the Header file used to support Init_App.c
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
 * @note            See source file for notes
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
#ifndef INC_APPLICATION_INIT_APP_H_
#define INC_APPLICATION_INIT_APP_H_

#ifdef __cplusplus
extern"C" {
#endif

#include "app_threadx.h"
#include "main.h"
#include <stdint.h>

// DEFINES
#define THREAD_STACK_SIZE           1024U
// BLOCK POOLS: One over head per block
#define BLOCK_POOL_OVERHEAD         sizeof(void *)
// 1KB
#define TOTAL_4KB_BLOCKS            1U
#define BLOCK_4KB_SIZE              4096U
#define BLOCK_4KB_POOL_SIZE         (BLOCK_4KB_SIZE + BLOCK_POOL_OVERHEAD) * TOTAL_4KB_BLOCKS


// TYPEDEFS AND ENUMS


// FUNCTION PROTOTYPES
void startAppTasks(void);

#ifdef __cplusplus
}
#endif
#endif /* INC_APPLICATION_INIT_APP_H_ */
