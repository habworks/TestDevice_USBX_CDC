/** ****************************************************************************************************
 * @file 			DebugPort.h
 * @brief			This is the Header file used to support DebugPort.h.c
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
#ifndef INC_THREADX_DEBUGPORT_H_
#define INC_THREADX_DEBUGPORT_H_

#ifdef __cplusplus
extern"C" {
#endif

#include "app_threadx.h"
#include <stdint.h>
#include <stdbool.h>

// DEFINES
#define MAX_CONSOLE_COMMANDS    22U //20U
#define MAX_COMMAND_LENGTH      (20U + 1U)
#define MAX_INPUT_LENGTH        30U
#define MAX_DESCRIPTION_LENGTH  50U
#define MAX_PARAMETER_LENGTH    20U


// TYPEDEFS AND ENUMS
typedef void (*ConsoleCommand)(void *);
typedef enum
{
    PARTIAL = 0,
    COMPLETE
}Type_Command;

typedef struct
{
    Type_Command    CommandType;
    char            CommandName[MAX_COMMAND_LENGTH];
    char            CommandDescription[MAX_DESCRIPTION_LENGTH];
    ConsoleCommand  CommandFunction;
}Type_ConsoleCommand;

typedef struct
{
    bool                Enable;
    uint8_t             NextCommandIndex;
    uint8_t             TotalCommandsAdded;
    ConsoleCommand      LastCommand;
    Type_ConsoleCommand Command[MAX_CONSOLE_COMMANDS];
    char                PartialCommandParameter[MAX_PARAMETER_LENGTH];
}Type_ConsoleCommandHandle;

typedef enum
{
    COMMAND_OK = 0,
    MAX_COMMANDS_EXCEEDED,
    COMMAND_LENGHT_EXCEEDED,
    COMMAND_DESCRIPTION_EXCEEDED
}Type_CommandError;


// FUNCTION PROTOTYPES
VOID debugConsoleTask(ULONG InitValue);
Type_ConsoleCommandHandle * Init_DebugConsoleCommand(void);
Type_CommandError debugConsoleCommandAdd(Type_ConsoleCommandHandle *Handle, const char *CommandName, const char *CommandDescription, ConsoleCommand consoleCommandFunction, Type_Command CommandType);
bool debugConsoleCommandSearch(Type_ConsoleCommandHandle *Handle, const char *CommandName, uint8_t *CommandIndex);
void debugConsoleCommandParse(Type_ConsoleCommandHandle *Handle, char *CommandChar, uint32_t TotalChars);
void debugConsoleCommandHelp(void);


#ifdef __cplusplus
}
#endif
#endif /* INC_THREADX_DEBUGPORT_H_ */
