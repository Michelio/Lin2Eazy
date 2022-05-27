#include "AliveEntity.h"

AliveEntity::AliveEntity()
{
    level = 0;
    curHp = 1;
    maxHp = 1;
    curMp = 1;
    maxMp = 1;
    speed = 1;
    title = "";
    alive = true;
    attackable = false;
    aType = STANDING;
}

AliveEntity::AliveEntity(uint32_t objectId, Coordinates position, QString name, EntityType eType,
                         QString title, bool alive, ActionType aType)
    :Entity(objectId, position, name, eType)
{
    level = 0;
    curHp = 1;
    maxHp = 1;
    curMp = 1;
    maxMp = 1;
    speed = 1;
    this->title = title;
    this->alive = alive;
    (eType == ATTACKABLE_NPC) ? this->attackable = true : this->attackable = false;
    this->aType = aType;
}

void AliveEntity::UpdateStats(QList<QPair<uint32_t, uint32_t>> stats)
{
    for (int i = 0; i < stats.count(); i++)
        switch (stats[i].first)
        {
            case 0x00000009:
            curHp = stats[i].second;
            break;
            case 0x0000000A:
            maxHp = stats[i].second;
            break;
            case 0x0000000B:
            curMp = stats[i].second;
            break;
            case 0x0000000C:
            maxMp = stats[i].second;
            break;
        }
}

void AliveEntity::CalculateSpeed(uint32_t baseDEX, uint32_t mod_per, uint32_t mod_diff)
{
    speed = speed * mod_DEX[baseDEX] * mod_per + mod_diff;
}

uint32_t AliveEntity::GetLevel()
{
    return level;
}

uint32_t AliveEntity::GetCurHp()
{
    return curHp;
}

uint32_t AliveEntity::GetMaxHp()
{
    return maxHp;
}

uint32_t AliveEntity::GetCurMp()
{
    return curMp;
}

uint32_t AliveEntity::GetMaxMp()
{
    return maxMp;
}

uint32_t AliveEntity::GetSpeed()
{
    return speed;
}

QString AliveEntity::GetTitle()
{
    return title;
}

bool AliveEntity::GetAlive()
{
    return alive;
}

bool AliveEntity::GetAttackable()
{
    return attackable;
}

ActionType AliveEntity::GetActionType()
{
    return aType;
}

bool AliveEntity::IsTarget()
{
    return isTarget;
}

void AliveEntity::SetLevel(uint32_t level)
{
    this->level = level;
}

void AliveEntity::SetCurHp(uint32_t curHp)
{
    this->curHp = curHp;
}

void AliveEntity::SetMaxHp(uint32_t maxHp)
{
    this->maxHp = maxHp;
}

void AliveEntity::SetHp(uint32_t curHp, uint32_t maxHp)
{
    this->curHp = curHp;
    this->maxHp = maxHp;
}

void AliveEntity::SetCurMp(uint32_t curMp)
{
    this->curMp = curMp;
}

void AliveEntity::SetMaxMp(uint32_t maxMp)
{
    this->maxMp = maxMp;
}

void AliveEntity::SetMp(uint32_t curMp, uint32_t maxMp)
{
    this->curMp = curMp;
    this->maxMp = maxMp;
}

void AliveEntity::SetSpeed(uint32_t speed)
{
    this->speed = speed;
}

void AliveEntity::SetTitle(QString title)
{
    this->title = title;
}

void AliveEntity::SetAlive(bool alive)
{
    this->alive = alive;
}

void AliveEntity::SetAttackable(bool attackable)
{
    this->attackable = attackable;
}

void AliveEntity::SetActionType(ActionType aType)
{
    this->aType = aType;
}

void AliveEntity::SetIsTarget(bool isTarget)
{
    this->isTarget = isTarget;
}

