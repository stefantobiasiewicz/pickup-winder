/*
 * state_s1.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "winder_machine.h"
/*
 * State variables;
 */

static int actual_turns = 0;

static char line_1[24];
static char line_2[24];

typedef enum {
	STOP, BEGGIN, ROTATE_COIL, MOVE_WIRE, CHANGE_DIR
} wind_process_t;

static wind_process_t machine_state = STOP;
static int turns;
static volatile bool one_rotation_done;
static bool dir;
static int speed_table[10] = { 230, 130, 70, 30, 20, 18, 16, 12, 11, 10 };
static long long machineTime_x;
static long long machineTime_y;

int distance_steps;
int distance_wire_steps;

int machine_x_distance_steps;
int machine_x_last_distance_steps;
/*
 * fuction declaration
 */
static void machine_move();
static void machine_stop();

static void update_view() {
	sprintf(line_1, "%-5s%2dF%8s", machine_params.cw == true ? "CW" : "CCW",
			machine_params.speed,
			machine_params.manual == true ? "MANUAL" : "AUTO");
	sprintf(line_2, "%-8d%8d", turns, machine_params.coil_turns);

	app_print(line_1, line_2);
}

void state_s6_change() {
	machine_params.speed = 1;
	machine_params.manual = true;

	actual_turns = 0;
	machine_state = BEGGIN;

	update_view();
}

machine_state_t state_s6_run(signal_t *signal) {
	machine_state_t result = NO_CHANGE;

	machine_move();

	if (machine_params.coil_turns <= turns) {
		machine_stop();
		return STATE_S7;
	}

	if (signal == NULL) {
		return result;
	}

	switch (signal->key_pressed) {
	case '\n':
		machine_params.manual = !machine_params.manual;
		motor_enable(!machine_params.manual);
		update_view();
		break;
	case '2':
		if (machine_params.speed > 1) {
			machine_params.speed--;
		}
		update_view();
		break;
	case '8':
		if (machine_params.speed < 10) {
			machine_params.speed++;
		}
		update_view();
		break;
	case '/':
		result = STATE_S61;
		break;
	default:
		break;
	}

	return result;
}

/*
 * Winding procces state machine
 */

static void machine_init() {
	machineTime_x = currentTimeUs();
	machineTime_y = currentTimeUs();
	dir = true;
	turns = 0;
	one_rotation_done = false;


	float mm_per_step = machine_static_params.x_screw / machine_static_params.x_motor_steps;

	distance_steps = machine_params.distance / mm_per_step;
	distance_wire_steps = machine_params.wire_size / mm_per_step;

	machine_x_distance_steps = 0;
	machine_x_last_distance_steps = 0;
}

/*
 * declarations
 */
static void rotate();
static void move_wire();
static void change_dir();

static void machine_move() {
	switch (machine_state) {
	case STOP:
		break;
	case BEGGIN:
		machine_init();
		machine_state = ROTATE_COIL;
		break;
	case ROTATE_COIL:
		rotate();
		break;
	case MOVE_WIRE:
		rotate();
		move_wire();
		break;
	case CHANGE_DIR:
		change_dir();
		break;
	default:
		break;
	}
}

static void rotate() {
	if (one_rotation_done) {
		one_rotation_done = false;
		machine_state = MOVE_WIRE;
		turns++;
		update_view();
		return;
	}

	if (machine_params.manual) {
		return;
	}

	long long currentTime = currentTimeUs();
	long long elapsedTime = currentTime - machineTime_x;

	// Move the motor if enough time has elapsed
	if (elapsedTime >= speed_table[machine_params.speed - 1]) { // speded can be form 1 to 10

		x_stepper_step(machine_params.cw);
		// Update the time of the last step
		machineTime_x = currentTimeUs();
	}
}


static void move_wire() {
	if (machine_x_distance_steps >= distance_steps) {
		machine_state = CHANGE_DIR;
		machine_x_distance_steps = 0;
		machine_x_last_distance_steps = 0;
		return;
	}

	int delta_distance = machine_x_distance_steps - machine_x_last_distance_steps;
	if (distance_wire_steps <= delta_distance) {
		machine_x_last_distance_steps = machine_x_distance_steps;
		machine_state = ROTATE_COIL;
		if (machine_params.manual) {
			motor_enable(false);
		}
		return;
	}

	if (machine_params.manual) {
		motor_enable(true);
	}

	long long currentTime = currentTimeUs();
	long long elapsedTime = currentTime - machineTime_y;

	// Move the motor if enough time has elapsed
	if (elapsedTime >= speed_table[3]) {

		y_stepper_step(dir);
		machine_x_distance_steps ++;

		// Update the time of the last step
		machineTime_y = currentTime;
	}
}

static void change_dir() {
	dir = !dir;
	machine_state = ROTATE_COIL;
}

static void machine_stop() {
	machine_state = STOP;
	//todo idk if it is nessesary
	machine_x_distance_steps = 0;
	machine_x_last_distance_steps = 0;
	motor_enable(false);
}

/*
 * entrypoint from one rotation has been made.
 */
void app_machine_rotated_indicator() {
	one_rotation_done = true;
}

