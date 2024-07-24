#include "Table.h"
#include "Config.h"

LogicData* ld = nullptr;

//-- Buffered mode parses input words and sends them to output separately
void logicDataPin_ISR()
{
    ld->PinChange(digitalRead(TABLE_DATA_PIN) == HIGH);
}

void Table::init()
{
    stop();

    pinMode(JUMPER_PIN, INPUT_PULLUP);
    if (digitalRead(JUMPER_PIN) == HIGH)
    {
        ld = new LogicData(0);
        if (!ld)
        {
            Serial.println(F("Failed to initialize LogicData!"));
            while (true);
        }
        pinMode(TABLE_DATA_PIN, INPUT);
        logicDataPin_ISR();
        attachInterrupt(digitalPinToInterrupt(TABLE_DATA_PIN), logicDataPin_ISR, CHANGE);
        ld->Begin();
    }
}

int Table::readPosition()
{
    if (ld)
    {
        uint32_t msg = ld->ReadTrace();
        return ld->GetNumber(msg);
    }
    return -1;
}

void Table::moveTo(int pos)
{
    _target_pos = pos;
}

void Table::moveUp()
{
    _move_dir = MOVE_DIR_UP;
    pinMode(TABLE_DOWN_PIN, INPUT);
    pinMode(TABLE_UP_PIN, OUTPUT);
    digitalWrite(TABLE_UP_PIN, HIGH);
}

void Table::moveDown()
{
    _move_dir = MOVE_DIR_DOWN;
    pinMode(TABLE_UP_PIN, INPUT);
    pinMode(TABLE_DOWN_PIN, OUTPUT);
    digitalWrite(TABLE_DOWN_PIN, HIGH);
}

void Table::stop()
{
    _target_pos = -1;
    _move_dir = MOVE_DIR_NONE;
    pinMode(TABLE_UP_PIN, INPUT);
    pinMode(TABLE_DOWN_PIN, INPUT);
}

void Table::update(int current_pos)
{
    if (isMovingToPosition())
    {
        switch (_move_dir)
        {
            case MOVE_DIR_NONE:
            {
                if (current_pos > _target_pos)
                    moveDown();
                else if (current_pos < _target_pos)
                    moveUp();
                break;
            }
            case MOVE_DIR_UP:
            {
                if (current_pos >= _target_pos)
                    stop();
                break;
            }
            case MOVE_DIR_DOWN:
            {
                if (current_pos <= _target_pos)
                    stop();
                break;
            }
        }
    }
}

Table::MoveDir Table::getMoveDir()
{
    return _move_dir;
}

bool Table::isMovingToPosition()
{
    return _target_pos > -1;
}