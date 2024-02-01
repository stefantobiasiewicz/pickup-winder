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


static void update_view() {
	app_print("Wind coil?", ">");

}

void state_s1_change() {
	update_view();
}

machine_state_t state_s1_main_page(signal_t * signal) {
	machine_state_t result = NO_CHANGE;

	if(signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = STATE_S2;
		break;
	default:
		break;
	}


	return result;
}
