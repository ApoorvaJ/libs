
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

2) Call easytab_handle_event() in your message-handling code. The function
   returns EASYTAB_OK if the message was a tablet message, and
   EASYTAB_EVENT_NOT_HANDLED otherwise.

3) Call easytab_destroy() in your shutdown code.

4) Once initialized, you can query tablet state using the following APIs:

       easytab_is_button_down()
       easytab_get_pos();
       easytab_get_pressure();

* Add -lXi to compiler options to link XInput on Linux.

----------------------------------------------------------------------------
EXAMPLES
----------------------------------------------------------------------------
1) Windows:

    int CALLBACK WinMain(...)
    {
        HWND win;

        ...

        if (easytab_init(win) != EASYTAB_OK) {
            OutputDebugStringA("Tablet init failed\n");
        }

        ...

        int x, y, range_x, range_y;
        easytab_get_pos(&x, &y, &range_x, &range_y);

        ...

        easy_tab_destroy();
    }

    LRESULT CALLBACK WindowProc(
        HWND win,
        UINT msg,
        WPARAM wp,
        LPARAM lp)
    {
        if (easytab_handle_event(win, msg, lp, wp) == EASYTAB_OK) {
            return true; /* Tablet event handled */
        }

        switch (msg) {
            ...
        }
    }


2) Linux:

    int main(...)
    {
        Display* disp;
        Window   win;

        ...

        if (easytab_init(disp, win) != EASYTAB_OK) {
            printf("Tablet init failed\n");
        }

        ...

        while (XPending(Disp)) {
            XEvent event;
            XNextEvent(XlibDisplay, &event);

            if (easytab_handle_event(&event) == EASYTAB_OK) {
                continue; /* Tablet event handled */
            }

            switch (event.type) {
                ...
            }
        }

        ...

        int x, y, range_x, range_y;
        easytab_get_pos(&x, &y, &range_x, &range_y);

        ...

        easytab_destroy();
    }

----------------------------------------------------------------------------
CREDITS
----------------------------------------------------------------------------
Apoorva Joshi       apoorvaj.io
Sergio Gonzalez     s3rg.io

This library is coded in the spirit of the stb libraries and follows the stb
guidelines.
