#include "Buttons.h"
#include "Config.h"

#define NUM_BUTTONS 3
#define LONG_PRESS_START_DURATION 500
#define PRESS_COUNT_RESET_DURATION 200

namespace Buttons
{
struct Button
{
    uint8_t button_pin = 0;
    bool is_pressed = false;
    unsigned long last_duration = 0;
    unsigned long last_change = 0;
    int last_num_presses = 0; // -1 = long press
    int num_button_presses = 0;

    void init(uint8_t pin) volatile
    {
        button_pin = pin;
        pinMode(button_pin, INPUT_PULLUP);
        is_pressed = digitalRead(button_pin) == LOW;
        last_change = millis();
    }

    void getState(State& s) volatile
    {
        const int n = num_button_presses;
        s.long_press = n < 0;
        s.num_presses = max(0, n);
    }

    void update() volatile
    {
        const unsigned long now = millis();        
        const unsigned long duration = (now < last_change) ? 0 : (now - last_change);

        if (duration > 50)
        {
            const bool is_pressed_now = digitalRead(button_pin) == LOW;

            if (is_pressed_now == is_pressed)
            {
                if (!is_pressed_now)
                {
                    if (duration > PRESS_COUNT_RESET_DURATION)
                    {
                        num_button_presses = last_num_presses;
                        last_num_presses = 0;
                    }
                }
                else if (duration > LONG_PRESS_START_DURATION)
                {
                    num_button_presses = -1;
                    last_num_presses = -1;
                }

                return;
            }

            if (!is_pressed_now)
            {
                ++last_num_presses;
            }

            is_pressed = is_pressed_now;
            last_duration = duration;
            last_change = now;
        }
    }
};

volatile bool is_updating = false;
volatile Button buttons[NUM_BUTTONS];

void update()
{
    if (is_updating)
        return;
        
    is_updating = true;
    for (int i = 0; i < NUM_BUTTONS; ++i)
    {
        buttons[i].update();
    }
    is_updating = false;
}

void init()
{
    buttons[0].init(UP_BUTTTON_PIN);
    buttons[1].init(DOWN_BUTTON_PIN);
    buttons[2].init(SAVE_BUTTON_PIN);

    pinMode(BUTTONS_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTONS_INTERRUPT_PIN), update, CHANGE);
}

States read()
{
    update();
    States result;
    buttons[0].getState(result.up_button);
    buttons[1].getState(result.down_button);
    buttons[2].getState(result.save_button);
    return result;
}
}