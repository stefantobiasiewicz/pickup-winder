/*
 * state_esc_config.c
 *
 *  Created on: May 16, 2024
 *      Author: stefantobiasiewicz
 */

#include "../../winder_machine.h"
#include "state_esc_config.h"
#include "state_s11.h"

static char line_1[36];
static char line_2[36];


static int pwm = 0;

static void update_view() {
	sprintf(line_1, "Esc calibration");
	sprintf(line_2, "PWM: %d", pwm);

	app_print(line_1, line_2);
}

static void change() {
	motor_speed(100);
	pwm = 100;

	update_view();
}

static const state_node_t* state_fun(signal_t *signal) {
	state_node_t* result = NULL;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		motor_speed(0);
		pwm = 0;
		update_view();
		break;
	case '/':
		pwm = 0;
		motor_speed(0);
		result = &state_s11_setting_page_state;
		break;
	case '8':
			update_view();
			break;
	case '2':

		update_view();
		break;
	default:
		break;
	}

	return result;
}


state_node_t state_esc_config;

void state_esc_config_init() {
	state_esc_config.state_change_func = change;
	state_esc_config.state_func = state_fun;
}
