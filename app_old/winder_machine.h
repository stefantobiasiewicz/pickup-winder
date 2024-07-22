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

#include "states_def.h"

#include "states/state_s05.h"
#include "states/state_s1.h"
#include "states/winding_config/state_s2.h"
#include "states/winding_config/state_s3.h"
#include "states/winding_config/state_s4.h"
#include "states/winding_config/state_s41.h"
#include "states/state_s5.h"
#include "states/state_s6.h"
#include "states/state_s61.h"
#include "states/state_s7.h"
#include "states/state_select_alg.h"
#include "states/settings/state_s11.h"
#include "states/settings/state_esc_config.h"
#include "states/settings/state_rotor_test.h"

#include "alg/alg.h"


typedef struct {
    bool cw;
    int coil_turns;
    float wire_size;
    float coil_distance;
    /* X offset minus in mm (correction form base to winder)*/
    float x_offset_minus_mm;
    /* X offset plus (correction form up plate out winder)*/
    float x_offset_plus_mm;

    int speed;
    bool manual;
    wind_alg_t algorithm;
} machine_params_t;

extern machine_params_t machine_params;

typedef enum {
    INIT = 0xaa,
    NO_INIT = 0xff
} flash_init_t;

typedef enum { STEPPER,
    PWM } motor_type_t;

typedef struct {
    flash_init_t init;
    int x_motor_steps;
    float x_screw;

    motor_type_t motor_type;
    /* rotor as PWM motor*/
    int max_pwm;
    int min_pwm;
} machine_static_params_t;

extern machine_static_params_t machine_static_params;

/*
 * callbacks def for cotrioll machine
 */
typedef void (*print_fun_t)(char* _1st_line, char* _2nd_line);
typedef void (*step_fun_t)(bool dir);
typedef void (*motor_enable_fun_t)(bool dir);
typedef long (*get_us_fun_t)();
typedef void (*write_flash_fun_t)(
    machine_static_params_t* machine_static_params);
typedef void (*read_flash_fun_t)(machine_static_params_t* machine_static_params);
typedef void (*set_motor_pwm_t)(int pwm);
typedef void (*set_motor_relay_t)(bool pwm);


typedef struct {
	print_fun_t print_fun;
	step_fun_t step_fun_x;
	step_fun_t step_fun_y;
	motor_enable_fun_t motor_enable_fun;
	get_us_fun_t get_us_fun;
	write_flash_fun_t write_flash_fun;
	read_flash_fun_t read_flash_fun;
	set_motor_pwm_t set_motor_pwm;
	set_motor_relay_t set_motor_relay;
}machine_control_t;

/*
 * Application states declaration
 */

void initialize_menu();


/*
 * callbacks for cotrioll machine
 */
void x_stepper_step(bool dir);
void motor_enable(bool en);
void y_stepper_step(bool dir);
long currentTimeUs();
void app_print(char* _1st_line, char* _2nd_line);

void read_flash(machine_static_params_t* machine_static_params);
void write_flash(machine_static_params_t* machine_static_params);


void motor_speed(int speed);
void motor_relay(bool dir);
/*
 * API functions call form main
 */
void app_init(machine_control_t machine_controll_callbacks);
void app_put_signal(signal_t signal);
void app_state_machine_loop();
void app_machine_rotated_indicator();

#endif /* WINDER_MACHINE_H_ */
