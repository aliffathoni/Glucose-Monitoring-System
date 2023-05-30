int state = 2; int a = 0;
#define REPORTING_PERIOD_MS     1000
uint32_t tsLastReport = 0;

String ssid, pass, uid;

bool connection_status = false;

#include <Wire.h>

#include "MAX30100_PulseOximeter.h"
#include "Network.h"
#include "Fuzzy.h"

PulseOximeter pox;