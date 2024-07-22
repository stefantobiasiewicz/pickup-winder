/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "../winder_machine.h"
#include "winding_config/state_s2.h"
#include "settings/state_s11.h"
/*
 * State variables;
 */


static char line_1[20];
static char line_2[20];

static const menu_node_t menu[] = {
		{
				.name = "Wind coil",
				.state= &state_s2_cw_ccw_decision_state,
		},
		{
				.name = "Settings",
				.state= &state_s11_setting_page_state,
		}
};

static int menu_count = 2;

static int menu_iterator = 0;

static void update_view() {
	sprintf(line_1, "%s <", menu[menu_iterator].name);
	if(menu_count > 1 && menu_iterator < menu_count - 1) {
		sprintf(line_2, "%s", menu[(menu_iterator + 1)].name);
	} else {
		sprintf(line_2, "");
	}

	app_print(line_1, line_2);
}

void state_s1_change() {
	update_view();
}

const state_node_t* state_s1_main_page(signal_t *signal) {
	state_node_t* result = NULL;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = menu[menu_iterator].state;
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


state_node_t state_s1_main_page_state;

void state_s1_init() {
	state_s1_main_page_state.state_change_func = state_s1_change;
	state_s1_main_page_state.state_func = state_s1_main_page;
}

