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

static char line_1[16];
static char line_2[16];

static void update_view() {
	sprintf(line_1, "Cancel");
	sprintf(line_2, "    ???");

	app_print(line_1, line_2);
}

void state_s61_change() {
	update_view();
}

machine_state_t state_s61_cancel(signal_t *signal) {
	machine_state_t result = NO_CHANGE;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = STATE_S1;
		machine_params.coil_turns = 0;
		machine_params.cw = true;
		machine_params.distance = 0;
		machine_params.manual = true;
		machine_params.speed = 1;
		machine_params.wire_size = 0;
		break;
	case '/':
		result = STATE_S6;
		break;
	default:
		break;
	}

	return result;
}
