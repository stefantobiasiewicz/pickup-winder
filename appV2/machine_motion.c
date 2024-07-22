/*
 * machine_motion.c
 *
 *  Created on: Jul 7, 2024
 *      Author: stefantobiasiewicz
 */

#include "machine_motion.h"

static stepper_fun_t step_x;
static stepper_fun_t step_a;
static get_us_fun_t get_us_fun;
static g_code_done_t g_code_done;
static current_g_code_info_t current_g_code_info;

void machine_motion_init(machine_motion_callbacks_t callbacks) {
	if(callbacks.step_x == NULL) {
		appV2_error("cal...s.step_x");
	}
	step_x = callbacks.step_x;

	if(callbacks.step_a == NULL) {
		appV2_error("cal...s.step_a");
	}
	step_a = callbacks.step_a;

	if(callbacks.get_us_fun == NULL) {
		appV2_error("cal...s.get_us_fun");
	}
	get_us_fun = callbacks.get_us_fun;

	if(callbacks.g_code_done == NULL) {
		appV2_error("cal...s.g_code_done");
	}
	g_code_done = callbacks.g_code_done;

	\
	if(callbacks.current_g_code_info == NULL) {
		appV2_error("cal...s.current_g_code_info");
	}
	current_g_code_info = callbacks.current_g_code_info;
}


static int speed_time = 100;

static const int axis_a_steps = 200 * 8;
static const int axis_x_steps = 200;

static void move_x(int dir) {
	if (dir < 0) {
		step_x(false);
	} else {
		step_x(true);
	}
}

static void move_y(int dir) {
	if (dir < 0) {
		step_a(true);
	} else {
		step_a(false);
	}
}


static int dx;
static int dy;
static int sx;
static int sy;
static int err;

static int x1;
static int y1;
static int x2;
static int y2;

static int work = 0;

static void bresenham_line(int _x1, int _y1, int _x2, int _y2) {

	x1 = _x1;
	x2 = _x2;

	y1 = _y1;
	y2 = _y2;



    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    sx = x1 < x2 ? 1 : -1;
    sy = y1 < y2 ? 1 : -1;
    err = dx - dy;

    work = 1;
}


long long machineTime;

void machine_motion_loop() {
    if (work == 0) {
        return;
    }


	long long currentTime = get_us_fun();
	long long elapsedTime = currentTime - machineTime;

	if (elapsedTime >= speed_time) { // speded can be form 1 to 10

	    if (x1 == x2 && y1 == y2) {
	        work = 0;
	        if (g_code_done != NULL) {
	        	g_code_done();
	        }
	        return;
	    }

	    int e2 = 2 * err;
	    if (e2 > -dy) {
	        err -= dy;
	        x1 += sx;
	        move_x(sx);  // Przesuń w osi X o jeden krok w kierunku sx
	    }
	    if (e2 < dx) {
	        err += dx;
	        y1 += sy;
	        move_y(sy);  // Przesuń w osi Y o jeden krok w kierunku sy
	    }

		machineTime = get_us_fun();
	}
}


int machine_motion_is_machine_working() {
    return work;
}

void machine_motion_set_speed_time(int speed) {
	speed_time = speed;
}

static int maiche_current_a_steps = 0;
static int maiche_current_x_steps = 0;

void machine_motion_move_g_code(g_code_t* g_code) {
	current_g_code_info(g_code);
    switch (g_code->type) {
        case G1:
            const int maiche_next_x_steps = g_code->X * axis_x_steps;
            const int maiche_next_a_steps = g_code->A * axis_a_steps;

            bresenham_line(maiche_current_x_steps, maiche_current_a_steps, maiche_next_x_steps, maiche_next_a_steps);

            maiche_current_x_steps = maiche_next_x_steps;
            maiche_current_a_steps = maiche_next_a_steps;
            break;
        case G92:
            maiche_current_a_steps = 0;
            maiche_current_x_steps = 0;
            break;
    }
}

void machine_set_work(bool set_work) {
	work = set_work;
}

