#include <stdint.h>

typedef struct {
    uint8_t leds[3];
    uint8_t relays[3];
    uint32_t temp_sensors[4];
} sim_state_t;
