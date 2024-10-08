#include <stdlib.h>
#include "page_manager.h"
#include "view.h"
#include "services/timestamp.h"
#include "theme/theme.h"
#include "theme/style.h"


static const char *TAG = "View";

static struct {
    pman_t        pman;
    lv_display_t *display;
} state = {
    .pman    = {},
    .display = NULL,
};


void view_init(model_t *model, lv_display_flush_cb_t flush_cb, lv_indev_read_cb_t read_cb) {
    (void)TAG;

    lv_init();

#ifdef BUILD_CONFIG_SIMULATED_APP
    (void)flush_cb;
    (void)read_cb;

    state.display =
        lv_sdl_window_create(BUILD_CONFIG_DISPLAY_HORIZONTAL_RESOLUTION, BUILD_CONFIG_DISPLAY_VERTICAL_RESOLUTION);
    lv_indev_t *touch_indev = lv_sdl_mouse_create();

#else
    state.display =
        lv_display_create(BUILD_CONFIG_DISPLAY_HORIZONTAL_RESOLUTION, BUILD_CONFIG_DISPLAY_VERTICAL_RESOLUTION);

    /*Static or global buffer(s). The second buffer is optional*/
    static lv_color_t buf_1[VIEW_LVGL_BUFFER_SIZE] = {0};

    /*Initialize `disp_buf` with the buffer(s). With only one buffer use NULL instead buf_2 */
    lv_display_set_buffers(state.display, buf_1, NULL, VIEW_LVGL_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(state.display, flush_cb);

    lv_indev_t *touch_indev = lv_indev_create();
    lv_indev_set_type(touch_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(touch_indev, read_cb);
#endif

    style_init();
    theme_init(state.display);

    pman_init(&state.pman, (void *)model, touch_indev, NULL, NULL);

    view_change_page(&page_main);
}


void view_change_page(const pman_page_t *page) {
    pman_change_page(&state.pman, *page);
}


void view_display_flush_ready(void) {
    if (state.display) {
        lv_display_flush_ready(state.display);
    }
}


mut_model_t *view_get_model(pman_handle_t handle) {
    return (mut_model_t *)pman_get_user_data(handle);
}


/*
void view_event(view_event_t event) {
    pman_event(&state.pman, (pman_event_t){.tag = PMAN_EVENT_TAG_USER, .as = {.user = &event}});
}
*/


void view_register_object_default_callback_with_number(lv_obj_t *obj, int id, int number) {
    uint16_t data = (id & 0xFF) | ((number & 0xFF) << 8);
    lv_obj_set_user_data(obj, (void *)(uintptr_t)data);

    pman_unregister_obj_event(obj);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_CLICKED);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_VALUE_CHANGED);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_RELEASED);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_PRESSED);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_PRESSING);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_LONG_PRESSED);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_LONG_PRESSED_REPEAT);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_CANCEL);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_READY);
}


void view_register_object_default_callback(lv_obj_t *obj, int id) {
    view_register_object_default_callback_with_number(obj, id, 0);
}


uint8_t view_get_obj_id(lv_obj_t *obj) {
    void *user_data = lv_obj_get_user_data(obj);
    if (user_data == NULL) {
        return 0;
    } else {
        return (uint8_t)(((uintptr_t)user_data) & 0xFF);
    }
}


uint8_t view_get_obj_number(lv_obj_t *obj) {
    void *user_data = lv_obj_get_user_data(obj);
    if (user_data == NULL) {
        return 0;
    } else {
        return (uint8_t)((((uintptr_t)user_data) & 0xFF00) >> 8);
    }
}
