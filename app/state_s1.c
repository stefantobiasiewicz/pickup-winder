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


static void update_view() {
	if (ui_changed == false) {
		return;
	}

	app_print("Wind coil?", ">");

	ui_changed = false;
}

machine_state_t state_s1_main_page(signal_t * signal) {
	machine_state_t result = NO_CHANGE;

	switch (signal->key_pressed) {
	case '\n':
		result = STATE_S2;
		break;
	default:
		break;
	}

	update_view();

	return result;
}
