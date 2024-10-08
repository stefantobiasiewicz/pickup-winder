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
//static char line_1[16];
//static char line_2[16];
//long time;
//
//int y_steps;
//bool dir = true;
//
//static void update_view() {
//	sprintf(line_1, "Y moving %d", machine_static_params.x_motor_steps);
//	sprintf(line_2, "steps" );
//
//	app_print(line_1, line_2);
//}
//
//void state_s16_change() {
//	update_view();
//
//	motor_enable(true);
//
//
//	y_steps = machine_static_params.x_motor_steps;
//	dir = true;
//	time = currentTimeUs();
//}
//
//state_node_t* state_s16_settings_ymoving(signal_t *signal) {
//	state_node_t* result = NULL;
//
//	long long currentTime = currentTimeUs();
//	long long elapsedTime = currentTime - time;
//
//	if (elapsedTime >= 100) {
//		y_stepper_step(dir);
//		y_steps--;
//		time = currentTime;
//
//		if(y_steps <= 0) {
//			dir = !dir;
//			y_steps = machine_static_params.x_motor_steps;
//		}
//	}
//
//
//	if (signal == NULL) {
//		return result;
//	}
//
//	switch (signal->key_pressed) {
//	case '\n':
//	case '*':
//		result = &state_s12_settings_menu_state;
//
//		motor_enable(false);
//		break;
//	case '/':
//		result = &state_s12_settings_menu_state;
//		motor_enable(false);
//		break;
//	default:
//		break;
//	}
//
//
//	return result;
//}
//
//
//const static state_node_t state_s16_settings_ymoving_state = {
//	.state_change_func = state_s16_change,
//	.state_func = state_s16_settings_ymoving,
//};
