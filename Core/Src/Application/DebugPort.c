/** ****************************************************************************************************
 * @file 			DebugPort.c
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
 *				      Spokane_Application
 *				      ACI Pittsburgh
 *
 * @copyright		Applied Concepts, Inc
 ********************************************************************************************************/

#include "DebugPort.h"
#include "TestApp_USB_CDC.h"
#include "TerminalEmulatorSupport.h"
#include "UART.h"
#include <stdio.h>


/**
* @brief This is the main task of the Debug Console.  This task handles all user debug console commands.
* It accepts the debug port entry, parses, and acts if a valid command has been entered.
*
* @author original: Hab Collector \n
*
* @note: Debug commands must be previously added - see function debugConsoleCommandAdd()
*
* @param InitValue a value passed on the creation of the task - not used
*
* @return VOID: ThreadX void
*
* STEP 1: If Debug Port user input send the input to be parsed for a command from the list
*/
VOID debugConsoleTask(ULONG InitValue)
{
    uint8_t DataBuffer[10];
    uint32_t DataSize = 0;

    // STEP 1: If Debug Port user input send the input to be parsed for a command from the list
    while(1)
    {
    	UART_DMA_Receive(&TestApp.Hardware.UART_2, DataBuffer, &DataSize);
        if (DataSize)
        {
            debugConsoleCommandParse(TestApp.DebugConsole, (char *)DataBuffer, DataSize);
        }
        tx_thread_sleep(1);  // Hab added for management
    }

} // END OF debugConsoleTask



/**
* @brief The debug console commands must be init before use.
*
* @author original: Hab Collector \n
*
* @param void
*
* @return A pointer to the debug console handle on success or NULL on fail
*
* STEP 1: Allocate memory for handle
* STEP 2: Reset struct member and clear command names and descriptions to start
*/
Type_ConsoleCommandHandle * Init_DebugConsoleCommand(void)
{
    // STEP 1: Allocate memory for handle
    Type_ConsoleCommandHandle *Handle = (Type_ConsoleCommandHandle *)malloc(sizeof(Type_ConsoleCommandHandle));

    // STEP 2: Reset struct member and clear command names and descriptions to start
    if (Handle != NULL)
    {
        Handle->Enable = true;
        Handle->TotalCommandsAdded = 0;
        Handle->NextCommandIndex = 0;
        Handle->LastCommand = debugConsoleCommandHelp;
        for (uint8_t Index = 0; Index < MAX_CONSOLE_COMMANDS; Index++)
        {
            memset(&Handle->Command[Index].CommandName, 0x00, MAX_COMMAND_LENGTH);
            memset(&Handle->Command[Index].CommandDescription, 0x00, MAX_DESCRIPTION_LENGTH);
            Handle->Command[Index].CommandFunction = NULL;
        }
        return(Handle);
    }
    else
        return(NULL);

} // END OF Init_ConsoleCommand



/**
* @brief Use this function to add debug commands.  The Name, Description and command function are associated
* with a command.  The command function must accept and return void.  The number of commands, command name
* length and command description all of limits.  This is a complete command.
*
* @author original: Hab Collector \n
*
* @note: There are two types of commands: Complete and Partial. Complete requires just the command name, partial
* requires the command name and additional arguments to complete the command. Example if Command name is Simulate
*   Complete would be just "Simulate"
*   Partial would be "Simulate <Additional Arguments As String>"
*
* @param Handle: Debug Console Handle
* @param CommandName: Name of the command that is typed in the debug console to launch the action - limited to MAX_COMMAND_LENGTH
* @param CommandDescription: A brief one sentence description of the command - limited to MAX_DESCRIPTION_LENGTH
* @param consoleCommandFunction: This is the debug function that will run when this command is called
* @param CommandType: Complete or Partial are the only valid options
*
* @return An error type associated with a failure to add the command or a command OK
*
* STEP 1: Check for errors in number of commands, command name and command description length
* STEP 2: Associate the command name, description and function with the next available command index
* STEP 3: Increment total number of commands and next command index
*/
Type_CommandError debugConsoleCommandAdd(Type_ConsoleCommandHandle *Handle, const char *CommandName, const char *CommandDescription, ConsoleCommand consoleCommandFunction, Type_Command CommandType)
{
    // STEP 1: Check for errors in number of commands, command name and command description length
    if (Handle->TotalCommandsAdded >= MAX_CONSOLE_COMMANDS)
        return(MAX_COMMANDS_EXCEEDED);
    if (strlen(CommandName) > MAX_COMMAND_LENGTH)
        return(COMMAND_LENGHT_EXCEEDED);
    if (strlen(CommandDescription) > MAX_DESCRIPTION_LENGTH)
        return(COMMAND_DESCRIPTION_EXCEEDED);

    // STEP 2: Associate the command name, description and function with the next available command index
    strcpy(&Handle->Command[Handle->NextCommandIndex].CommandName[0], CommandName);
    strcpy(&Handle->Command[Handle->NextCommandIndex].CommandDescription[0], CommandDescription);
    Handle->Command[Handle->NextCommandIndex].CommandType = CommandType;
    if (consoleCommandFunction != NULL)
        Handle->Command[Handle->NextCommandIndex].CommandFunction = consoleCommandFunction;

    // STEP 3: Increment total number of commands and next command index
    Handle->TotalCommandsAdded++;
    Handle->NextCommandIndex++;

    return(COMMAND_OK);

} // END OF debugConsoleCommandAdd



