#include "Display.h"
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Display::Display() : _oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
{
}

void Display::init()
{
  if (!_oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("Failed to init SSD1306"));
    while (true);
  }
}

void Display::update(const Content& content)
{
  _oled.setTextSize(3);
  _oled.setTextColor(WHITE);
  _oled.clearDisplay();

  _oled.setCursor(12, 18);
  _oled.print(content.current_position);

  _oled.setTextSize(1);
  _oled.setCursor(96, 10);
  _oled.print(content.up_position);
  _oled.setCursor(96, 40);
  _oled.print(content.down_position);

  _oled.setCursor(4, 50);
  switch(content.move_dir)
  {
    case Table::MOVE_DIR_UP:
      _oled.print("MOVING UP...");
      break;
    case Table::MOVE_DIR_DOWN:
      _oled.print("MOVING DOWN...");
      break;
  }

  _oled.display();
}