#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "LittleFS.h"
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>

const uint32_t DISTANCE_NM_PER_ROTATION = 326000;

typedef struct {
    uint32_t speed;
    uint8_t direction;
    uint32_t distance;
    uint16_t interval;
} input;

String processor(const String &var);

float calculate_pause_interval(uint32_t);

void initialize_server();

void print_input();

extern input inputs[15];
#endif // !WEB_SERVER_H

