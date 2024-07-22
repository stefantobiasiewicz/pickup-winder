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
//
//static char number_buff[10] = { 0 };
//
//static void update_view() {
//	sprintf(line_1, "Y move:");
//	sprintf(line_2, ">%-11ssteps", number_buff);
//
//	app_print(line_1, line_2);
//}
//
//void state_s15_change() {
//	update_view();
//}
//
//state_node_t* state_s15_settings_ymove(signal_t *signal) {
//	state_node_t* result = NULL;
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
//		int y_steps = atoi(number_buff);
//
//		motor_enable(true);
//		long time = currentTimeUs();
//		while (!(y_steps <= 0)) {
//			long long currentTime = currentTimeUs();
//			long long elapsedTime = currentTime - time;
//
//			if (elapsedTime >= 100) {
//				y_stepper_step(true);
//				y_steps--;
//				time = currentTime;
//			}
//		}
//		motor_enable(false);
//
//		break;
//	case '1':
//		strcat(number_buff, "1");
//		update_view();
//		break;
//	case '2':
//		strcat(number_buff, "2");
//		update_view();
//		break;
//	case '3':
//		strcat(number_buff, "3");
//		update_view();
//		break;
//	case '4':
//		strcat(number_buff, "4");
//		update_view();
//		break;
//	case '5':
//		strcat(number_buff, "5");
//		update_view();
//		break;
//	case '6':
//		strcat(number_buff, "6");
//		update_view();
//		break;
//	case '7':
//		strcat(number_buff, "7");
//		update_view();
//		break;
//	case '8':
//		strcat(number_buff, "8");
//		update_view();
//		break;
//	case '9':
//		strcat(number_buff, "9");
//		update_view();
//		break;
//	case '0':
//		strcat(number_buff, "0");
//		update_view();
//		break;
//	case '\r':
//		number_buff[strlen(number_buff) - 1] = '\0';
//		update_view();
//		break;
//	case '/':
//		result = &state_s12_settings_menu_state;
//		break;
//	default:
//		break;
//	}
//
//	return result;
//}
//
//
//const static state_node_t state_s15_settings_ymove_state = {
//	.state_change_func = state_s15_change,
//	.state_func = state_s15_settings_ymove,
//};
