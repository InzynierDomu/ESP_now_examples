#pragma once

#include <stdint.h>

namespace config
{
const uint8_t my_card_id[4] = {0xE0, 0x64, 0xA7, 0x19};

const uint8_t slave_address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

const char* ssid = "Jeremek_NET";

const uint8_t pin_rst = 9;
const uint8_t pin_ss = 10;

const uint8_t screen_width = 128;
const uint8_t screen_height = 64;
} // namespace config