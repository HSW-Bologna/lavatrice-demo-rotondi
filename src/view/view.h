#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED


#include "page_manager.h"
#include "model/model.h"
#include "lvgl.h"


typedef uint16_t view_object_data_t;


void         view_init(model_t *model, lv_display_flush_cb_t flush_cb, lv_indev_read_cb_t read_cb);
uint8_t      view_get_obj_id(lv_obj_t *obj);
uint8_t      view_get_obj_number(lv_obj_t *obj);
mut_model_t *view_get_model(pman_handle_t handle);
void         view_change_page(const pman_page_t *page);
void         view_register_object_default_callback_with_number(lv_obj_t *obj, int id, int number);
void         view_register_object_default_callback(lv_obj_t *obj, int id);


extern const pman_page_t page_main, page_washing;


#endif
