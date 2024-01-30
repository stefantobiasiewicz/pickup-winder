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
static bool ui_changed = true;

static char line_1[16];
static char line_2[16];

static void update_view() {
	if (ui_changed == false) {
		return;
	}

	sprintf(line_1, "Start?     %s", machine_params.cw == true ? "CW" : "CCW");
	sprintf(line_2, "%-5dT   %.2fmm", machine_params.coil_turns, machine_params.wire_size);

	app_print(line_1, line_2);

	ui_changed = false;
}

machine_state_t state_s5_summary(signal_t *signal) {
	machine_state_t result = NO_CHANGE;

	switch (signal->key_pressed) {
	case '\n':
		result = STATE_S6;
		break;
	default:
		break;
	}

	update_view();

	return result;
}
