/*
 * machine.h
 *
 *  Created on: Jul 7, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef MACHINE_H_
#define MACHINE_H_

#include "stdbool.h"
#include <stdlib.h>
#include <string.h>


typedef enum { G1, G92 } g_code_type_t;

typedef struct {
    g_code_type_t type;
    double X;
    double A;
    double F;
} g_code_t;


typedef long (*get_us_fun_t)();
void appV2_error(const char * message);


typedef struct {
	float offset_plus;
    float offset_minus;
} machine_offsets_t;

// interface for SD/UART/generator
typedef void (*init_t)(void *);
typedef bool (*is_program_finished_t)();
typedef bool (*is_gcode_available_t)();
typedef g_code_t * (*get_next_gcode_t)();


typedef struct {
	init_t init;
	is_program_finished_t is_program_finished;
	is_gcode_available_t is_gcode_available;
	get_next_gcode_t get_next_gcode;
} gcode_provider_t;

#endif /* MACHINE_H_ */
