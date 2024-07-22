/*
 * winder_states.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "winder_machine.h"
#include "libs/FIFO.h"

#include "states/state_start.h"


const static state_node_t *current_node_state;
static fifo_t fifo;
static print_fun_t print;


void app_init(machine_control_t machine_controll_callbacks)
{
	init_states();

    fifo = fifo_create(10, sizeof(signal_t));

    print = machine_controll_callbacks.print_fun;

    current_node_state = &state_start_state;
    state_start_state.state_change_func();
}

void app_print(char* _1st_line, char* _2nd_line)
{
    print(_1st_line, _2nd_line);
}

void app_put_signal(signal_t signal)
{
    fifo_add(fifo, &signal);
}

void app_state_machine_loop()
{
    if (current_node_state == NULL) {
        return;
    }

    const state_node_t* next;
    if (!fifo_is_empty(fifo)) {
        signal_t current_signal;
        fifo_get(fifo, &current_signal);

        next = current_node_state->state_func(&current_signal);
        if (next == NULL) {
            return;
        }
    } else {
        next = current_node_state->state_func(NULL);
        if (next == NULL) {
            return;
        }
    }

    if (next->state_change_func != NULL) {
    	next->state_change_func();
    }

    current_node_state = next;
}



static char fetch_parameter_i_buff[10] = { 0 };
static char fetch_parameter_f_buff[10] = { 0 };

static char line_1[24];
static char line_2[24];

/**
 * internal module funtion for printing parameter featch screen
 * @param name: name of feaching parameter
 * @param param_buffer: current state of input buffer
 */
static void fetch_parameter_print(char * name, char* param_buffer) {
	sprintf(line_1, "%s", name);
	sprintf(line_2, ">%s", param_buffer);


	app_print(line_1, line_2);
}

/**
 * @param signal: key form numpad
 * @param name: name to display on LCD
 * @param param:  pointer to value
 * @return: -1 if go back, 0 if nothing hapen, 1 if parameter featched properly
 */

int fetch_parameter_i(signal_t *signal, char *name, int *param) {
	static void *param_tracker;
	if (param_tracker != param) {	// checking if feaching other parameter, if yes print new name and set buffer
		memset(fetch_parameter_i_buff, 0, 10);

		if (*param != 0){
			sprintf(fetch_parameter_i_buff, "%d", *param);
		}

		fetch_parameter_print(name, fetch_parameter_i_buff);
		param_tracker = param;
	}

	if (signal == NULL) {
		return 0;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		*param = atoi(fetch_parameter_i_buff);
		param_tracker = NULL;
		return 1;
	case '1':
		strcat(fetch_parameter_i_buff, "1");
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '2':
		strcat(fetch_parameter_i_buff, "2");
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '3':
		strcat(fetch_parameter_i_buff, "3");
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '4':
		strcat(fetch_parameter_i_buff, "4");
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '5':
		strcat(fetch_parameter_i_buff, "5");
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '6':
		strcat(fetch_parameter_i_buff, "6");
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '7':
		strcat(fetch_parameter_i_buff, "7");
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '8':
		strcat(fetch_parameter_i_buff, "8");
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '9':
		strcat(fetch_parameter_i_buff, "9");
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '0':
		strcat(fetch_parameter_i_buff, "0");
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '\r':
		fetch_parameter_i_buff[strlen(fetch_parameter_i_buff) - 1] = '\0';
		fetch_parameter_print(name, fetch_parameter_i_buff);
		break;
	case '/':
		param_tracker = NULL;
		return -1;
	default:
		break;
	}

	return 0;
}


/**
 * @param signal: key form numpad
 * @param name: name to display on LCD
 * @param param:  pointer to value
 * @return: -1 if go back, 0 if nothing hapen, 1 if parameter featched properly
 */

int fetch_parameter_f(signal_t *signal, char *name, float *param) {
	static void *param_tracker;
	if (param_tracker != param) {	// checking if feaching other parameter, if yes print new name and set buffer
		memset(fetch_parameter_f_buff, 0, 10);

		if (*param != 0){
			sprintf(fetch_parameter_f_buff, "%0.2f", *param);
		}

		fetch_parameter_print(name, fetch_parameter_f_buff);
		param_tracker = param;
	}

	if (signal == NULL) {
		return 0;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		*param = atof(fetch_parameter_f_buff);
		param_tracker = NULL;
		return 1;
		break;
	case '1':
		strcat(fetch_parameter_f_buff, "1");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '2':
		strcat(fetch_parameter_f_buff, "2");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '3':
		strcat(fetch_parameter_f_buff, "3");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '4':
		strcat(fetch_parameter_f_buff, "4");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '5':
		strcat(fetch_parameter_f_buff, "5");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '6':
		strcat(fetch_parameter_f_buff, "6");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '7':
		strcat(fetch_parameter_f_buff, "7");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '8':
		strcat(fetch_parameter_f_buff, "8");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '9':
		strcat(fetch_parameter_f_buff, "9");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '0':
		strcat(fetch_parameter_f_buff, "0");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '.':
		strcat(fetch_parameter_f_buff, ".");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '-':
		strcat(fetch_parameter_f_buff, "-");
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '\r':
		fetch_parameter_f_buff[strlen(fetch_parameter_f_buff) - 1] = '\0';
		fetch_parameter_print(name, fetch_parameter_f_buff);
		break;
	case '/':
		param_tracker = NULL;
		return -1;
		break;
	default:
		break;
	}


	return 0;
}

