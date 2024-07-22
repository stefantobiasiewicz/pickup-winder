/*
 * states_def.c
 *
 *  Created on: Jul 11, 2024
 *      Author: stefantobiasiewicz
 */

#include "states/state_basic_alg_parameter.h"
#include "states/state_start.h"
#include "states/state_run.h"
#include "states/state_cross_alg_parameter.h"

void init_states() {
	state_basic_alg_parameter_init();
	state_start_init();
	state_run_init();
	state_cross_alg_parameter_init();
}
