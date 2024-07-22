/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "state_basic_alg_parameter.h"
#include "state_cross_alg_parameter.h"
#include <stdlib.h>
#include "../winder_machine.h"
#include "state_start.h"
/*
 * State variables;
 */


static char line_1[20];
static char line_2[20];

static const menu_node_t menu[] = {
		{
				.name = "Wind Basic",
				.state= &state_basic_alg_parameter_state,
		},
		{
				.name = "Wind Cross",
				.state= &state_cross_alg_parameter_state,
		},
		{
				.name = "Settings",
				.state= NULL,
		}
};

static const int menu_count = sizeof(menu) / sizeof(menu_node_t);

static int menu_iterator = 0;

static void update_view() {
	sprintf(line_1, "%s <", menu[menu_iterator].name);
	if(menu_count > 1 && menu_iterator < menu_count - 1) {
		sprintf(line_2, "%s", menu[(menu_iterator + 1)].name);
	} else {
		sprintf(line_2, " ");
	}

	app_print(line_1, line_2);
}

static void state_change() {
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
		if (menu[menu_iterator].state != NULL) {
			result = menu[menu_iterator].state;
		}
		break;
	case '8':
		menu_iterator = (menu_iterator - 1);
			if (menu_iterator <= 0)
				menu_iterator = 0;
			update_view();
			break;
	case '2':
		menu_iterator = (menu_iterator + 1);
		if (menu_iterator >= menu_count - 1)
			menu_iterator = menu_count - 1;
		update_view();
		break;
	default:
		break;
	}

	return result;
}


state_node_t state_start_state;

void state_start_init() {
	state_start_state.state_change_func = state_change;
	state_start_state.state_func = state_fun;
}

