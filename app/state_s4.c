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
static bool ui_changed = true;

static char print_buff[16];
static char number_buff[10] = {0};

static void update_view() {
	if (ui_changed == false) {
		return;
	}

	sprintf(print_buff, ">%-10smm", number_buff);

	app_print("Wire thick:", print_buff);

	ui_changed = false;
}

machine_state_t state_s4_wire_thick_decision(signal_t *signal) {
	machine_state_t result = NO_CHANGE;

	switch (signal->key_pressed) {
	case '\n':
		result = STATE_S41;

		machine_params.wire_size = atof(number_buff);
		break;
	case '1':
		strcat(number_buff, "1");
		ui_changed = true;
		break;
	case '2':
	    strcat(number_buff, "2");
		ui_changed = true;
		break;
	case '3':
	    strcat(number_buff, "3");
		ui_changed = true;
		break;
	case '4':
	    strcat(number_buff, "4");
		ui_changed = true;
		break;
	case '5':
	    strcat(number_buff, "5");
		ui_changed = true;
		break;
	case '6':
	    strcat(number_buff, "6");
		ui_changed = true;
		break;
	case '7':
	    strcat(number_buff, "7");
		ui_changed = true;
		break;
	case '8':
	    strcat(number_buff, "8");
		ui_changed = true;
		break;
	case '9':
	    strcat(number_buff, "9");
		ui_changed = true;
		break;
	case '0':
	    strcat(number_buff, "0");
		ui_changed = true;
		break;
	case '.':
	    strcat(number_buff, ".");
		ui_changed = true;
		break;
	case '-':
		number_buff[strlen(number_buff)-1] = '\0';
		ui_changed = true;
		break;
	case '\r':
		result = STATE_S3;
		break;
	default:
		break;
	}

	update_view();

	return result;
}
