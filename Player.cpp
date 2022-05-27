#include "Player.h"

Player::Player()
{

}

bool Player::isTarget(uint32_t objectId)
{
    if (objectId == targetObjectId)
        return true;

    else return false;
}


// Getters
uint32_t Player::GetCurCp()
{
    return curCp;
}

uint32_t Player::GetMaxCp()
{
    return maxCp;
}

uint32_t Player::GetTargetObjectId()
{
    return targetObjectId;
}


// Setters
void Player::SetCurCp(uint32_t curCp)
{
    this->curCp = curCp;
}

void Player::SetMaxCp(uint32_t maxCp)
{
    this->maxCp = maxCp;
}

void Player::SetCp(uint32_t curCp, uint32_t maxCp)
{
    this->curCp = curCp;
    this->maxCp = maxCp;
}

void Player::SetTargetObjectId(uint32_t targetObjectId)
{
    this->targetObjectId = targetObjectId;
}
