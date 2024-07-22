/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "../../winder_machine.h"

#include "state_s4.h"
#include "../state_select_alg.h"
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

const state_node_t* state_s41_distance_decision(signal_t *signal) {
	state_node_t* result = NULL;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = &state_select_alg;

		machine_params.coil_distance = atof(number_buff);
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
		result = &state_s4_wire_thick_decision_state;
		break;
	default:
		break;
	}

	return result;
}



state_node_t state_s41_distance_decision_state;

void state_s41_init() {
	state_s41_distance_decision_state.state_change_func = state_s41_change;
	state_s41_distance_decision_state.state_func = state_s41_distance_decision;
}
