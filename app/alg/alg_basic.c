#include "alg.h"

static float this_coil_distance;
static float this_wire_thick;

bool direction;

void init_basic_alg(float coil_distance, float wire_thick, void *data) {
	this_coil_distance = coil_distance;
	this_wire_thick = wire_thick;

	direction = true;
}

void basic_alg(int turns, float x, float x_offset_minus_mm,
		float x_offset_plus_mm) {
	if (direction) { /* comes of winder*/
		if (x + this_wire_thick
				>= this_coil_distance + x_offset_plus_mm) {
			direction = !direction;
		} else {
			machine_go_to_x(x + this_wire_thick);
		}
	} else {
		if (x - this_wire_thick >= 0 - x_offset_minus_mm) {
			direction = !direction;
		} else {
			machine_go_to_x(x - this_wire_thick);
		}
	}
}

/**
 * 			..			..
 * 			||			||
 * 			||**********||
 * 			||**********||	----------->
 * 			||			||	winder site
 * 			||**********||
 * 			||**********||
 * 			||			||
 * 			  <-------->  machine_params.coil_distance
 * 		   <->          <->
 * 	x_offset_plus_mm   x_offset_minus_mm
 *         <-------------->  total distane
 *
 */

