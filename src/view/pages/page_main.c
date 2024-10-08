#include <stdio.h>
#include <assert.h>
#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"


#define NUM_PROGRAMS 6


struct page_data {
    lv_obj_t *buttons[NUM_PROGRAMS];
    lv_obj_t *button_start;

    lv_obj_t *label_name;
    lv_obj_t *label_temperatures[NUM_PROGRAMS];

    int16_t selected_program;
};


enum {
    BTN_PROGRAM_ID,
    BTN_START_ID,
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
    pdata->selected_program = -1;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    lv_obj_t *cont = lv_obj_create(lv_screen_active());
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_add_style(cont, &style_padless_cont, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont, STYLE_COLOR_WHITE, LV_STATE_DEFAULT);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    {
        lv_obj_t *programs_cont = lv_obj_create(cont);
        lv_obj_remove_flag(programs_cont, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_style(programs_cont, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(programs_cont, 32, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(programs_cont, 8, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(programs_cont, 32, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(programs_cont, 32, LV_STATE_DEFAULT);
        lv_obj_set_size(programs_cont, LV_PCT(80), LV_PCT(100));
        lv_obj_set_layout(programs_cont, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(programs_cont, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(programs_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_align(programs_cont, LV_ALIGN_TOP_LEFT, 0, LV_PCT(15));

        for (uint16_t i = 0; i < NUM_PROGRAMS; i++) {
            lv_obj_t *button = lv_button_create(programs_cont);
            lv_obj_add_flag(button, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_set_height(button, LV_PCT(100));
            lv_obj_set_flex_grow(button, 1);
            lv_obj_set_style_radius(button, 32, LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(button, 6, LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(button, STYLE_COLOR_WHITE, LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(button, STYLE_COLOR_GRAY, LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(button, 4, LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(button, lv_color_darken(STYLE_COLOR_GRAY, LV_OPA_20), LV_STATE_DEFAULT);

            lv_obj_set_style_bg_color(button, STYLE_COLOR_WHITE, LV_STATE_CHECKED);
            lv_obj_set_style_pad_bottom(button, 0, LV_STATE_CHECKED);
            lv_obj_set_style_border_width(button, 10, LV_STATE_CHECKED);
            lv_obj_set_style_border_color(button, STYLE_COLOR_TEXT, LV_STATE_CHECKED);
            lv_obj_set_style_text_color(button, STYLE_COLOR_TEXT, LV_STATE_CHECKED);

            view_register_object_default_callback_with_number(button, BTN_PROGRAM_ID, i);

            lv_obj_t *label = lv_label_create(button);
            lv_label_set_text_fmt(label, "%i°", temperatures[i]);
            lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -96);

            pdata->buttons[i]            = button;
            pdata->label_temperatures[i] = label;
        }
    }

    lv_obj_t *label_name = lv_label_create(cont);
    lv_obj_set_style_text_color(label_name, STYLE_COLOR_TEXT, LV_STATE_DEFAULT);
    lv_obj_align(label_name, LV_ALIGN_TOP_LEFT, 64, 0);
    pdata->label_name = label_name;

    lv_obj_t *button_start = lv_button_create(cont);
    lv_obj_set_size(button_start, 128, 128);
    lv_obj_align(button_start, LV_ALIGN_RIGHT_MID, 0, 0);
    view_register_object_default_callback(button_start, BTN_START_ID);
    pdata->button_start = button_start;

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    struct page_data *pdata = state;

    mut_model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_OPEN:
            break;

        case PMAN_EVENT_TAG_USER: {
            break;
        }

        case PMAN_EVENT_TAG_LVGL: {
            lv_obj_t           *target   = lv_event_get_current_target_obj(event.as.lvgl);
            view_object_data_t *obj_data = lv_obj_get_user_data(target);

            switch (lv_event_get_code(event.as.lvgl)) {
                case LV_EVENT_CLICKED: {
                    switch (view_get_obj_id(target)) {
                        case BTN_START_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(&page_washing,
                                                                           (void *)(uintptr_t)pdata->selected_program);
                            break;

                        case BTN_PROGRAM_ID:
                            if (pdata->selected_program == view_get_obj_number(target)) {
                                pdata->selected_program = -1;
                            } else {
                                pdata->selected_program = view_get_obj_number(target);
                            }
                            update_page(model, pdata);
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

    for (uint16_t i = 0; i < NUM_PROGRAMS; i++) {
        if (pdata->selected_program == i) {
            lv_obj_add_state(pdata->buttons[i], LV_STATE_CHECKED);
            lv_label_set_text_fmt(pdata->label_name, "%i° %s", temperatures[i], descriptions[i]);
        } else {
            lv_obj_clear_state(pdata->buttons[i], LV_STATE_CHECKED);
        }
    }

    if (pdata->selected_program == -1) {
        lv_obj_add_flag(pdata->label_name, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_state(pdata->button_start, LV_STATE_DISABLED);
    } else {
        lv_obj_remove_flag(pdata->label_name, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_state(pdata->button_start, LV_STATE_DISABLED);
    }
}


const pman_page_t page_main = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
