#pragma once

#include <stdint.h>

namespace config
{
const uint8_t my_card_id[4] = {0xE0, 0x64, 0xA7, 0x19};

const uint8_t slave_address[] = {0x3C, 0xE9, 0x0E, 0x7F, 0x30, 0x80};

const char* ssid = "ssid";

const uint8_t pin_rst = 9;
const uint8_t pin_ss = 10;

const uint8_t screen_width = 128;
const uint8_t screen_height = 64;
} // namespace config