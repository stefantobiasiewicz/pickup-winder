/*
 * winder_states.h
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef WINDER_MACHINE_H_
#define WINDER_MACHINE_H_

#include <stdbool.h>
#include <stdio.h>
#include "states_def.h"





/*
 * callbacks def for cotrioll machine
 */

typedef void (*print_fun_t)(char* _1st_line, char* _2nd_line);


typedef struct {
	print_fun_t print_fun;
}machine_control_t;


/*
 * Application states declaration
 */

void initialize_menu();


/*
 * callbacks for cotrioll machine
 */

void app_print(char* _1st_line, char* _2nd_line);


/*
 * API functions call form main
 */
void app_init(machine_control_t machine_controll_callbacks);
void app_put_signal(signal_t signal);
void app_state_machine_loop();

int fetch_parameter_f(signal_t *signal, char *name, float *param);
int fetch_parameter_i(signal_t *signal, char *name, int *param);

#endif /* WINDER_MACHINE_H_ */
