/*
 * program.h
 *
 *  Created on: Jul 8, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "machine.h"



typedef void (*program_start_callback_t)();
typedef void (*program_finished_callback_t)();
typedef void (*program_send_next_gcode_to_machine_t)(g_code_t * code);
typedef struct {
	program_start_callback_t program_start_callback;
	program_finished_callback_t program_finished_callback;
	program_send_next_gcode_to_machine_t program_send_next_gcode_to_machine;
} program_callbacks_t;

extern machine_offsets_t offsets;
void program_init(program_callbacks_t callbacks);

g_code_t * program_get_next_gcode();
void program_loop();
g_code_t program_parse_g_code(const char* line);
void program_machine_wait_for_gcode();
void program_start(gcode_provider_t * provider);


typedef struct {
	float distance;
	int turns;
	float distance_step;
	bool cw;
} basic_alg_args;

extern gcode_provider_t basic_alg_provider;

typedef struct {
	float distance;
	int turns;
	bool cw;
} cross_alg_args;

extern gcode_provider_t cross_alg_provider;



#endif /* PROGRAM_H_ */
