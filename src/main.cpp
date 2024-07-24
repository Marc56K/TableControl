#include <Arduino.h>
#include "Config.h"
#include "Display.h"
#include "Sensor.h"
#include "Table.h"
#include "Buttons.h"
#include "Positions.h"

Display display;
Sensor sensor;
Table table;

void setup()
{
  Serial.begin(115200);

  display.init();
  sensor.init();
  table.init();

  Buttons::init();
}

int getTablePosition()
{
  int pos = table.readPosition();
  if (pos < 0)
  {
    return sensor.getDistance();
  }
  return pos;
}

void loop()
{
  auto btns = Buttons::read();
  int current_pos = getTablePosition();
  int up_pos = getSavedUpPosition();
  int dwn_pos = getSavedDownPosition();

  if (btns.save_button.long_press)
  {
    if (btns.up_button.num_presses == 1)
    {
      up_pos = current_pos;
      saveUpPosition(up_pos);
    }

    if (btns.down_button.num_presses == 1)
    {
      dwn_pos = current_pos;
      saveDownPosition(dwn_pos);
    }
  }
  else
  {
    if (btns.up_button.num_presses == 1 || btns.down_button.num_presses == 1)
    {
      table.stop();
    }

    if (btns.up_button.num_presses == 2)
    {
      table.moveTo(up_pos);
    }
    else if (btns.up_button.long_press)
    {
      if (table.isMovingToPosition())
      {
        table.stop();
      }
      table.moveUp();
    }
    
    if (btns.down_button.num_presses == 2)
    {
      table.moveTo(dwn_pos);
    }
    else if (btns.down_button.long_press)
    {
      if (table.isMovingToPosition())
      {
        table.stop();
      }
      table.moveDown();
    }

    if (!table.isMovingToPosition()
      && btns.up_button.num_presses == 0 
      && !btns.up_button.long_press 
      && btns.down_button.num_presses == 0 
      && !btns.down_button.long_press)
    {
      table.stop();
    }
  }

  table.update(current_pos);

  Display::Content content;
  content.current_position = current_pos;
  content.up_position = up_pos;
  content.down_position = dwn_pos;
  content.move_dir = table.getMoveDir();
  display.update(content);
}