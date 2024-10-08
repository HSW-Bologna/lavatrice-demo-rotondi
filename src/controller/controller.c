#include "controller.h"
#include "model/model.h"
#include "view/view.h"


void controller_init(void) {
    mut_model_t model = {};

    view_init(&model, NULL, NULL);
}


void controller_manage(void) {
    #ifndef BUILD_CONFIG_SIMULATED_APP
    static timestamp_t last_invoked = 0;

    if (last_invoked != timestamp_get()) {
        if (last_invoked > 0) {
            lv_tick_inc(timestamp_interval(last_invoked, timestamp_get()));
        }
        last_invoked = timestamp_get();
    }
#endif

    lv_timer_handler();
}
