#pragma clang diagnostic ignored "-Wc++11-extensions"
#pragma clang diagnostic ignored "-Wpragma-once-outside-header"
#pragma once
#include <stdint.h>
#include "Arduino.h"

// separator
const uint8_t SEPARATOR = 0xff;

// incoming
const uint8_t GIVE_WATER = 0x00, STOP_WATER = 0x01, PLAY_SOUND = 0x02;

// outgoing
const uint8_t WATER_STAMP = 0x00, SOUND_STAMP = 0x02, TOUCH_CHAN_0 = 0x03, TOUCH_CHAN_1 = 0x04, LEVER = 0x05;

inline uint8_t* u32tobyte(uint32_t input) {
    uint8_t* buffer = new uint8_t[4];
    buffer[3] = (uint8_t)input;
    buffer[2] = (uint8_t)(input >> 8);
    buffer[1] = (uint8_t)(input >> 16);
    buffer[0] = (uint8_t)(input >> 24);
    return buffer;
}

inline uint8_t* i32tobyte(int32_t input) {
    uint8_t* buffer = new uint8_t[4];
    buffer[3] = (uint8_t)input;
    buffer[2] = (uint8_t)(input >> 8);
    buffer[1] = (uint8_t)(input >> 16);
    buffer[0] = (uint8_t)(input >> 24);
    return buffer;
}

inline int32_t bytetoi32(uint8_t* input) {
    int32_t result = (int32_t)(input[0] << 24 | input[1] << 16 | input[2] << 8 | input[3]);
    delete [] input;
    return result;
}

inline uint32_t bytetou32(uint8_t* input) {
    uint32_t result = (uint32_t)(input[0] << 24 | input[1] << 16 | input[2] << 8 | input[3]);
    delete [] input;
    return result;
}

inline void send_signal(const uint8_t signal_type, uint8_t *signal_value) {
    uint8_t *serial_buffer;
    Serial.write(SEPARATOR);
    Serial.write(signal_type);
    serial_buffer = u32tobyte(millis());
    Serial.write(serial_buffer, 4);
    delete [] serial_buffer;
    serial_buffer = signal_value;
    Serial.write(serial_buffer, 4);
    delete [] signal_value;
}

inline uint32_t receive_signal(uint8_t* signal_type, uint8_t* signal_value) {
    uint32_t available = Serial.available();
    uint32_t result_count = 0;
    if (available > 2) {
        while (Serial.read() != SEPARATOR) {available--;}
        result_count = (available + 1) / 3;
        uint8_t* signal_type = new uint8_t[result_count];
        uint8_t* signal_value = new uint8_t[result_count];
        signal_type[0] = Serial.read();
        signal_value[0] = Serial.read();
        for (uint32_t idx = 1; idx < result_count; idx++) {
            Serial.read();
            signal_type[0] = Serial.read(); 
            signal_value[0] = Serial.read(); 
        }
        return result_count;
    } else return 0;
}
