#pragma once

/*
    easytab - Multi-platform drawing-tablet library

    https://github.com/ApoorvaJ/libs/easytab
*/

enum EasyTabResult {
    EASYTAB_OK = 0,
    EASYTAB_EVENT_NOT_HANDLED      =  1,
    /* Errors */
    EASYTAB_MEMORY_ERROR           = -1,
    EASYTAB_X11_ERROR              = -2,
    EASYTAB_DLL_LOAD_ERROR         = -3,
    EASYTAB_WACOM_WIN32_ERROR      = -4,
    EASYTAB_INVALID_FUNCTION_ERROR = -5,
};

enum EasyTabButton {
    EasyTabButton_Pen_Touch , /* Pen is touching tablet */
    EasyTabButton_Pen_Lower,  /* Lower pen button is pressed */
    EasyTabButton_Pen_Upper,  /* Upper pen button is pressed */
};

enum EasyTabTrackingMode {
    EASYTAB_TRACKING_MODE_SYSTEM   = 0,
    EASYTAB_TRACKING_MODE_RELATIVE = 1,
};

#if defined(__linux__)

    #include <X11/extensions/XInput.h>
    enum EasyTabResult easytab_init(Display* disp, Window win);
    enum EasyTabResult easytab_handle_event(XEvent* event);

#elif defined(_WIN32)

    #include <windows.h>
    enum EasyTabResult easytab_init(HWND win);
    enum EasyTabResult easytab_init_ex(HWND win, EasyTabTrackingMode mode,
                                       float relative_mode_sensitivity,
                                       unsigned char move_cursor);
    enum EasyTabResult easytab_hande_event(HWND win, UINT msg,
                                           LPARAM lp, WPARAM wp);

#else
    #error "Unsupported platform."
#endif /* __linux__ , _WIN32 */

/* Non-zero if button is down. Zero otherwise. */
unsigned char easytab_is_button_down(enum EasyTabButton button);
/* (*out_x) and (*out_y) are -1 if the pen is beyond the range of the tablet */
void easytab_get_pos(int* out_x, int* out_y, int* out_range_x, int* out_range_y);
float easytab_get_pressure();
void easytab_destroy();
