/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "../winder_machine.h"
#include "state_s1.h"
/*
 * State variables;
 */


void state_s05_change() {

}

const state_node_t* state_s05_init_flash(signal_t *signal) {
	state_node_t* result = &state_s1_main_page_state;

	read_flash(&machine_static_params);

	if (machine_static_params.init != INIT) {
		machine_static_params.init = INIT;
		machine_static_params.x_motor_steps = 1600;
		machine_static_params.x_screw = 8.f;

		write_flash(&machine_static_params);
	}

	return result;
}

state_node_t state_s05_init_flash_state;

void state_s05_init_flash_state_init() {
	state_s05_init_flash_state.state_change_func = state_s05_change;
	state_s05_init_flash_state.state_func = state_s05_init_flash;
}
