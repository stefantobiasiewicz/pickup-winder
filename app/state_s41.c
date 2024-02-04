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
static char print_buff[16];
static char number_buff[10] = { 0 };

static void update_view() {
	sprintf(print_buff, ">%-10smm", number_buff);

	app_print("Coil width:", print_buff);
}

void state_s41_change() {
	update_view();
}

machine_state_t state_s41_distance_decision(signal_t *signal) {
	machine_state_t result = NO_CHANGE;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = STATE_S5;

		machine_params.distance = atof(number_buff);
		break;
	case '1':
		strcat(number_buff, "1");
		update_view();
		break;
	case '2':
		strcat(number_buff, "2");
		update_view();
		break;
	case '3':
		strcat(number_buff, "3");
		update_view();
		break;
	case '4':
		strcat(number_buff, "4");
		update_view();
		break;
	case '5':
		strcat(number_buff, "5");
		update_view();
		break;
	case '6':
		strcat(number_buff, "6");
		update_view();
		break;
	case '7':
		strcat(number_buff, "7");
		update_view();
		break;
	case '8':
		strcat(number_buff, "8");
		update_view();
		break;
	case '9':
		strcat(number_buff, "9");
		update_view();
		break;
	case '0':
		strcat(number_buff, "0");
		update_view();
		break;
	case '.':
		strcat(number_buff, ".");
		update_view();
		break;
	case '\r':
		number_buff[strlen(number_buff) - 1] = '\0';
		update_view();
		break;
	case '/':
		result = STATE_S4;
		break;
	default:
		break;
	}

	return result;
}