/**
* @brief Searches the command array for the first occurrence of a command name from the list of added debug
* commands or from the built in (base) list of commands.  The built in list includes
*   Clear: Clears the terminal screen
*   Help:  Command listing
*
* @author original: Hab Collector \n
*
* @note: There are two types of commands: Complete and Partial. Complete requires just the command name, partial
* requires the command name and additional arguments to complete the command. Example if Command name is Simulate
*   Complete would be just "Simulate"
*   Partial would be "Simulate <Additional Arguments As String>"
*
* @param Handle: Debug Console Handle
* @param CommandName: Name of the command for which to search
* @param CommandIndex: Return by reference the command index if the search is found
*
* @return True if the command name was found in the list and CommandIndex pointer is set accordingly
*
* STEP 1: Linear search for command - if found return true and pass by reference the command index
*/
bool debugConsoleCommandSearch(Type_ConsoleCommandHandle *Handle, const char *CommandName, uint8_t *CommandIndex)
{
    // STEP 1: Linear search for command - if found return true and pass by reference the command index
    for (uint8_t Index = 0; Index < Handle->TotalCommandsAdded; Index++)
    {
        // Check base listing first then check added commands
        if (strcmp(CommandName, "Clear") == 0)
        {
            terminal_ClearScreen();
            return(false);
        }
        if (strcmp(CommandName, "Help") == 0)
        {
            debugConsoleCommandHelp();
            return(false);
        }
        // Check added commands: Only look to add command argument (parameter) if partial command
        uint8_t UserEntryStrLen = strlen(CommandName);
        uint8_t CmdStrLen = strlen(&Handle->Command[Index].CommandName[0]);
        if (strncmp(CommandName, &Handle->Command[Index].CommandName[0], CmdStrLen) == 0)
        {
            if (Handle->Command[Index].CommandType == COMPLETE)
            {
                *CommandIndex = Index;
                return(true);
            }
            else
            {
                if ((UserEntryStrLen - CmdStrLen) > MAX_PARAMETER_LENGTH)
                    return(false);
                memset(&Handle->PartialCommandParameter, 0x00, MAX_PARAMETER_LENGTH);
                strncpy(Handle->PartialCommandParameter, &CommandName[CmdStrLen], (UserEntryStrLen - CmdStrLen));
                *CommandIndex = Index;
                return(true);
            }
        }
    }
    return(false);

} // END OF debugConsoleCommandSearch



