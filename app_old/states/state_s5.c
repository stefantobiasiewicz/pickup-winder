/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "../winder_machine.h"
#include "state_select_alg.h"
/*
 * State variables;
 */

static char line_1[36];
static char line_2[36];

static void update_view() {
	// todo Warning sprintf wypierdala wskaźniki na funkcje !!!!
	// jak dugosc tekstu wykorczy poza bufor to niszczy pamiec!!!!

	sprintf(line_1, "Summary: %-3s", machine_params.cw == true ? "CW" : "CCW");
	sprintf(line_2, "%-5d;%1.2f;%3.2f", machine_params.coil_turns,
			machine_params.wire_size, machine_params.coil_distance);

	app_print(line_1, line_2);
}

void state_s5_change() {
	update_view();
}

const state_node_t* state_s5_summary(signal_t *signal) {
	state_node_t* result = NULL;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = &state_s6_run_state;
		break;
	case '/':
		result = &state_select_alg;
		break;
	default:
		break;
	}

	return result;
}


state_node_t state_s5_summary_state;

void state_s5_init() {
	state_s5_summary_state.state_change_func = state_s5_change;
	state_s5_summary_state.state_func = state_s5_summary;
}


