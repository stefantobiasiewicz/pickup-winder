/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "../winder_machine.h"
/*
 * State variables;
 */

static char line_1[16];
static char line_2[16];

static void update_view() {
	sprintf(line_1, "Done! %s", machine_params.cw == true ? "CW" : "CCW");
	sprintf(line_2, "T:%4dws:%1.2fmm", machine_params.coil_turns,
			machine_params.wire_size);

	app_print(line_1, line_2);
}

void state_s7_change() {
	update_view();
}

machine_state_t state_s7_done(signal_t *signal) {
	machine_state_t result = NO_CHANGE;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '0':
	case '.':
	case '\r':
	case '/':
		result = STATE_S1;
		machine_params.coil_turns = 0;
		machine_params.cw = true;
		machine_params.coil_distance = 0;
		machine_params.manual = true;
		machine_params.speed = 1;
		machine_params.wire_size = 0;
		break;
	default:
		break;
	}

	return result;
}
