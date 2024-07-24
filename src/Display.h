#pragma once
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "Table.h"

class Display
{
public:
    struct Content
    {
        int current_position = 0;
        int up_position = 0;
        int down_position = 0;
        Table::MoveDir move_dir = Table::MOVE_DIR_NONE;
    };

public:
    Display();
    void init();
    void update(const Content& content);

private:
    Adafruit_SSD1306 _oled;
};