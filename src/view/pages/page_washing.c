#include <stdio.h>
#include <assert.h>
#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "services/timestamp.h"
#include "stopwatch.h"


#define NUM_PROGRAMS 6


struct page_data {
    lv_obj_t *label_name;
    lv_obj_t *label_time;

    lv_obj_t *arc;

    lv_obj_t *button_control;
    lv_obj_t *button_stop;

    pman_timer_t *timer;

    int16_t     selected_program;
    stopwatch_t stopwatch;
};


enum {
    BTN_STOP_ID,
    BTN_CONTROL_ID,
    TIMER_REFRESH_ID,
};


static void update_page(model_t *model, struct page_data *pdata);


static const uint16_t temperatures[NUM_PROGRAMS] = {60, 30, 30, 90, 40, 20};
static const char    *descriptions[NUM_PROGRAMS] = {
    "Molto Sporchi", "Lana", "Prelavaggio", "Molto Sporchi", "Colorati", "A Mano",
};


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);
    pdata->selected_program = (int16_t)(uintptr_t)extra;

    pdata->timer = PMAN_REGISTER_TIMER_ID(handle, 250, 0);
    stopwatch_init(&pdata->stopwatch);
    stopwatch_set(&pdata->stopwatch, 10 * 60 * 1000);
    stopwatch_resume(&pdata->stopwatch, timestamp_get());

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    pman_timer_resume(pdata->timer);

    lv_obj_t *cont = lv_obj_create(lv_screen_active());
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(cont, &style_padless_cont, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont, STYLE_COLOR_WHITE, LV_STATE_DEFAULT);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *label_name = lv_label_create(cont);
    lv_obj_set_style_text_color(label_name, STYLE_COLOR_TEXT, LV_STATE_DEFAULT);
    lv_obj_align(label_name, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text_fmt(label_name, "%i° %s", temperatures[pdata->selected_program],
                          descriptions[pdata->selected_program]);
    pdata->label_name = label_name;

    lv_obj_t *arc = lv_arc_create(lv_screen_active());
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_start_angle(arc, 0);
    lv_arc_set_end_angle(arc, 360);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_arc_width(arc, 52, LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(arc, 0, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, STYLE_COLOR_GRAY, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, STYLE_COLOR_GREEN, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(arc, 48, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(arc, 0, LV_PART_INDICATOR);
    lv_obj_align(arc, LV_ALIGN_LEFT_MID, 32, 0);
    lv_arc_set_range(arc, 0, 10 * 60);
    lv_obj_set_size(arc, 380, 380);
    pdata->arc = arc;

    lv_obj_t *label_time = lv_label_create(lv_screen_active());
    lv_obj_align_to(label_time, arc, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(label_time, STYLE_COLOR_TEXT, LV_STATE_DEFAULT);
    pdata->label_time = label_time;

    lv_obj_t *button_control = lv_button_create(cont);
    lv_obj_set_size(button_control, 128, 128);
    lv_obj_align(button_control, LV_ALIGN_RIGHT_MID, 0, 0);
    view_register_object_default_callback(button_control, BTN_CONTROL_ID);
    pdata->button_control = button_control;

    lv_obj_t *button_stop = lv_button_create(cont);
    lv_obj_set_size(button_stop, 96, 96);
    lv_obj_align_to(button_stop, button_control, LV_ALIGN_OUT_TOP_MID, 0, -8);
    view_register_object_default_callback(button_stop, BTN_STOP_ID);
    pdata->button_stop = button_stop;

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    struct page_data *pdata = state;

    mut_model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_OPEN: {
            break;
        }

        case PMAN_EVENT_TAG_USER: {
            break;
        }

        case PMAN_EVENT_TAG_TIMER: {
            update_page(model, pdata);
            break;
        }

        case PMAN_EVENT_TAG_LVGL: {
            lv_obj_t           *target   = lv_event_get_current_target_obj(event.as.lvgl);
            view_object_data_t *obj_data = lv_obj_get_user_data(target);

            switch (lv_event_get_code(event.as.lvgl)) {
                case LV_EVENT_CLICKED: {
                    switch (view_get_obj_id(target)) {
                        case BTN_CONTROL_ID:
                            if (stopwatch_is_paused(&pdata->stopwatch)) {
                                stopwatch_resume(&pdata->stopwatch, timestamp_get());
                            } else {
                                stopwatch_pause(&pdata->stopwatch, timestamp_get());
                            }
                            update_page(model, pdata);
                            break;

                        case BTN_STOP_ID:
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;
                    }
                    break;
                }

                default:
                    break;
            }
            break;
        }

        default:
            break;
    }

    return msg;
}


static void update_page(model_t *model, struct page_data *pdata) {
    (void)model;
    (void)pdata;

    if (pdata->selected_program == -1) {
        lv_obj_add_flag(pdata->label_name, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_remove_flag(pdata->label_name, LV_OBJ_FLAG_HIDDEN);
    }

    timestamp_t remaining_ms      = stopwatch_get_remaining(&pdata->stopwatch, timestamp_get());
    uint16_t    remaining_seconds = remaining_ms / 1000;
    uint16_t    remaining_minutes = remaining_seconds / 60;
    lv_label_set_text_fmt(pdata->label_time, "%02i:%02i", remaining_minutes, remaining_seconds % 60);

    lv_arc_set_value(pdata->arc, remaining_seconds);

    if (stopwatch_is_paused(&pdata->stopwatch)) {
        lv_obj_remove_flag(pdata->button_stop, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(pdata->button_stop, LV_OBJ_FLAG_HIDDEN);
    }
}


static void destroy_page(void *state, void *extra) {
    (void)extra;

    struct page_data *pdata = state;
    pman_timer_delete(pdata->timer);

    lv_free(state);
}


static void close_page(void *state) {
    struct page_data *pdata = state;
    pman_timer_pause(pdata->timer);
    lv_obj_clean(lv_scr_act());
}


const pman_page_t page_washing = {
    .create        = create_page,
    .destroy       = destroy_page,
    .open          = open_page,
    .close         = close_page,
    .process_event = page_event,
};
