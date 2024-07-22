/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "../../winder_machine.h"

#include "../state_s1.h"
#include "state_s3.h"
/*
 * State variables;
 */
static bool cw = true;

static void update_view() {
	if (cw) {
		app_print("CW or CCW", " >CW  CCW");
	} else {
		app_print("CW or CCW", "  CW >CCW");
	}
}

void state_s2_change() {
	update_view();
}

const state_node_t* state_s2_cw_ccw_decision(signal_t *signal) {
	state_node_t* result = NULL;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = &state_s3_coil_turns_decision_state;

		machine_params.cw = cw;
		break;
	case '6':
	case '4':
		cw = !cw;
		update_view();
		break;
	case '/':
		result = &state_s1_main_page_state;
		break;
	default:
		break;
	}

	return result;
}


state_node_t state_s2_cw_ccw_decision_state;

void state_s2_init() {
	state_s2_cw_ccw_decision_state.state_change_func = state_s2_change;
	state_s2_cw_ccw_decision_state.state_func = state_s2_cw_ccw_decision;
}
