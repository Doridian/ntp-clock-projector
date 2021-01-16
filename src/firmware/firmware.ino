/*
 * Copyright 2018 Chris Spencer (chrisspen@gmail.com)
 * Edited 2020 by Mark Dietzer (git@doridian.net)
 *
 * Firmware for the NTP Projection Clock.
*/

#include <ESP8266WiFi.h>
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()
#include <TZ.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"

#include "credentials.h" // Needs WIFI_SSID and WIFI_PASSWORD

////////////////////////////////////////////////////////

#define CLOCK_UPDATE_T 100
#define DISCONNECTED_T 500

#define MYTZ TZ_America_Los_Angeles

unsigned long last_clock_update = 0;
unsigned long last_blink_time = 0;

bool cbtime_set = false;

Adafruit_7segment matrix = Adafruit_7segment();

bool colon = true;

int last_minute = -1;
int last_hour = -1;
bool last_colon = false;

void write_time() {
    if (WiFi.status() == WL_CONNECTED) {
        colon = 1;
    } else if (millis() - last_blink_time >= DISCONNECTED_T) {
        // If disconnected from wifi, blink colon quickly.
        colon = !colon;
        last_blink_time = millis();
    }

    int minute = 88;
    int hour = 88;

    if (cbtime_set) {
        time_t now;
        now = time(nullptr);
        const tm* tm = localtime(&now);

        minute = tm->tm_min;
        hour = tm->tm_hour;
        if (hour == 24) {
          hour = 0;
        }
    }

    if (hour != last_hour || minute != last_minute || colon != last_colon) {
        last_hour = hour;
        last_minute = minute;
        last_colon = colon;
        
        int minute1 = minute/1%10;
        int minute2 = minute/10%10;

        int hour1 = hour/1%10;
        int hour2 = hour/10%10;
        
        matrix.writeDigitNumMirrored(0, minute1, false);  // minute, low digit
        matrix.writeDigitNumMirrored(1, minute2, false);  // minute, high digit
        matrix.drawColon(colon);
        matrix.writeDigitNumMirrored(3, hour1, false);  // hour, low digit
        if (hour2) {
          matrix.writeDigitNumMirrored(4, hour2, false);  // hour, high digit
        } else {
          matrix.turnOffDigit(4);
        }

        matrix.writeDisplay();
    }
}

void time_is_set(void) {
    cbtime_set = true;
}

void setup() {
    // https://github.com/esp8266/Arduino/blob/master/libraries/Wire/Wire.cpp
    // On the esp8266, you can manually specify the i2c pins.
    Wire.begin();  // SDA=A5=D2, SCL=A4=D1

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // Turn off built-in LED always

    matrix.begin(0x70);

    settimeofday_cb(time_is_set);

    // configTime(TZ_SEC, DST_SEC, "pool.ntp.org");
    configTime(MYTZ, "pool.ntp.org");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
  if (millis() - last_clock_update >= CLOCK_UPDATE_T) {
    last_clock_update = millis();
    write_time();
  }
}
