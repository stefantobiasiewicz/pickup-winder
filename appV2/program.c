/*
 * program.c
 *
 *  Created on: Jul 8, 2024
 *      Author: stefantobiasiewicz
 */
#include "program.h"
#include <math.h>

static gcode_provider_t *gcode_provider = NULL;
machine_offsets_t offsets;

static program_start_callback_t program_start_callback;
static program_finished_callback_t program_finished_callback;
static program_send_next_gcode_to_machine_t program_send_next_gcode_to_machine;

void program_init(program_callbacks_t callbacks) {
	if (callbacks.program_start_callback == NULL) {
		appV2_error("cal..s.program_start_callback");
	}
	program_start_callback = callbacks.program_start_callback;

	if (callbacks.program_finished_callback == NULL) {
		appV2_error("cal..s.program_finished_callback");
	}
	program_finished_callback = callbacks.program_finished_callback;

	if (callbacks.program_send_next_gcode_to_machine == NULL) {
		appV2_error("cal..s.program_send_next_gcode_to_machine");
	}
	program_send_next_gcode_to_machine =
			callbacks.program_send_next_gcode_to_machine;
}

static g_code_t *buffer_code;

g_code_t* program_get_next_gcode() {
	g_code_t *result = buffer_code;
	buffer_code = NULL;

	return result;
}

static bool machine_wait_for_code = false;
void program_machine_wait_for_gcode() {
	machine_wait_for_code = true;
}

void program_loop() {
	if (gcode_provider == NULL) {
		return;
	}

	if (buffer_code == NULL) {
		//featch next cod
		if (gcode_provider->is_program_finished()) {
			gcode_provider = NULL;

			g_code_t base = { .type = G92 };
			program_send_next_gcode_to_machine(&base);

			program_finished_callback();

			return;
		}

		if (gcode_provider->is_gcode_available()) {
			buffer_code = gcode_provider->get_next_gcode();

			if (buffer_code == NULL) {
				return;
			}

			if (machine_wait_for_code) {
				//send code to machine
				if (program_send_next_gcode_to_machine == NULL) {
					appV2_error("program_send_next_gcode_to_machine");
				}
				program_send_next_gcode_to_machine(buffer_code);
			}
		}
	}
}

void program_start(gcode_provider_t *provider) {
	gcode_provider = provider;
	program_machine_wait_for_gcode();
	program_start_callback();
}

static char parser_buffer[256];

g_code_t program_parse_g_code(const char *line) {
	g_code_t g_code = { 0 };

	strncpy(parser_buffer, line, sizeof(parser_buffer));
	parser_buffer[sizeof(parser_buffer) - 1] = '\0';

	if (strncmp(parser_buffer, "G1", 2) == 0) {
		g_code.type = G1;
		char *token = strtok(parser_buffer + 2, " ");
		while (token != NULL) {
			if (token[0] == 'X') {
				g_code.X = atof(token + 1);
			} else if (token[0] == 'A') {
				g_code.A = atof(token + 1);
			} else if (token[0] == 'F') {
				g_code.F = atof(token + 1);
			}
			token = strtok(NULL, " ");
		}
	} else if (strncmp(parser_buffer, "G92", 3) == 0) {
		g_code.type = G92;
	}

	return g_code;
}

/*
 * common variables
 */
static float current_turn = 0;
static float current_distance = 0;


// Basic g_code provider algorithm
static float basic_distance = 0;
static int basic_turns = 0;
static float basic_distance_step = 0;
static bool basic_cw = false;

void basic_init(void *arg) {
	basic_alg_args *casted_args = (basic_alg_args*) arg;

	if (casted_args == NULL) {
		appV2_error("basic_alg_args null");
	}

	basic_distance = casted_args->distance;
	basic_turns = casted_args->turns;
	basic_distance_step = casted_args->distance_step;
	basic_cw = casted_args->cw;

	current_turn = 0;
	current_distance = 0;

}

bool basic_is_program_finished() {
	return current_turn >= basic_turns;
}

bool basic_is_gcode_available() {
	return current_turn < basic_turns;
}


static g_code_t basic_g_code;

