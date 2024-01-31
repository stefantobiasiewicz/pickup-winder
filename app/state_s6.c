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

static int actual_turns = 0;


static char line_1[16];
static char line_2[16];

static void update_view() {
	sprintf(line_1, "%-3s F%2d %s", machine_params.cw == true ? "CW" : "CCW", machine_params.speed, machine_params.manual == true ? "MANUAL" : "AUTO");
	sprintf(line_2, "%-5dT   %-5dT", actual_turns,
			machine_params.coil_turns);

	app_print(line_1, line_2);
}

void state_s6_change() {
	machine_params.speed = 1;
	machine_params.manual = true;

	actual_turns = 0;

	update_view();
}

machine_state_t state_s6_run(signal_t *signal) {
	machine_state_t result = NO_CHANGE;

	switch (signal->key_pressed) {
	case '\n':
		machine_params.manual = !machine_params.manual;
		update_view();
		break;
	case '2':
		if (machine_params.speed > 1) {
			machine_params.speed--;
		}
		update_view();
		break;
	case '8':
		if (machine_params.speed < 10) {
			machine_params.speed++;
		}
		update_view();
		break;
	default:
		break;
	}

	//todo running application

	return result;
}


typedef enum {
  STOP,
  BEGGIN,
  ROTATE_COIL,
  MOVE_WIRE,
  CHANGE_DIR
} wind_process_t;
