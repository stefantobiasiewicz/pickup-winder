/*
 * winder_states.h
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef WINDER_MACHINE_H_
#define WINDER_MACHINE_H_

#include "usbh_hid_keybd.h"
#include <stdbool.h>


typedef enum {
  NO_CHANGE,
  STATE_S1,
  STATE_S2,
  STATE_S3,
  STATE_S4,
  STATE_S41,
  STATE_S5,
  STATE_S6
} machine_state_t;

typedef struct {
	char key_pressed;
} signal_t;

typedef struct {
	machine_state_t stateName;
	machine_state_t (*state_func)(signal_t * signal);
} state_node_t;


typedef void (*print_fun_t)(char* _1st_line, char* _2nd_line);

/*
 * Application states declaration
 */
machine_state_t state_s1_main_page(signal_t * signal);
machine_state_t state_s2_cw_ccw_decision(signal_t * signal);
machine_state_t state_s3_coil_turns_decision(signal_t * signal);
machine_state_t state_s4_wire_thick_decision(signal_t * signal);
machine_state_t state_s41_distance_decision(signal_t * signal);
machine_state_t state_s5_summary(signal_t * signal);
//machine_state_t state_s6_run(signal_t * signal);



typedef struct {
	bool cw;
	int coil_turns;
	float wire_size;
	float distance;
	int speed;
} machine_params_t;

extern machine_params_t machine_params;


void app_init(print_fun_t print_fun);
void app_put_signal(signal_t signal);
void app_state_machine_loop();


void app_print(char* _1st_line, char* _2nd_line);


#endif /* WINDER_MACHINE_H_ */
