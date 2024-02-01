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


static char line_1[16];
static char line_2[16];



typedef enum {
  STOP,
  BEGGIN,
  ROTATE_COIL,
  MOVE_WIRE,
  CHANGE_DIR
} wind_process_t;

static wind_process_t machine_state = STOP;
static int turns;
static float distance;
static volatile bool one_rotation_done;
static bool dir;
static int speed_table[10] = {
	750, 500, 400, 360, 230, 130, 70, 50, 30, 20
};
static long long machineTime;
/*
 * fuction declaration
 */
void machine_move();

static void update_view() {
	sprintf(line_1, "%-3s F%2d %s", machine_params.cw == true ? "CW" : "CCW", machine_params.speed, machine_params.manual == true ? "MANUAL" : "AUTO");
	sprintf(line_2, "%-5dT   %-5dT", turns,
			machine_params.coil_turns);

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


	if(signal == NULL) {
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
	default:
		break;
	}


	return result;
}









void machine_init() {
	machineTime = currentTimeUs();
	dir = true;
	turns = 0;
	one_rotation_done = false;
}

void rotate();
void move_wire();
void change_dir();

void machine_move() {
	switch(machine_state) {
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
		move_wire();
		break;
	case CHANGE_DIR:
		change_dir();
		break;
	default:
		break;
	}
}


void rotate() {
	if(one_rotation_done) {
		one_rotation_done = false;
		machine_state = MOVE_WIRE;
		turns++;
		update_view();
		return;
	}

	if(machine_params.manual) {
		return;
	}

	long long currentTime = currentTimeUs();
	long long elapsedTime = currentTime - machineTime;

	// Move the motor if enough time has elapsed
	if (elapsedTime >= speed_table[machine_params.speed- 1 ]) { // speded can be form 1 to 10

		x_stepper_step(machine_params.cw);
		// Update the time of the last step
		machineTime = currentTimeUs();
	}
}

static float last_distance = 0;

void move_wire() {
	if(machine_params.distance <= distance) {
		machine_state = CHANGE_DIR;
		distance = 0;
		last_distance = 0;
		return;
	}

	if(machine_params.wire_size <= distance - last_distance) {
		last_distance = distance;
		machine_state = ROTATE_COIL;
		return;
	}

	long long currentTime = currentTimeUs();
	long long elapsedTime = currentTime - machineTime;

	// Move the motor if enough time has elapsed
	if (elapsedTime >= speed_table[5]) {

		y_stepper_step(dir);
		// Update the time of the last step

		distance += 0.04; // 8mm screw per rotor -> 200 steps 8/200 -> one step 0.04mm
		machineTime = currentTime;
	}
}

void change_dir() {
	dir = !dir;
	machine_state = ROTATE_COIL;
}

/*
 * entrypoint from one rotation has been made.
 */
void app_machine_rotated_indicator() {
	one_rotation_done = true;
}

