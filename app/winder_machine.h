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
	void (*state_change_func)(void);
} state_node_t;


typedef void (*print_fun_t)(char* _1st_line, char* _2nd_line);
typedef void (*step_fun_t)(bool dir);

/*
 * Application states declaration
 */
machine_state_t state_s1_main_page(signal_t * signal);
void state_s1_change();
machine_state_t state_s2_cw_ccw_decision(signal_t * signal);
void state_s2_change();
machine_state_t state_s3_coil_turns_decision(signal_t * signal);
void state_s3_change();
machine_state_t state_s4_wire_thick_decision(signal_t * signal);
void state_s4_change();
machine_state_t state_s41_distance_decision(signal_t * signal);
void state_s41_change();
machine_state_t state_s5_summary(signal_t * signal);
void state_s5_change();
machine_state_t state_s6_run(signal_t * signal);
void state_s6_change();

void x_stepper_step(bool dir);
void y_stepper_step(bool dir);


typedef struct {
	bool cw;
	int coil_turns;
	float wire_size;
	float distance;
	int speed;
	bool manual;
} machine_params_t;

extern machine_params_t machine_params;


void app_init(print_fun_t print_fun, step_fun_t x_step, step_fun_t y_step);
void app_put_signal(signal_t signal);
void app_state_machine_loop();


void app_print(char* _1st_line, char* _2nd_line);


void app_machine_rotated_indicator();


#endif /* WINDER_MACHINE_H_ */
