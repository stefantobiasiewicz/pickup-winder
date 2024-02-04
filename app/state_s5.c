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
	sprintf(line_1, "Summary: %-3s", machine_params.cw == true ? "CW" : "CCW");
	sprintf(line_2, "%-5d;%1.2f;%3.2f", machine_params.coil_turns,
			machine_params.wire_size, machine_params.distance);

	app_print(line_1, line_2);
}

void state_s5_change() {
	update_view();
}

machine_state_t state_s5_summary(signal_t *signal) {
	machine_state_t result = NO_CHANGE;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = STATE_S6;
		break;
	case '/':
		result = STATE_S41;
		break;
	default:
		break;
	}

	return result;
}
