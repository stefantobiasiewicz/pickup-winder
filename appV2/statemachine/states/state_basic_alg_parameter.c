/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */


/*
 * State variables;
 */
#include "state_basic_alg_parameter.h"
#include <stdlib.h>
#include "../winder_machine.h"
#include "state_start.h"
#include "../../program.h"
#include "state_run.h"

static basic_alg_args args = {0};


static void state_change() {

}

static const state_node_t* state_basic_alg_parameter(signal_t *signal) {
	state_node_t* result = NULL;


	static int proggress = 0;

	if (proggress < 0) {
		proggress = 0;
		return &state_start_state;
	}
	if (proggress >= 4) {
		proggress = 0;
		//return next step

		basic_alg_provider.init(&args);
		return return_state_run(&basic_alg_provider);
	}


	switch(proggress) {
		case 0:
			proggress += fetch_parameter_f(signal, "Distance", &args.distance);
			break;
		case 1:
			proggress += fetch_parameter_i(signal, "Coil turns", &args.turns);
			break;
		case 2:
			proggress += fetch_parameter_f(signal, "step", &args.distance_step);
			break;
		case 3:
			proggress += fetch_parameter_i(signal, "cw", &args.cw);
			break;
	}



	return result;
}


state_node_t state_basic_alg_parameter_state;

void state_basic_alg_parameter_init() {
	state_basic_alg_parameter_state.state_change_func = state_change;
	state_basic_alg_parameter_state.state_func = state_basic_alg_parameter;
}



