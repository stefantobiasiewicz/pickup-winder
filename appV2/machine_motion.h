/*
 * machine_motion.h
 *
 *  Created on: Jul 7, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef MACHINE_MOTION_H_
#define MACHINE_MOTION_H_

#include "machine.h"

typedef void (*stepper_fun_t)(bool dir);
typedef void (*g_code_done_t)();
typedef void (*current_g_code_info_t)(g_code_t* current);

typedef struct {
	stepper_fun_t step_x;
	stepper_fun_t step_a;
	get_us_fun_t get_us_fun;
	g_code_done_t g_code_done;
	current_g_code_info_t current_g_code_info;
} machine_motion_callbacks_t;

void machine_motion_init(machine_motion_callbacks_t callbacks);

void machine_motion_loop();
int machine_motion_is_machine_working();
void machine_motion_set_speed_time(int speed);
void machine_motion_move_g_code(g_code_t* g_code);

void machine_set_work(bool set_work);


#endif /* MACHINE_MOTION_H_ */
