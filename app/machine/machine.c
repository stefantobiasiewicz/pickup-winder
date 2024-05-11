#include "machine.h"


/*
 * ************************************
 * Winding procces state machine
 * ************************************
 */

void check_if_rotation_done();
static void move_x(bool dir_to_move);

typedef enum {
    STOP,
    BEGGIN,
	WORK
} wind_process_t;

static wind_process_t machine_state = STOP;

/* X stepper motor time*/
static long long machineTime_x;
/* X distance form start point*/
static float x_distance_mm;

static int turns;


float mm_per_step;

static int speed_table[10] = { 230, 130, 70, 30, 20, 18, 16, 12, 11, 10 };

// int total_distance_steps;
// int distance_wire_steps;

// int machine_x_distance_steps;
// int machine_x_last_distance_steps;

static void machine_init()
{
    machineTime_x = currentTimeUs();

    mm_per_step = machine_static_params.x_screw / machine_static_params.x_motor_steps;

    /*Set machine X zero*/
    x_distance_mm = 0;
    /*starting motor*/
    set_motor_speed(20);
    motor_enable(true);
	machine_state = WORK;
}

void machine_reset() {
	turns = 0;
}


void machine_main_loop()
{
    // move motors if needed
    make_machine_move();
}


void move_x(bool dir_to_move)
{
    long long currentTime = currentTimeUs();
    long long elapsedTime = currentTime - machineTime_x;

    // Move the motor if enough time has elapsed
    if (elapsedTime >= speed_table[3]) {
        x_stepper_step(dir_to_move);
        x_distance_mm += mm_per_step;

        // Update the time of the last step
        machineTime_x = currentTime;
    }
}


static float distace_to_move;
/**
 * non blocking funvtion for running commands
*/
void make_machine_move() {
    float delta = distace_to_move - x_distance_mm;
    if (delta > 0) {
        move_x(true);
    } else if (delta < 0) {
        move_x(false);
    }
}



void machine_go_to_x(float x) {
    distace_to_move = x;
}

int machine_get_turns() {
	return turns;
}


void machine_stop() {
	machine_state = STOP;
	set_motor_speed(0);
	motor_enable(false);
}

void machine_rotated_indicator()
{
	turns++;
}
