/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "winder_machine.h"
/*
 * State variables;
 */

void state_s05_change() {

}

machine_state_t state_s05_init_flash(signal_t *signal) {
	machine_state_t result = STATE_S1;

	read_flash(&machine_static_params);

	if (machine_static_params.init != INIT) {
		machine_static_params.init = INIT;
		machine_static_params.x_motor_steps = 1600;
		machine_static_params.x_screw = 8.f;

		write_flash(&machine_static_params);
	}

	return result;
}
