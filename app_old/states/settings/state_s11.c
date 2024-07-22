/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "../../winder_machine.h"
#include "state_esc_config.h"
#include "state_rotor_test.h"
/*
 * State variables;
 */

static char line_1[20];
static char line_2[20];

//	menu[1] = (menu_t ) { &state_s14_settings_xscrew_state, "X Screw" };
//	menu[2] = (menu_t ) { &state_s15_settings_ymove_state, "Move X" };
//	menu[3] = (menu_t ) { &state_s16_settings_ymoving_state, "Moving X" };
//	menu[4] = (menu_t ) { &state_s12_settings_menu_state, " " }; // Zakładam, że " " jest miejscem na menu

const static menu_node_t menu[] = {
		{
				.name = "calibrate ESC",
				.state= &state_esc_config,
		},
		{
				.name = "Test Rotor",
				.state= &state_rotor_test,
		},
		{
				.name = "X Screw",
				.state= NULL,
		},
		{
				.name = "Move X",
				.state= NULL,
		},
};

static int menu_count = 4;

static int menu_iterator = 0;

static void update_view() {
	sprintf(line_1, "%s <", menu[menu_iterator].name);
	if(menu_count > 1 && menu_iterator < menu_count) {
		sprintf(line_2, "%s", menu[(menu_iterator + 1)].name);
	} else {
		sprintf(line_2, "");
	}

	app_print(line_1, line_2);
}

void state_s11_change() {
	update_view();
}

const state_node_t* state_s11_setting_page(signal_t *signal) {
	state_node_t* result = NULL;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		result = menu[menu_iterator].state;
		break;
	case '/':
		result = &state_s1_main_page_state;
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


state_node_t state_s11_setting_page_state;

void state_s11_init() {
	state_s11_setting_page_state.state_change_func = state_s11_change;
	state_s11_setting_page_state.state_func = state_s11_setting_page;
}