/**
* @brief Parse the console input.  The input is made of alpha numeric entries and special characters which
* are handled specifically.  Special handling characters include:
*   '\r': Carriage Return - Execute the command that was entered
*   '\b': Back Space - undo last typed character
*   '?':  Show help
*   '`':  Repeat last command
* The user enters commands (according to help menu) and presses enter.  If the command entered via the console
* matches a command in the command list then that command is executed.  If not then no action.  Note commands can
* be PARTIAL or COMPLETE
*
* @author original: Hab Collector \n
*
* @note: There are two types of commands: Complete and Partial. Complete requires just the command name, partial
* requires the command name and additional arguments to complete the command. Example if Command name is Simulate
*   Complete would be just "Simulate"
*   Partial would be "Simulate <Additional Arguments As String>"
*
* @param Handle: Debug Console Handle
* @param CommandChars: A buffer with the present command (partial) being entered by user
* @param TotalChars: Total number of characters in the CommandChar buffer
*
* @return void
*
* STEP 1: For all entries except CR and ` echo the input, build the command entry name and check for errors
* STEP 2: On CR check if command entered = a command from list if so execute - if not do nothing
* STEP 3: If user enters `, then repeat the last valid command
* STEP 4: If user enters ?, then launch help
* STEP 5: If user enters backspace to undo typing
* STEP 6: Reset the output as is necessary
*/
void debugConsoleCommandParse(Type_ConsoleCommandHandle *Handle, char *CommandChars, uint32_t TotalChars)
{
    static char CommandEntry[MAX_DESCRIPTION_LENGTH] = {0};
    static uint8_t CommandCharIndex = 0;
    static bool CommandLengthExceeded = false;
    char *CommandChar;
    bool ResetCommand = false;

    for (uint32_t CharIndex = 0; CharIndex < TotalChars; CharIndex++)
    {
        CommandChar = &CommandChars[CharIndex];
        // STEP 1: For all entries except CR and ` echo the input, build the command entry name and check for errors
        if ((*CommandChar != '\r') && (*CommandChar != '`') && (*CommandChar != '?') && (*CommandChar != '\b'))
        {
            printf("%c", CommandChar[0]);
            fflush(stdout);
            if (CommandCharIndex > sizeof(CommandEntry))
            {
                memset(CommandEntry, 0x00, MAX_COMMAND_LENGTH);
                CommandCharIndex = 0;
                CommandLengthExceeded = true;
            }
            CommandEntry[CommandCharIndex] = *CommandChar;
            CommandCharIndex++;
        }

        // STEP 2: On CR check if command entered = a command from list if so execute - if not do nothing
        if (*CommandChar == '\r')
        {
            printf("\r\n");
            fflush(stdout);
            if (CommandLengthExceeded)
            {
                printf("\r\nError Debug Console: Max command length = %d\r\n", MAX_COMMAND_LENGTH);
            }
            else
            {
                uint8_t CommandIndex;
                if (debugConsoleCommandSearch(Handle, CommandEntry, &CommandIndex))
                {
                    if (Handle->Command[CommandIndex].CommandType == COMPLETE)
                        Handle->Command[CommandIndex].CommandFunction(NULL);
                    else
                        Handle->Command[CommandIndex].CommandFunction((void *)Handle->PartialCommandParameter);
                    Handle->LastCommand = Handle->Command[CommandIndex].CommandFunction;
                }
                ResetCommand = true;
            }
        }

        // STEP 3: If user enters `, then repeat the last valid command
        if (*CommandChar == '`')
        {
            if (Handle->LastCommand != NULL)
            {
                printf("\r\n");
                Handle->LastCommand((void *)Handle->PartialCommandParameter);
            }
            ResetCommand = true;
        }

        // STEP 4: If user enters ?, then launch help
        if (*CommandChar == '?')
        {
            printf("\r\n");
            debugConsoleCommandHelp();
            ResetCommand = true;
        }

        // STEP 5: If user enters backspace to undo typing
        if (*CommandChar == '\b')
        {
            CommandEntry[CommandCharIndex] = 0x00;
            CommandCharIndex = (CommandCharIndex == 0)? 0 : (CommandCharIndex - 1);
            terminal_CursorBackSpace();
            printf(" ");
            terminal_CursorBackSpace();
            fflush(stdout);
        }

        // STEP 6: Reset the output as is necessary
        if (ResetCommand)
        {
            CommandLengthExceeded = false;
            memset(CommandEntry, 0x00, sizeof(CommandEntry));
            CommandCharIndex = 0;
            printf("\r\nCommand: ");
            fflush(stdout);
        }
    }

} // END OF debugConsoleCommandParse



/**
* @brief List all debug commands along with a description of the command
*
* @author original: Hab Collector \n
*
* @param void
*
* @return void
*
* STEP 1: List all commands and the associated description - commands are listed in the order added
*/
void debugConsoleCommandHelp(void)
{
    // STEP 1: List all commands and the associated description - commands are listed in the order added
    printf("%-20s: %s\r\n", "COMMAND", "DESCRIPTION");
    for (uint8_t Index = 0; Index < TestApp.DebugConsole->TotalCommandsAdded; Index++)
    {
        printf("%-20s: %s\r\n", TestApp.DebugConsole->Command[Index].CommandName, TestApp.DebugConsole->Command[Index].CommandDescription);
    }
    printf("%-20s: %s\r\n", "Help", "List all debug commands");
    printf("%-20s: %s\r\n", "Clear", "Clears the terminal screen");
    printf("%-20s: %s\r\n", "`", "Repeat last command");
    printf("%-20s: %s\r\n", "?", "Same as help");

} // END OF debugConsoleCommandHelp
