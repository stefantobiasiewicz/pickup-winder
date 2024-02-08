/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "winder_machine.h"
/*
 * State variables;
 */

static char line_1[16];
static char line_2[16];
long time;

int y_steps;
bool dir = true;

static void update_view() {
	sprintf(line_1, "Y moving %d", machine_static_params.x_motor_steps);
	sprintf(line_2, "steps" );

	app_print(line_1, line_2);
}

void state_s16_change() {
	update_view();

	motor_enable(true);


	y_steps = machine_static_params.x_motor_steps;
	dir = true;
	time = currentTimeUs();
}

machine_state_t state_s16_settings_ymoving(signal_t *signal) {
	machine_state_t result = NO_CHANGE;

	long long currentTime = currentTimeUs();
	long long elapsedTime = currentTime - time;

	if (elapsedTime >= 100) {
		y_stepper_step(dir);
		y_steps--;
		time = currentTime;

		if(y_steps <= 0) {
			dir = !dir;
			y_steps = machine_static_params.x_motor_steps;
		}
	}


	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = STATE_S12;

		motor_enable(false);
		break;
	case '/':
		result = STATE_S12;
		motor_enable(false);
		break;
	default:
		break;
	}


	return result;
}
