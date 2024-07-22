/*
 * winder_states.c
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#include "winder_machine.h"
#include "machine/machine.h"
#include "libs/FIFO.h"

machine_params_t machine_params;
machine_static_params_t machine_static_params;

const static state_node_t *current_node_state;
static fifo_t fifo;
static print_fun_t print;
static step_fun_t x_step_fun;
static step_fun_t y_step_fun;
static get_us_fun_t get_us_fun;
static motor_enable_fun_t motor_enable_fun;
static write_flash_fun_t write_flash_fun;
static read_flash_fun_t read_flash_fun;
static set_motor_pwm_t set_motor_pwm_fun;
static set_motor_relay_t set_motor_relay;


extern state_node_t state_s05_init_flash_state;

void app_init(machine_control_t machine_controll_callbacks)
{
	state_s05_init_flash_state_init();
	state_s1_init();
	//settings
	state_s11_init();
	state_rotor_test_init();
	state_esc_config_init();

	state_s2_init();
	state_s3_init();
	state_s4_init();
	state_s41_init();
	state_s5_init();
	state_s6_init();
	state_s61_init();
	state_s7_init();
	state_select_alg_inti();

    fifo = fifo_create(10, sizeof(signal_t));

    print = machine_controll_callbacks.print_fun;
    x_step_fun = machine_controll_callbacks.step_fun_x;
    y_step_fun = machine_controll_callbacks.step_fun_y;
    get_us_fun = machine_controll_callbacks.get_us_fun;
    motor_enable_fun = machine_controll_callbacks.motor_enable_fun;
    write_flash_fun = machine_controll_callbacks.write_flash_fun;
    read_flash_fun = machine_controll_callbacks.read_flash_fun;
	set_motor_pwm_fun = machine_controll_callbacks.set_motor_pwm;
	set_motor_relay = machine_controll_callbacks.set_motor_relay;

	machine_static_params.motor_type = PWM;

	current_node_state = &state_s05_init_flash_state;

//	initialize_menu();
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

    machine_main_loop();
}

/**
 * functions for making step of stepper motor (X - moving wire, Y - Rotor motor in case of using stepper motor)
*/
void x_stepper_step(bool dir)
{
    x_step_fun(dir);
}
void y_stepper_step(bool dir)
{
    y_step_fun(dir);
}
void motor_enable(bool en)
{
    motor_enable_fun(en);
}

/**
 * Adjust the speed of a motor based on a normalized input range.
 *
 * This function maps a user-provided speed from a scale of 0 to 100 to a
 * PWM-compatible scale of 5 to 200 and then sets the motor PWM accordingly.
 *
 * @param speed An integer representing the desired motor speed as a percentage
 *              from 0 (minimum) to 100 (maximum).
 */
void motor_speed(int speed) {
    // Validate and clamp the input range to avoid out-of-range values
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;

    // Map the speed from a range of 0-100 to 5-200
    int pwm_value = (speed * (200 - 5) / 100) + 5;

    // Set the motor PWM to the calculated value
    set_motor_pwm_fun(pwm_value);
}

void motor_relay(bool dir) {
	set_motor_relay(dir);
}

long currentTimeUs()
{
    return get_us_fun();
}

void read_flash(machine_static_params_t* machine_static_params)
{
    read_flash_fun(machine_static_params);
}

void write_flash(machine_static_params_t* machine_static_params)
{
    write_flash_fun(machine_static_params);
}
