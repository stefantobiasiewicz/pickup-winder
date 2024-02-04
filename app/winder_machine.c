/*
 * winder_states.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "winder_machine.h"
#include "libs/FIFO.h"

machine_params_t machine_params;
machine_static_params_t machine_static_params;

static machine_state_t current_state = STATE_S05;
static fifo_t fifo;
static print_fun_t print;
static step_fun_t x_step_fun;
static step_fun_t y_step_fun;
static get_us_fun_t get_us_fun;
static motor_enable_fun_t motor_enable_fun;
static write_flash_fun_t write_flash_fun;
static read_flash_fun_t read_flash_fun;

state_node_t states[] = {
/* INIT */
{ STATE_S05, state_s05_init_flash, state_s05_change },
/* SETTINGS */
{ STATE_S1, state_s1_main_page, state_s1_change }, { STATE_S11,
		state_s11_settings_page, state_s11_change }, { STATE_S12,
		state_s12_settings_menu, state_s12_change }, { STATE_S13,
		state_s13_settings_xmotor, state_s13_change }, { STATE_S14,
		state_s14_settings_xscrew, state_s14_change }, { STATE_S15,
		state_s15_settings_ymove, state_s15_change },
/* MANIN WORKFLOW */
{ STATE_S2, state_s2_cw_ccw_decision, state_s2_change }, { STATE_S3,
		state_s3_coil_turns_decision, state_s3_change }, { STATE_S4,
		state_s4_wire_thick_decision, state_s4_change }, { STATE_S41,
		state_s41_distance_decision, state_s41_change }, { STATE_S5,
		state_s5_summary, state_s5_change }, { STATE_S6, state_s6_run,
		state_s6_change }, { STATE_S61, state_s61_cancel, state_s61_change }, {
		STATE_S7, state_s7_done, state_s7_change } };

state_node_t* getState(machine_state_t state) {
	for (int i = 0; i < sizeof(states) / sizeof(state_node_t); i++) {
		if (states[i].stateName == current_state) {

			return &states[i];
		}
	}

	return NULL;
}

void app_init(print_fun_t print_fun, step_fun_t x_step, step_fun_t y_step,
		get_us_fun_t get_us, motor_enable_fun_t motor_enable,
		write_flash_fun_t write_flash, read_flash_fun_t read_flash) {
	fifo = fifo_create(10, sizeof(signal_t));
	print = print_fun;
	x_step_fun = x_step;
	y_step_fun = y_step;
	get_us_fun = get_us;
	motor_enable_fun = motor_enable;
	write_flash_fun = write_flash;
	read_flash_fun = read_flash;
}

void app_print(char *_1st_line, char *_2nd_line) {
	print(_1st_line, _2nd_line);
}

void app_put_signal(signal_t signal) {
	fifo_add(fifo, &signal);
}

void app_state_machine_loop() {
	if (current_state == NO_CHANGE) {
		return;
	}

	state_node_t *state = getState(current_state);

	if (state == NULL) {
		//todo error handling
		return;
	}

	machine_state_t next;
	if (!fifo_is_empty(fifo)) {
		signal_t current_signal;
		fifo_get(fifo, &current_signal);

		next = state->state_func(&current_signal);
		if (next == NO_CHANGE) {
			return;
		}
	} else {
		next = state->state_func(NULL);
		if (next == NO_CHANGE) {
			return;
		}
	}

	current_state = next;
	state_node_t *next_state = getState(current_state);

	if (next_state == NULL) {
		//todo error handling
		return;
	}

	if (next_state->state_change_func != NULL) {
		next_state->state_change_func();
	}
}

void x_stepper_step(bool dir) {
	x_step_fun(dir);
}
void y_stepper_step(bool dir) {
	y_step_fun(dir);
}
void motor_enable(bool en) {
	motor_enable_fun(en);
}

long currentTimeUs() {
	return get_us_fun();
}

void read_flash(machine_static_params_t *machine_static_params) {
	read_flash_fun(machine_static_params);
}

void write_flash(machine_static_params_t *machine_static_params) {
	write_flash_fun(machine_static_params);
}
