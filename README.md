# TestDevice_USBX_CDC
This is a test app that supports the Azure USBX Device as a CDC Virtual Comm Port (VComm) for the Spokane Hardware.
How It Works:
1. Connect a terminal to the debug screen
2. Connect a terminal to the Spokane USB Port

The Spokane connected terminal will constatly output the message: "USBX Running VCOMM #", where # is an incrementing value starting from 0
Typing on the Spokane connected terminal will be displayed on the debug terminal in yellow.
Typing a 1 on the VComm will turn on TP9
Typing a 0 on the VComm will turn off TP9

Note on the Debug Terminal you can type Help or ? to get a list of commands.