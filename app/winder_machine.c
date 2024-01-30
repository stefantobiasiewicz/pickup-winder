/*
 * winder_states.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "winder_machine.h"
#include "libs/FIFO.h"

machine_params_t machine_params;

static machine_state_t current_state = STATE_S1;
static fifo_t fifo;
static print_fun_t print;


state_node_t states[] = {
		{STATE_S1, state_s1_main_page},
		{STATE_S2, state_s2_cw_ccw_decision},
		{STATE_S3, state_s3_coil_turns_decision},
		{STATE_S4, state_s4_wire_thick_decision},
		{STATE_S41, state_s41_distance_decision},
		{STATE_S5, state_s5_summary},
//		{STATE_S6, state_s6_run}
};


void app_init(print_fun_t print_fun){
	fifo = fifo_create(10, sizeof(signal_t));
	print = print_fun;
}


void app_print(char* _1st_line, char* _2nd_line) {
	print(_1st_line, _2nd_line);
}

void app_put_signal(signal_t signal) {
	fifo_add(fifo, &signal);
}

void app_state_machine_loop() {
	if(current_state == NO_CHANGE) {
		return;
	}


	for(int i = 0; i < sizeof(states); i++) {
		if (states[i].stateName == current_state) {
			signal_t current_signal;

			if (!fifo_is_empty(fifo)){
				fifo_get(fifo, &current_signal);

				machine_state_t next = states[i].state_func(&current_signal);
				if (next == NO_CHANGE) {
					return;
				}


				current_state = next;
				return;
			}
		}
	}
}

