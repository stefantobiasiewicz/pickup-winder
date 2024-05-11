/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "../winder_machine.h"
#include "../machine/machine.h"
#include "../alg/alg.h"
/*
 * State variables;
 */

static int actual_turns = 0;

static char line_1[24];
static char line_2[24];

init_alg_fun_t alg_init = init_basic_alg;
alg_fun_t alg_fun = basic_alg;

void default_view()
{
    sprintf(line_1, "%-5s%2dF%8s", machine_params.cw == true ? "CW" : "CCW",
        machine_params.speed,
        machine_params.manual == true ? "MANUAL" : "AUTO");
    sprintf(line_2, "%-8d%8d", actual_turns, machine_params.coil_turns);
}

void machine_params_view()
{
    // sprintf(line_1, "%-5s%2dF%8s", machine_params.cw == true ? "CW" : "CCW",
    //     machine_params.speed,
    //     machine_params.manual == true ? "MANUAL" : "AUTO");
    // sprintf(line_2, "%-8d%8d", turns, machine_params.coil_turns);
}

static void update_view()
{
    default_view();

    app_print(line_1, line_2);
}

void state_s6_change()
{
    machine_params.speed = 1;
    machine_params.manual = true;

    actual_turns = 0;

    update_view();
    machine_reset();
    alg_init(machine_params.coil_distance, machine_params.wire_size, NULL);
}

machine_state_t state_s6_run(signal_t* signal)
{
    machine_state_t result = NO_CHANGE;

   	machine_main_loop();

	actual_turns = machine_get_turns();
    if (machine_params.coil_turns <= actual_turns) {
        machine_stop();
        return STATE_S7;
    }



    if (signal == NULL) {
        return result;
    }

    switch (signal->key_pressed) {
    case '\n':
        machine_params.manual = !machine_params.manual;
        motor_enable(!machine_params.manual);
        update_view();
        break;
    case '2':
        if (machine_params.speed > 1) {
            machine_params.speed--;
        }
        update_view();
        break;
    case '8':
        if (machine_params.speed < 10) {
            machine_params.speed++;
        }
        update_view();
        break;
    case '/':
        result = STATE_S61;
        break;
    default:
        break;
    }

    return result;
}


/*
 * entrypoint from one rotation has been made.
 */
void app_machine_rotated_indicator()
{
    machine_rotated_indicator();
}
