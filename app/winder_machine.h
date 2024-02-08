/*
 * winder_states.h
 *
 *  Created on: Jan 30, 2024
 *      Author: stefantobiasiewicz
 */

#ifndef WINDER_MACHINE_H_
#define WINDER_MACHINE_H_

#include "usbh_hid_keybd.h"
#include <stdbool.h>

typedef enum {
	NO_CHANGE,
	STATE_S05,
	STATE_S1,
	STATE_S11,
	STATE_S12,
	STATE_S13,
	STATE_S14,
	STATE_S15,
	STATE_S16,
	STATE_S2,
	STATE_S3,
	STATE_S4,
	STATE_S41,
	STATE_S5,
	STATE_S6,
	STATE_S61,
	STATE_S7
} machine_state_t;

typedef struct {
	char key_pressed;
} signal_t;

typedef struct {
	machine_state_t stateName;
	machine_state_t (*state_func)(signal_t *signal);
	void (*state_change_func)(void);
} state_node_t;

typedef struct {
	bool cw;
	int coil_turns;
	float wire_size;
	float distance;
	int speed;
	bool manual;
} machine_params_t;

extern machine_params_t machine_params;

typedef enum {
	INIT = 0xaa, NO_INIT = 0xff
} flash_init_t;

typedef struct {
	flash_init_t init;
	int x_motor_steps;
	float x_screw;
} machine_static_params_t;

extern machine_static_params_t machine_static_params;

/*
 * callbacks def for cotrioll machine
 */
typedef void (*print_fun_t)(char *_1st_line, char *_2nd_line);
typedef void (*step_fun_t)(bool dir);
typedef void (*motor_enable_fun_t)(bool dir);
typedef long (*get_us_fun_t)();
typedef void (*write_flash_fun_t)(
		machine_static_params_t *machine_static_params);
typedef void (*read_flash_fun_t)(machine_static_params_t *machine_static_params);

/*
 * Application states declaration
 */
machine_state_t state_s05_init_flash(signal_t *signal);
void state_s05_change();
machine_state_t state_s1_main_page(signal_t *signal);
void state_s1_change();
machine_state_t state_s11_settings_page(signal_t *signal);
void state_s11_change();
machine_state_t state_s12_settings_menu(signal_t *signal);
void state_s12_change();
machine_state_t state_s13_settings_xmotor(signal_t *signal);
void state_s13_change();
machine_state_t state_s14_settings_xscrew(signal_t *signal);
void state_s14_change();
machine_state_t state_s15_settings_ymove(signal_t *signal);
void state_s15_change();
machine_state_t state_s16_settings_ymoving(signal_t *signal);
void state_s16_change();
machine_state_t state_s2_cw_ccw_decision(signal_t *signal);
void state_s2_change();
machine_state_t state_s3_coil_turns_decision(signal_t *signal);
void state_s3_change();
machine_state_t state_s4_wire_thick_decision(signal_t *signal);
void state_s4_change();
machine_state_t state_s41_distance_decision(signal_t *signal);
void state_s41_change();
machine_state_t state_s5_summary(signal_t *signal);
void state_s5_change();
machine_state_t state_s6_run(signal_t *signal);
void state_s6_change();
machine_state_t state_s61_cancel(signal_t *signal);
void state_s61_change();
machine_state_t state_s7_done(signal_t *signal);
void state_s7_change();

/*
 * callbacks for cotrioll machine
 */
void x_stepper_step(bool dir);
void motor_enable(bool en);
void y_stepper_step(bool dir);
long currentTimeUs();
void app_print(char *_1st_line, char *_2nd_line);

void read_flash(machine_static_params_t *machine_static_params);
void write_flash(machine_static_params_t *machine_static_params);

/*
 * API functions call form main
 */
void app_init(print_fun_t print_fun, step_fun_t x_step, step_fun_t y_step,
		get_us_fun_t get_us, motor_enable_fun_t motor_enable,
		write_flash_fun_t write_flash_fun, read_flash_fun_t read_flash_fun);
void app_put_signal(signal_t signal);
void app_state_machine_loop();
void app_machine_rotated_indicator();

#endif /* WINDER_MACHINE_H_ */
