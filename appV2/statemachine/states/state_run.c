/*
 * state_run.c
 *
 *  Created on: Jul 11, 2024
 *      Author: stefantobiasiewicz
 */

#include <stdlib.h>

#include "state_start.h"
#include "state_run.h"
#include "state_basic_alg_parameter.h"

#include "../winder_machine.h"
#include "../../program.h"
#include "../../machine_motion.h"

static char line_1[20];
static char line_2[20];

static int speed = 100;
static bool work = 0;
static g_code_t *code;

static bool program_finsihed = false;

static void update_view() {
	sprintf(line_1, "S: %d W:%d", speed, work);
	if (code != NULL) {
		sprintf(line_2, "G1X%2.1f A%4.1f", code->X, code->A);
	} else {
		sprintf(line_2, "---No Code---");
	}

	app_print(line_1, line_2);
}

static void state_change() {
	update_view();
}

static const state_node_t* state_fun(signal_t *signal) {
	state_node_t *result = NULL;

	//handle
	static int offset_screens = 0;
	int res = 0;
	switch(offset_screens) {
		case 1:
			res = fetch_parameter_f(signal, "Offset plus", &offsets.offset_plus);
			if (res == 1 || res == -1) {
				offset_screens = 0;
				update_view();
			}
			return NULL;
		case 2:
			res = fetch_parameter_f(signal, "Offset minus", &offsets.offset_minus);
			if (res == 1 || res == -1) {
				offset_screens = 0;
				update_view();
			}
			return NULL;
		default:
			break;
	}


	if (signal == NULL) {
		return result;
	}

	if (program_finsihed) {
		return &state_start_state;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		work = work == true ? false : true;
		machine_set_work(work);
		update_view();
		break;
	case '8':
		speed -= 5;
		machine_motion_set_speed_time(speed);
		update_view();
		break;
	case '2':
		speed += 5;
		machine_motion_set_speed_time(speed);
		update_view();
		break;
	case '7':
		offset_screens = 1;
		break;
	case '4':
		offset_screens = 2;
		break;
	default:
		break;
	}

	return result;
}

state_node_t state_run_state;

void state_run_init() {
	state_run_state.state_change_func = state_change;
	state_run_state.state_func = state_fun;
}

const state_node_t* return_state_run(gcode_provider_t *provider) {
	program_start(provider);
	work = false;
	machine_set_work(work);
	program_finsihed = false;

	return & state_run_state;
}

void state_run_current_gcode(g_code_t *gcode) {
	code = gcode;
	update_view();
}


void state_run_program_finished() {
	program_finsihed = true;

	sprintf(line_1, "----Winding----");
	sprintf(line_2, "-----Done!-----");

	app_print(line_1, line_2);
}
