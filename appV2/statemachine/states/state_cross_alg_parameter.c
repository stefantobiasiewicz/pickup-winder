/*
 * state_cross_alg_parameter.c
 *
 *  Created on: Jul 13, 2024
 *      Author: stefantobiasiewicz
 */
#include "state_cross_alg_parameter.h"

#include <stdlib.h>
#include "../winder_machine.h"
#include "state_start.h"
#include "../../program.h"
#include "state_run.h"


static cross_alg_args args = {0};


static void state_change() {

}

static const state_node_t* state_cross_alg_parameter(signal_t *signal) {
	state_node_t* result = NULL;


	static int proggress = 0;

	if (proggress < 0) {
		proggress = 0;
		return &state_start_state;
	}
	if (proggress >= 3) {
		proggress = 0;
		//return next step

		cross_alg_provider.init(&args);
		return return_state_run(&cross_alg_provider);
	}


	switch(proggress) {
		case 0:
			proggress += fetch_parameter_f(signal, "Distance", &args.distance);
			break;
		case 1:
			proggress += fetch_parameter_i(signal, "Coil turns", &args.turns);
			break;
		case 2:
			proggress += fetch_parameter_i(signal, "cw", &args.cw);
			break;
	}



	return result;
}


state_node_t state_cross_alg_parameter_state;

void state_cross_alg_parameter_init() {
	state_cross_alg_parameter_state.state_change_func = state_change;
	state_cross_alg_parameter_state.state_func = state_cross_alg_parameter;
}
