
easytab - Multi-platform drawing-tablet library

https://github.com/ApoorvaJ/libs/easytab

--------------------------------------------------------------------------------
SINGLE-HEADER USAGE
--------------------------------------------------------------------------------
1) Add the following lines in exactly one of your cpp files to compile the
   implementation.

       #define EASYTAB_IMPLEMENTATION
       #include "easytab.h"

--------------------------------------------------------------------------------
API USAGE
--------------------------------------------------------------------------------
1) Call easytab_init() with correct parameters to initialize EasyTab. These
   parameters vary per OS, so look at the function declarations or examples
   below. Function returns EASYTAB_OK if initialization was successful.

2) Call EasyTab_HandleEvent() in your message-handling code. The function
   returns EASYTAB_OK if the message was a tablet message, and
   EASYTAB_EVENT_NOT_HANDLED otherwise.

3) Call EasyTab_Unload() in your shutdown code.

4) Once initialized, you can query tablet state using the EasyTab pointer.
   e.g.:

       EasyTab->PosX        // X position of the pen
       EasyTab->PosY        // Y position of the pen
       EasyTab->Pressure    // Pressure of the pen ranging from 0.0f to 1.0f

   For more info, have a look at the EasyTabInfo struct below.


* Add -lXi to compiler options to link XInput on Linux.

----------------------------------------------------------------------------
EXAMPLES
----------------------------------------------------------------------------
1) Windows:

    int CALLBACK WinMain(...)
    {
        HWND Window;

        ...

        if (easytab_init(Window) != EASYTAB_OK)                                  // Load
        {
            OutputDebugStringA("Tablet init failed\n");
        }

        ...

        // Once you've set up EasyTab loading, unloading and event handling,
        // use the EasyTab variable at any point in your program to access
        // the tablet state:
        //    EasyTab->PosX
        //    EasyTab->PosY
        //    EasyTab->Pressure
        // For more tablet information, look at the EasyTabInfo struct.

        ...

        EasyTab_Unload();                                                      // Unload
    }

    LRESULT CALLBACK WindowProc(
        HWND Window,
        UINT Message,
        WPARAM WParam,
        LPARAM LParam)
    {
        if (EasyTab_HandleEvent(Window, Message, LParam, WParam) == EASYTAB_OK) // Event
        {
            return true; // Tablet event handled
        }

        switch (Message)
        {
            ...
        }
    }


2) Linux:

    int main(...)
    {
        Display* Disp;
        Window   Win;

        ...

        if (easytab_init(Disp, Win) != EASYTAB_OK)                   // Load
        {
            printf("Tablet init failed\n");
        }

        ...

        while (XPending(Disp)) // Event loop
        {
            XEvent Event;
            XNextEvent(XlibDisplay, &Event);

            if (EasyTab_HandleEvent(&Event) == EASYTAB_OK)          // Event
            {
                continue; // Tablet event handled
            }

            switch (Event.type)
            {
                ...
            }
        }

        ...

        // Once you've set up EasyTab loading, unloading and event handling,
        // use the EasyTab variable at any point in your program to access
        // the tablet state:
        //    EasyTab->PosX
        //    EasyTab->PosY
        //    EasyTab->Pressure
        // For more tablet information, look at the EasyTabInfo struct.

        ...

        EasyTab_Unload();                                          // Unload
    }

----------------------------------------------------------------------------
CREDITS
----------------------------------------------------------------------------
Apoorva Joshi       apoorvaj.io
Sergio Gonzalez     s3rg.io

This library is coded in the spirit of the stb libraries and follows the stb
guidelines.
