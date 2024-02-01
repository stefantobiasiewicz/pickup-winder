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
static bool cw = true;



static void update_view() {
	if(cw) {
		app_print("CW or CCW", " >CW  CCW");
	} else {
		app_print("CW or CCW", "  CW >CCW");
	}
}

void state_s2_change() {
	update_view();
}

machine_state_t state_s2_cw_ccw_decision(signal_t * signal) {
	machine_state_t result = NO_CHANGE;


	if(signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = STATE_S3;

		machine_params.cw = cw;
		break;
	case '6':
	case '4':
		cw = !cw;
		update_view();
		break;
	case '/':
		result = STATE_S1;
		break;
	default:
		break;
	}


	return result;
}
