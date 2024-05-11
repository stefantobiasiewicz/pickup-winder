/*
 * machine.h
 *
 *  Created on: May 9, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef MACHINE_MACHINE_H_
#define MACHINE_MACHINE_H_


#include "../winder_machine.h"


void machine_reset();
void machine_main_loop();
int machine_get_turns();
void machine_go_to_x(float x);
void machine_stop();


void machine_rotated_indicator();


#endif /* MACHINE_MACHINE_H_ */
