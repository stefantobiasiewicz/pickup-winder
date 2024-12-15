/*
 * cross.h
 *
 *  Created on: Aug 27, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef PROGRAMS_CROSS_H_
#define PROGRAMS_CROSS_H_

#include "../machine.h"

typedef struct {
	float distance;
	int turns;
	int cross_amount;
	int cross_density;
	bool cw;
} cross_alg_args;

extern gcode_provider_t cross_alg_provider;

#endif /* PROGRAMS_CROSS_H_ */
