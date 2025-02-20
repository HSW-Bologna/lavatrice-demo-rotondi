#include "lvgl.h"
#include "style.h"


static const lv_style_const_prop_t style_transparent_cont_props[] = {
    LV_STYLE_CONST_PAD_BOTTOM(0), LV_STYLE_CONST_PAD_TOP(0),      LV_STYLE_CONST_PAD_LEFT(0),
    LV_STYLE_CONST_PAD_RIGHT(0),  LV_STYLE_CONST_BORDER_WIDTH(0), LV_STYLE_CONST_BG_OPA(LV_OPA_TRANSP),
    LV_STYLE_CONST_PROPS_END,
};
LV_STYLE_CONST_INIT(style_transparent_cont, (void *)style_transparent_cont_props);


static const lv_style_const_prop_t style_padless_cont_props[] = {
    LV_STYLE_CONST_PAD_BOTTOM(0), LV_STYLE_CONST_PAD_TOP(0),    LV_STYLE_CONST_PAD_LEFT(0), LV_STYLE_CONST_PAD_RIGHT(0),
    LV_STYLE_CONST_PAD_ROW(0),    LV_STYLE_CONST_PAD_COLUMN(0), LV_STYLE_CONST_PROPS_END,
};
LV_STYLE_CONST_INIT(style_padless_cont, (void *)style_padless_cont_props);


static const lv_style_const_prop_t style_borderless_cont_props[] = {
    LV_STYLE_CONST_BORDER_WIDTH(0),
    LV_STYLE_CONST_RADIUS(0),
    LV_STYLE_CONST_PROPS_END,
};
LV_STYLE_CONST_INIT(style_borderless_cont, (void *)style_borderless_cont_props);


static const lv_style_const_prop_t style_black_border_props[] = {
    LV_STYLE_CONST_BORDER_WIDTH(4),
    LV_STYLE_CONST_BORDER_COLOR(STYLE_COLOR_BLACK),
    LV_STYLE_CONST_RADIUS(16),
    LV_STYLE_CONST_PROPS_END,
};
LV_STYLE_CONST_INIT(style_black_border, (void *)style_black_border_props);

static const lv_style_const_prop_t style_config_btn_props[] = {
    LV_STYLE_CONST_BG_COLOR(STYLE_PRIMARY_DARKER),
    LV_STYLE_CONST_PROPS_END,
};
LV_STYLE_CONST_INIT(style_config_btn, (void *)style_config_btn_props);


static const lv_style_const_prop_t style_white_icon_props[] = {
    LV_STYLE_CONST_IMAGE_RECOLOR_OPA(LV_OPA_COVER),
    LV_STYLE_CONST_IMAGE_RECOLOR(LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)),
    LV_STYLE_CONST_PROPS_END,
};
LV_STYLE_CONST_INIT(style_white_icon, (void *)style_white_icon_props);


void style_init(void) {}
