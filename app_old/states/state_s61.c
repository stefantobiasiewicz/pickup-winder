/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "../winder_machine.h"
#include "state_s6.h"
#include "state_s1.h"
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

const state_node_t* state_s61_cancel(signal_t *signal) {
	state_node_t* result = NULL;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = &state_s1_main_page_state;
		machine_params.coil_turns = 0;
		machine_params.cw = true;
		machine_params.coil_distance = 0;
		machine_params.manual = true;
		machine_params.speed = 1;
		machine_params.wire_size = 0;
		break;
	case '/':
		result = &state_s6_run_state;
		break;
	default:
		break;
	}

	return result;
}


state_node_t state_s61_cancel_state;

void state_s61_init() {
	state_s61_cancel_state.state_change_func = state_s61_change;
	state_s61_cancel_state.state_func = state_s61_cancel;
}
