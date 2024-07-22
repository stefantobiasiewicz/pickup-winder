/*
 * state_run.h
 *
 *  Created on: Jul 11, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef STATEMACHINE_STATES_STATE_RUN_H_
#define STATEMACHINE_STATES_STATE_RUN_H_

#include "../states_def.h"
#include "../../program.h"

extern state_node_t state_run_state;

const state_node_t * return_state_run(gcode_provider_t * provider);
void state_run_current_gcode(g_code_t * code);
void state_run_program_finished();

void state_run_init();

#endif /* STATEMACHINE_STATES_STATE_RUN_H_ */
