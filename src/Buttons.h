#pragma once
#include <Arduino.h>

namespace Buttons
{
    struct State
    {
        bool long_press = false;
        int num_presses = 0;
    };

    struct States
    {
        State up_button;
        State down_button;
        State save_button;
    };

    void init();
    States read();
}