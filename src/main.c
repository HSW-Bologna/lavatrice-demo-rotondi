#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "controller/controller.h"


int main(int argc, char *argv[]) {
    controller_init();

    printf("Init main loop\n");
    for (;;) {
        controller_manage();

        usleep(5 * 1000);
    }

    return 0;
}
