/* gcc test_linux.c ../easytab_linux.c -I../ -lXi -lX11 --std=c89 */

#include "easytab.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool running = true;

#define ETCHK(x) { \
    enum EasytabResult res = x; \
    if (x != EASYTAB_OK) { \
        printf("%s failed with value %d at %s:%d\n", #x, res, __FILE__, __LINE__); \
    }}

int main()
{
    Display* disp = 0;
    Window win = 0;
    Atom wm_delete_msg;
    enum EasytabResult res;

    disp = XOpenDisplay(0);
    wm_delete_msg = XInternAtom(disp, "WM_DELETE_WINDOW", false);
    win = XCreateSimpleWindow(disp, DefaultRootWindow(disp),
                              10, 10,   /* x, y */
                              800, 600, /* width, height */
                              0, 0,     /* border_width, border */
                              0);       /* background */
    XSetWMProtocols(disp, win, &wm_delete_msg, 1);
    XMapWindow(disp, win);

    res = easytab_init(disp, win);
    if (res != EASYTAB_OK) {
        printf("easy_tab_init failed with %d\n", res);
        exit(-1);
    }

    while (running) {
        /* Event loop */
        while (XPending(disp)) {
            XEvent event;
            XNextEvent(disp, &event);

            if (easytab_handle_event(&event) == EASYTAB_OK) {
                continue;
            }

            if(event.type == ClientMessage &&
               event.xclient.data.l[0] == wm_delete_msg) {
                running = false;
            }
        }
        /* Printing tablet values */
        {
            float pressure = 0.0f;
            int x = 0, y = 0;
            ETCHK( easytab_get_pos(&x, &y) );
            ETCHK( easytab_get_pressure(&pressure) );
            printf("x: %d, y: %d, pressure: %.02f\n", x, y, pressure);
        }
    }

    XDestroyWindow(disp, win);
    XCloseDisplay(disp); 

    return 0;
}