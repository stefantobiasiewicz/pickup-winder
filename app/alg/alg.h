/*
 * alg.h
 *
 *  Created on: May 8, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef ALG_ALG_H_
#define ALG_ALG_H_

#include "../machine/machine.h"


typedef void (*init_alg_fun_t)(float coil_distance, float wire_thick, void* data);
typedef void (*alg_fun_t)(int turns, float x, float x_offset_minus_mm, float x_offset_plus_mm);

typedef struct {
	const char * name;
	init_alg_fun_t init_alg_fun;
	alg_fun_t alg_fun;
} wind_alg_t;


void init_basic_alg(float coil_distance, float wire_thick, void *data);
void basic_alg(int turns, float x, float x_offset_minus_mm, float x_offset_plus_mm);


#endif /* ALG_ALG_H_ */
