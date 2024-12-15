#include "cross.h"

// Cross g-code provider algorithm constants
#define CROSS_DISTANCE_DIVIDER 5
#define CROSS_DISTANCE_STEP 0.1
#define CROSS_SECTION_PERCENT 50
#define MAX_GCODES 1000 // Maksymalna liczba G-code'ów

// Parameters for the cross winding algorithm
static float cross_distance = 0;
static int cross_turns = 0;
static bool cross_clockwise = false;
static int cross_amount;
static int cross_density;

// Algorithm state management
static int cross_algorithm_step = 0;
static int first_turn_point;
static int second_turn_point;

// Machine state
static int current_turn = 0;
static float current_distance = 0;

// G-code buffer
static g_code_t gcode_buffer[MAX_GCODES]; // Statyczna tablica na G-code'y
static int gcode_count = 0;
static int gcode_index = 0; // Index to track the current G-code being returned

// Function prototypes
static void generate_normal_winding(float distance, int turns, g_code_t* gcode, machine_offsets_t* machine_offset);
static void generate_cross_winding(g_code_t* gcode, machine_offsets_t* machine_offset);
static void update_turn_and_distance(float distance, int turns);
static void generate_all_gcodes(machine_offsets_t *machine_offset);

void cross_init(void *arg) {
    cross_alg_args *args = (cross_alg_args*)arg;

    if (args == NULL) {
        appV2_error("cross_alg_args is null");
        return;
    }

    cross_distance = args->distance;
    cross_turns = args->turns;
    cross_clockwise = args->cw;
    cross_amount = args->cross_amount;
    cross_density = args->cross_density;

    current_turn = 0;
    current_distance = 0;
    cross_algorithm_step = 0;

    first_turn_point = ((100 - cross_amount) / 100) * cross_turns;
    second_turn_point = (cross_amount / 100) * cross_turns;

    gcode_count = 0;
    gcode_index = 0;

    // Wygeneruj wszystkie G-code'y na starcie
    generate_all_gcodes(NULL); // Przykładowo przekazujemy NULL, ale w realnym użyciu przekaż machine_offset
}

bool cross_is_program_finished() {
    return gcode_index >= gcode_count; // Program kończy się, gdy wszystkie G-code'y zostaną zwrócone
}

bool cross_is_gcode_available() {
    return gcode_index < gcode_count; // G-code'y są dostępne, dopóki indeks nie przekroczy ilości wygenerowanych kodów
}

g_code_t* cross_get_next_gcode(machine_offsets_t *machine_offset) {
    if (cross_is_gcode_available()) {
        return &gcode_buffer[gcode_index++]; // Zwróć aktualny G-code i zwiększ indeks
    }
    return NULL;
}

// Generuje wszystkie G-code'y na początku
static void generate_all_gcodes(machine_offsets_t *machine_offset) {
    for (int i = 0; i < cross_turns && gcode_count < MAX_GCODES; i++) { // Sprawdzamy, czy nie przekroczymy maksymalnej liczby G-code'ów
        if (i < first_turn_point) {
            generate_normal_winding(cross_distance + (machine_offset ? machine_offset->offset_plus : 0), i + round(cross_distance / CROSS_DISTANCE_STEP), &gcode_buffer[gcode_count], machine_offset);
        } else if (i >= first_turn_point && i < second_turn_point) {
            generate_cross_winding(&gcode_buffer[gcode_count], machine_offset);
        } else if (i >= second_turn_point) {
            generate_normal_winding(current_distance < (cross_distance / 2) ? cross_distance + (machine_offset ? machine_offset->offset_plus : 0) : -(machine_offset ? machine_offset->offset_minus : 0), i + round(cross_distance / CROSS_DISTANCE_STEP), &gcode_buffer[gcode_count], machine_offset);
        }

        gcode_count++; // Zwiększ ilość wygenerowanych kodów
    }
}

static void generate_normal_winding(float distance, int turns, g_code_t* gcode, machine_offsets_t* machine_offset) {
    if (turns > cross_turns) {
        turns = cross_turns;
    }

    gcode->type = G1;
    gcode->X = distance;
    gcode->A = cross_clockwise ? -turns : turns;

    update_turn_and_distance(distance, turns);
}

static void generate_cross_winding(g_code_t* gcode, machine_offsets_t* machine_offset) {
    switch (cross_algorithm_step) {
        case 0:
            generate_normal_winding(cross_distance + (machine_offset ? machine_offset->offset_plus : 0), current_turn + CROSS_DISTANCE_DIVIDER, gcode, machine_offset);
            cross_algorithm_step++;
            break;
        case 1:
            current_turn += 0.5;
            gcode->type = G1;
            gcode->X = current_distance;
            gcode->A = cross_clockwise ? -current_turn : current_turn;
            cross_algorithm_step++;
            break;
        case 2:
            generate_normal_winding(-(machine_offset ? machine_offset->offset_minus : 0), current_turn + CROSS_DISTANCE_DIVIDER, gcode, machine_offset);
            cross_algorithm_step++;
            break;
        case 3:
            current_turn += 0.5;
            gcode->type = G1;
            gcode->X = current_distance;
            gcode->A = cross_clockwise ? -current_turn : current_turn;
            cross_algorithm_step = 0;
            break;
        default:
            break;
    }
}

static void update_turn_and_distance(float distance, int turns) {
    current_distance = distance;
    current_turn = turns;
}

// G-code provider setup
gcode_provider_t cross_alg_provider = {
    .init = cross_init,
    .get_next_gcode = cross_get_next_gcode,
    .is_gcode_available = cross_is_gcode_available,
    .is_program_finished = cross_is_program_finished,
};
