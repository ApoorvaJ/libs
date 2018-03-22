#pragma once

/*
    easytab - Multi-platform drawing-tablet library

    https://github.com/ApoorvaJ/libs/easytab
*/

enum EasytabResult {
    EASYTAB_OK = 0,
    EASYTAB_EVENT_NOT_HANDLED      =  1,
    /* Errors */
    EASYTAB_NOT_INITIALIZED_ERROR  = -1,
    EASYTAB_MEMORY_ERROR           = -2,
    EASYTAB_DEVICE_NOT_FOUND_ERROR = -3,
    EASYTAB_DLL_LOAD_ERROR         = -4,
    EASYTAB_WACOM_WIN32_ERROR      = -5,
    EASYTAB_INVALID_FUNCTION_ERROR = -6,
};

#if defined(__linux__)

    #include <X11/extensions/XInput.h>

    enum EasytabResult easytab_init(Display* disp, Window win);
    enum EasytabResult easytab_handle_event(XEvent* event);

#elif defined(_WIN32)

    #include <windows.h>
    enum EasytabTrackingMode {
        EASYTAB_TRACKING_MODE_SYSTEM   = 0,
        EASYTAB_TRACKING_MODE_RELATIVE = 1,
    };

    enum EasytabResult easytab_init(HWND win);
    enum EasytabResult easytab_init_ex(HWND win, EasytabTrackingMode mode,
                                       float relative_mode_sensitivity,
                                       unsigned char move_cursor);
    enum EasytabResult easytab_hande_event(HWND win, UINT msg,
                                           LPARAM lp, WPARAM wp);

#else
    #error "Unsupported platform."
#endif /* __linux__ , _WIN32 */

enum EasytabResult easytab_get_pos(int* out_x, int* out_y);
enum EasytabResult easytab_get_pressure(float* out_pressure);
enum EasytabResult easytab_destroy();
