#pragma once
#include <Arduino.h>
#include "LogicData.h"

class Table
{
public:
    enum MoveDir
    {
        MOVE_DIR_NONE = 0,
        MOVE_DIR_UP,
        MOVE_DIR_DOWN
    };
public:
    void init();

    int readPosition();

    void moveTo(int pos);
    void moveUp();
    void moveDown();
    void stop();

    void update(int current_pos);

    MoveDir getMoveDir();
    bool isMovingToPosition();

private:
    int _target_pos = -1;
    MoveDir _move_dir = MOVE_DIR_NONE;
};