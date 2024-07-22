/*
 * states_def.h
 *
 *  Created on: May 12, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef STATES_STATES_DEF_H_
#define STATES_STATES_DEF_H_


typedef struct {
    char key_pressed;
} signal_t;

typedef struct state_node_t state_node_t;

struct state_node_t{
    const state_node_t* (*state_func)(signal_t* signal);
    void (*state_change_func)(void);
};


typedef struct menu_node_t menu_node_t;

struct menu_node_t{
	const char * name;
	state_node_t* state;
};

#endif /* STATES_STATES_DEF_H_ */
