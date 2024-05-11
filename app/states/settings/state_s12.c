/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "../../winder_machine.h"
/*
 * State variables;
 */

typedef struct {
	machine_state_t state;
	char *name;
} menu_t;

menu_t menu[] = { { STATE_S13, "X Steps" }, { STATE_S14, "X Screw" }, {
		STATE_S15, "Move X" }, 	{STATE_S16, "Moving X" }, {STATE_S12, " " } };
int8_t menu_iteator = 0;
#define MENU_COUNT 4

static char line_1[16];
static char line_2[16];

static void update_view() {
	sprintf(line_1, "%s <", menu[menu_iteator].name);
	sprintf(line_2, "%s", menu[(menu_iteator + 1)].name);

	app_print(line_1, line_2);
}

void state_s12_change() {
	update_view();
}

machine_state_t state_s12_settings_menu(signal_t *signal) {
	machine_state_t result = NO_CHANGE;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		// goto next state witch is menu state 
		result = menu[menu_iteator].state;
		break;
	case '8':
		menu_iteator = (menu_iteator - 1);
		if (menu_iteator <= 0)
			menu_iteator = 0;
		update_view();
		break;
	case '2':
		menu_iteator = (menu_iteator + 1);
		if (menu_iteator >= MENU_COUNT - 1)
			menu_iteator = MENU_COUNT - 1;
		update_view();
		break;
	case '/':
		result = STATE_S11;
		break;
	default:
		break;
	}

	return result;
}
