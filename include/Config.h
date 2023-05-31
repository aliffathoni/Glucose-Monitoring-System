int state = 2; int a = 0;
#define REPORTING_PERIOD_MS     1000
uint32_t tsLastReport = 0;

String ssid, pass, uid;

bool connection_status = false;

#include <Wire.h>
#include "driver/rtc_io.h"
#include "MAX30100_PulseOximeter.h"
#include "Network.h"
#include "Fuzzy.h"
#include <SPI.h>
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

PulseOximeter pox;