g_code_t * basic_get_next_gcode(machine_offsets_t *machie_offset) {
	if (current_turn < basic_turns) {

		float distance = 0;

		if (current_distance < (basic_distance / 2)) {
			distance = basic_distance + offsets.offset_plus;
		} else {
			distance = 0 - offsets.offset_minus;
		}

		int turns = current_turn + round(basic_distance / basic_distance_step);

		if (turns > basic_turns) {
			turns = turns - (turns - basic_turns);
		}

		basic_g_code.type = G1;
		basic_g_code.X = distance;
		basic_g_code.A = turns;

		if (basic_cw == true) {
			basic_g_code.A = -turns;
		}


		current_distance = distance;
		current_turn = turns;

		return &basic_g_code;
	}
	return NULL;
}

gcode_provider_t basic_alg_provider = { .init = basic_init, .get_next_gcode =
		basic_get_next_gcode, .is_gcode_available = basic_is_gcode_available,
		.is_program_finished = basic_is_program_finished, };

// Cross g_code provider algorithm

#define CROSS_DISTANCE_DIVIDER 5
#define CROSS_DISTANCE_STEP 0.1

#define CROSS_SECTION_PRCENT 50

static int cross_turns_till_cross;

static float cross_distance = 0;
static int cross_turns = 0;

static int cross_alg_step = 0;
static bool cross_cw = false;


void cross_init(void *arg) {
	cross_alg_args *casted_args = (cross_alg_args*) arg;

	if (casted_args == NULL) {
		appV2_error("basic_alg_args null");
	}

	cross_distance = casted_args->distance;
	cross_turns = casted_args->turns;

	cross_cw = casted_args->cw;

	current_turn = 0;
	current_distance = 0;
	cross_alg_step = 0;

	cross_turns_till_cross = cross_turns * CROSS_SECTION_PRCENT / 100;

}

bool cross_is_program_finished() {
	return current_turn >= cross_turns;
}

bool cross_is_gcode_available() {
	return current_turn < cross_turns;
}

static g_code_t cross_g_code;
g_code_t * cross_get_next_gcode(machine_offsets_t *machie_offset) {
	if (current_turn < cross_turns) {

		if (current_turn < cross_turns_till_cross) {
			// tutaj nawijanie jest krossowe

			switch (cross_alg_step) {
			case 0: {
				float distance = cross_distance + offsets.offset_plus;
				float turns = current_turn + CROSS_DISTANCE_DIVIDER;

				cross_g_code.type = G1;
				cross_g_code.X = distance;
				cross_g_code.A = turns;

				if (cross_cw == true) {
					cross_g_code.A = -turns;
				}

				current_distance = distance;
				current_turn = turns;
				cross_alg_step++;
			}
				break;
			case 1: {
				current_turn += 0.5;


				cross_g_code.type = G1;
				cross_g_code.X = current_distance;
				cross_g_code.A = current_turn;

				if (cross_cw == true) {
					cross_g_code.A = -current_turn;
				}

				cross_alg_step++;
			}
				break;
			case 2: {
				float distance = 0 - offsets.offset_minus;
				float turns = current_turn + CROSS_DISTANCE_DIVIDER;

				cross_g_code.type = G1;
				cross_g_code.X = distance;
				cross_g_code.A = turns;

				if (cross_cw == true) {
					cross_g_code.A = -turns;
				}


				current_distance = distance;
				current_turn = turns;
				cross_alg_step++;
			}
				break;
			case 3: {
				current_turn += 0.5;

				cross_g_code.type = G1;
				cross_g_code.X = current_distance;
				cross_g_code.A = current_turn;

				if (cross_cw == true) {
					cross_g_code.A = -current_turn;
				}


				cross_alg_step = 0;
			}
				break;
			default:
				break;
			}

		} else {
			// tutaj nawijanie normalne z stepem
			float distance = 0;

			if (current_distance < (cross_distance / 2)) {
				distance = cross_distance + offsets.offset_plus;
			} else {
				distance = 0 - offsets.offset_minus;
			}

			int turns = current_turn + round(cross_distance / CROSS_DISTANCE_STEP);

			if (turns > cross_turns) {
				turns = turns - (turns - cross_turns);
			}

			cross_g_code.type = G1;
			cross_g_code.X = distance;
			cross_g_code.A = turns;

			if (cross_cw == true) {
				cross_g_code.A = -turns;
			}


			current_distance = distance;
			current_turn = turns;
		}

		return &cross_g_code;
	}
	return NULL;
}

gcode_provider_t cross_alg_provider = { .init = cross_init, .get_next_gcode =
		cross_get_next_gcode, .is_gcode_available = cross_is_gcode_available,
		.is_program_finished = cross_is_program_finished, };
