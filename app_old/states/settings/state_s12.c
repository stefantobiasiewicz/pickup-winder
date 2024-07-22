///*
// * state_s1.c
// *
// *  Created on: Jan 30, 2024
// *      Author: stefantobiasiewicz
// */
//
//#include "../../winder_machine.h"
///*
// * State variables;
// */
//
//typedef struct {
//	const state_node_t *state;
//	char *name;
//} menu_t;
//
//extern const state_node_t state_s13_settings_xmotor_state;
//extern const state_node_t state_s14_settings_xscrew_state;
//extern const state_node_t state_s15_settings_ymove_state;
//extern const state_node_t state_s16_settings_ymoving_state;
//extern const state_node_t state_s12_settings_menu_state;
//
//// Deklaracja globalnej tablicy menu i iteratora
//menu_t menu[5];  // Zmieniona deklaracja na nieconst
//int8_t menu_iterator = 0; // Dobrze byłoby zachować nazwę konsekwentną, np. menu_iterator
//
//// Definicja makra ilości elementów w menu
//#define MENU_COUNT 4
//
//// Funkcja inicjalizująca menu
//void initialize_menu() {
//	menu[0] = (menu_t ) { &state_s13_settings_xmotor_state, "X Steps" };
//	menu[1] = (menu_t ) { &state_s14_settings_xscrew_state, "X Screw" };
//	menu[2] = (menu_t ) { &state_s15_settings_ymove_state, "Move X" };
//	menu[3] = (menu_t ) { &state_s16_settings_ymoving_state, "Moving X" };
//	menu[4] = (menu_t ) { &state_s12_settings_menu_state, " " }; // Zakładam, że " " jest miejscem na menu
//}
//
//static char line_1[16];
//static char line_2[16];
//
//static void update_view() {
//	sprintf(line_1, "%s <", menu[menu_iterator].name);
//	sprintf(line_2, "%s", menu[(menu_iterator + 1)].name);
//
//	app_print(line_1, line_2);
//}
//
//void state_s12_change() {
//	update_view();
//}
//
//state_node_t* state_s12_settings_menu(signal_t *signal) {
//	state_node_t *result = NULL;
//
//	if (signal == NULL) {
//		return result;
//	}
//
//	switch (signal->key_pressed) {
//	case '\n':
//	case '*':
//		// goto next state witch is menu state
//		result = menu[menu_iterator].state;
//		break;
//	case '8':
//		menu_iterator = (menu_iterator - 1);
//		if (menu_iterator <= 0)
//			menu_iterator = 0;
//		update_view();
//		break;
//	case '2':
//		menu_iterator = (menu_iterator + 1);
//		if (menu_iterator >= MENU_COUNT - 1)
//			menu_iterator = MENU_COUNT - 1;
//		update_view();
//		break;
//	case '/':
//		result = &state_s11_settings_page_state;
//		break;
//	default:
//		break;
//	}
//
//	return result;
//}
//
//const static state_node_t state_s12_settings_menu_state = { .state_change_func =
//		state_s12_change, .state_func = state_s12_settings_menu };
//
