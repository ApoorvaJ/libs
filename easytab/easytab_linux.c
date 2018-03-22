/*
    ----------------------------------------------------------------------------
    LINUX_IMPLEMENTATION
    ----------------------------------------------------------------------------
*/
#ifdef __linux__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "easytab.h"

struct EasytabCtx {
    int32_t pos_x, pos_y;
    int32_t range_x, range_y;

    float pressure; /* Range: [0,1] */
    int32_t max_pressure;

    XDevice* device;
    uint32_t motion_type;
    XEventClass event_classes[1024];
    uint32_t num_event_classes;
    Display* disp;
};

struct EasytabCtx* ctx;

enum EasyTabResult easytab_init(Display* disp, Window win)
{
    int32_t i, j;
    int32_t num_devices = 0;

    ctx = (struct EasytabCtx*)calloc(1, sizeof(*ctx));
    if (!ctx) { return EASYTAB_MEMORY_ERROR; }

    ctx->disp = disp;

    XDeviceInfoPtr Devices = (XDeviceInfoPtr)XListInputDevices(disp, &num_devices);
    if (!Devices) { return EASYTAB_X11_ERROR; }

    for (i = 0; i < num_devices; i++) {
        if (!strstr(Devices[i].name, "stylus") &&
            !strstr(Devices[i].name, "eraser")) {
            continue;
        }

        ctx->device = XOpenDevice(disp, Devices[i].id);
        XAnyClassPtr ClassPtr = Devices[i].inputclassinfo;

        for (j = 0; j < Devices[i].num_classes; j++) {
#if defined(__cplusplus)
            switch (ClassPtr->c_class)
#else
            switch (ClassPtr->class)
#endif
            {
            case ValuatorClass: {
                XValuatorInfo *info = (XValuatorInfo *)ClassPtr;
                XEventClass EventClass;
                DeviceMotionNotify(ctx->device, ctx->motion_type, EventClass);

                /* Pos */
                if (info->num_axes > 0) {
                    ctx->range_x = info->axes[0].max_value;
                }
                if (info->num_axes > 1) {
                    ctx->range_y = info->axes[1].max_value;
                }
                /* Pressure */
                if (info->num_axes > 2) {
                    int32_t min          = info->axes[2].min_value;
                    ctx->max_pressure = info->axes[2].max_value;
                }
                if (EventClass) {
                    ctx->event_classes[ctx->num_event_classes] = EventClass;
                    ctx->num_event_classes++;
                }
            } break;
        }

            ClassPtr = (XAnyClassPtr) ((uint8_t*)ClassPtr + ClassPtr->length);
        }

        XSelectExtensionEvent(disp, win, ctx->event_classes, ctx->num_event_classes);
    }

    XFreeDeviceList(Devices);

    if (ctx->device) {
        return EASYTAB_OK;
    } else {
        return EASYTAB_X11_ERROR;
    }
}

enum EasyTabResult easytab_handle_event(XEvent* event)
{
    if (event->type != ctx->motion_type) {
        return EASYTAB_EVENT_NOT_HANDLED;
    }

    XDeviceMotionEvent* me = (XDeviceMotionEvent*)(event);
    ctx->pos_x = me->x;
    ctx->pos_y = me->y;
    ctx->pressure = (float)me->axis_data[2] / (float)ctx->max_pressure;
    return EASYTAB_OK;
}

unsigned char easytab_is_button_down(enum EasyTabButton button)
{
    /* TODO: Implement */
    return 0;
}


void easytab_get_pos(int* out_x, int* out_y, int* out_range_x, int* out_range_y)
{
    *out_x = ctx->pos_x;
    *out_y = ctx->pos_y;
    *out_range_x = ctx->range_x;
    *out_range_y = ctx->range_y;
}

float easytab_get_pressure()
{
    return ctx->pressure;
}

void easytab_destroy()
{
    XCloseDevice(ctx->disp, ctx->device);
    free(ctx);
}

#endif // __linux__
