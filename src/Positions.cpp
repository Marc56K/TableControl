#include "Positions.h"
#include <EEPROM.h>

int getSavedUpPosition()
{
    int result = 0;
    EEPROM.get(0, result);
    return result;
}

void saveUpPosition(int pos)
{
    EEPROM.put(0, pos);
}

int getSavedDownPosition()
{
    int result = 0;
    EEPROM.get(sizeof(int), result);
    return result;
}

void saveDownPosition(int pos)
{
    EEPROM.put(sizeof(int), pos);
}