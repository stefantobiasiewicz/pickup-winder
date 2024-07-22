/*
 * state_rotor_test.c
 *
 *  Created on: May 16, 2024
 *      Author: stefantobiasiewicz
 */

#include "state_rotor_test.h"
#include "state_s11.h"

#include "../../winder_machine.h"

static char line_1[36];
static char line_2[36];

static int pwm = 0;

static void update_view() {
	sprintf(line_1, "Rotor test");
	sprintf(line_2, "%d%%", pwm);

	app_print(line_1, line_2);
}

static void change() {
	pwm = 0;
	motor_speed(pwm);

	update_view();
}

static const state_node_t* state_fun(signal_t *signal) {
	state_node_t *result = NULL;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		pwm = 0;
		motor_speed(pwm);
		update_view();
		break;
	case '/':
		pwm = 0;
		motor_speed(pwm);
		result = &state_s11_setting_page_state;
		break;
	case '8':
		pwm += 5;

		if (pwm >= 100) {
			pwm = 100;
		}

		motor_speed(pwm);

		update_view();
		break;
	case '2':
		pwm -= 5;
		if (pwm <= 0) {
			pwm = 0;
		}

		motor_speed(pwm);

		update_view();
		break;
	default:
		break;
	}

	return result;
}

state_node_t state_rotor_test;

void state_rotor_test_init() {
	state_rotor_test.state_change_func = change;
	state_rotor_test.state_func = state_fun;
}
