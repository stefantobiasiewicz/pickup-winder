/*
 * state_select_alg.c
 *
 *  Created on: May 13, 2024
 *      Author: stefantobiasiewicz
 */


#include "../winder_machine.h"
#include "state_select_alg.h"
#include "state_s5.h"

#include "../alg/alg.h"
/*
 * State variables;
 */

static char line_1[36];
static char line_2[36];

extern wind_alg_t wind_algorithms[];
int alg_iterator = 0;

static void update_view() {
	sprintf(line_1, "%s <", wind_algorithms[alg_iterator].name);
	if(alg_count > 1 && alg_iterator < alg_count - 1) {
		sprintf(line_2, "%s", wind_algorithms[(alg_iterator + 1)].name);
	}

	app_print(line_1, line_2);
}

static void change() {



	update_view();
}

static const state_node_t* state_fun(signal_t *signal) {
	state_node_t* result = NULL;

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
	case '*':
		machine_params.algorithm = wind_algorithms[alg_iterator];

		result = &state_s5_summary_state;
		break;
	case '/':
		result = &state_s41_distance_decision_state;
		break;
	case '8':
			alg_iterator = (alg_iterator - 1);
			if (alg_iterator <= 0)
				alg_iterator = 0;
			update_view();
			break;
	case '2':
		alg_iterator = (alg_iterator + 1);
		if (alg_iterator >= alg_count - 1)
			alg_iterator = alg_count - 1;
		update_view();
		break;
	default:
		break;
	}

	return result;
}


state_node_t state_select_alg;

void state_select_alg_inti() {
	state_select_alg.state_change_func = change;
	state_select_alg.state_func = state_fun;
}


