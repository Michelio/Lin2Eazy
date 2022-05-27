#include "Character.h"

Character::Character()
{
    exp = 0;
    stats = { 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0,
              0, 0, 0, 0, 0 };
    speed = 0;
    sp = 0;
    curWeight = 0;
    maxWeight = 0;
    title = "";
    pvpFlag = 0;
    pkCount = 0;
    pvpCount = 0;
}

// Getters
Characteristics Character::GetStats()
{
    return stats;
}

uint32_t Character::GetExp()
{
    return exp;
}

uint32_t Character::GetSp()
{
    return sp;
}

uint32_t Character::GetCurWeight()
{
    return curWeight;
}

uint32_t Character::GetMaxWeight()
{
    return maxWeight;
}

uint32_t Character::GetPAttack()
{
    return stats.pAttack;
}

uint32_t Character::GetPAttackSpd()
{
    return stats.pAttackSpeed;
}

uint32_t Character::GetPDefense()
{
    return stats.pDefense;
}

uint32_t Character::GetEvasion()
{
    return stats.evasion;
}

uint32_t Character::GetAccuracy()
{
    return stats.accuracy;
}

uint32_t Character::GetCritical()
{
    return stats.critical;
}

uint32_t Character::GetMAttack()
{
    return stats.mAttack;
}

uint32_t Character::GetCastSpd()
{
    return stats.castingSpeed;
}

int Character::UpdateStats(QList<QPair<uint32_t, uint32_t>> stats)
{
    int charMainStats = 0,
        charSubStats  = 0;

    for (int i = 0; i < stats.count(); i++)
        switch (stats[i].first)
        {
        case 0x00000001:
            level = stats[i].second;
            charMainStats = 1;
            break;
        case 0x00000002:
            exp = stats[i].second;
            charMainStats = 1;
            break;
        case 0x00000003:
            this->stats.STR = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000004:
            this->stats.DEX = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000005:
            this->stats.CON = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000006:
            this->stats.INT = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000007:
            this->stats.WIT = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000008:
            this->stats.MEN = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000009:
            curHp = stats[i].second;
            charMainStats = 1;
            break;
        case 0x0000000A:
            maxHp = stats[i].second;
            charMainStats = 1;
            break;
        case 0x0000000B:
            curMp = stats[i].second;
            charMainStats = 1;
            break;
        case 0x0000000C:
            maxMp = stats[i].second;
            charMainStats = 1;
            break;
        case 0x0000000D:
            sp = stats[i].second;
            charSubStats = 2;
            break;
        case 0x0000000E:
            curWeight = stats[i].second;
            break;
        case 0x0000000F:
            maxWeight = stats[i].second;
            break;
        case 0x00000010:
        case 0x00000011:
            this->stats.pAttack = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000012:
            this->stats.pAttackSpeed = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000013:
            this->stats.pDefense = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000014:
            this->stats.evasion = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000015:
            this->stats.accuracy = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000016:
            this->stats.critical = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000017:
            this->stats.mAttack = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000018:
            this->stats.castingSpeed = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000019:
            this->stats.mDefense = stats[i].second;
            charSubStats = 2;
            break;
        case 0x0000001A:
            pvpFlag = stats[i].second;
            break;
        case 0x0000001B:
            karma = stats[i].second;
            charSubStats = 2;
            break;
        case 0x00000021:
            curCp = stats[i].second;
            charMainStats = 1;
            break;
        case 0x00000022:
            maxCp = stats[i].second;
            charMainStats = 1;
            break;
        }

    return charMainStats + charSubStats;
}

uint32_t Character::GetMDefense()
{
    return stats.mDefense;
}

uint32_t Character::GetKarma()
{
    return karma;
}

uint32_t Character::GetPkCount()
{
    return pkCount;
}

uint32_t Character::GetPvpCount()
{
    return pvpCount;
}

uint32_t Character::GetInventorySize()
{
    return inventorySize;
}


// Setters
void Character::SetStats(Characteristics stats)
{
    this->stats = stats;
}

void Character::SetStats(uint32_t STR, uint32_t DEX, uint32_t CON, uint32_t INT, uint32_t WIT, uint32_t MEN)
{
    stats.STR = STR;
    stats.DEX = DEX;
    stats.CON = CON;
    stats.INT = INT;
    stats.WIT = WIT;
    stats.MEN = MEN;
}

void Character::SetSTR(uint32_t STR)
{
    stats.STR = STR;
}

void Character::SetDEX(uint32_t DEX)
{
    stats.DEX = DEX;
}

void Character::SetCON(uint32_t CON)
{
    stats.CON = CON;
}

void Character::SetINT(uint32_t INT)
{
    stats.INT = INT;
}

void Character::SetWIT(uint32_t WIT)
{
    stats.WIT = WIT;
}

void Character::SetMEN(uint32_t MEN)
{
    stats.MEN = MEN;
}

void Character::SetExp(uint32_t exp)
{
    this->exp = exp;
}

void Character::SetSp(uint32_t sp)
{
    this->sp = sp;
}

void Character::SetCurWeight(uint32_t curWeight)
{
    this->curWeight = curWeight;
}

void Character::SetMaxWeight(uint32_t maxWeight)
{
    this->maxWeight = maxWeight;
}

void Character::SetPAttack(uint32_t pAttack)
{
    stats.pAttack = pAttack;
}

void Character::SetPAttackSpd(uint32_t pAttackSpeed)
{
    stats.pAttackSpeed = pAttackSpeed;
}

void Character::SetPDefense(uint32_t pDefense)
{
    stats.pDefense = pDefense;
}

void Character::SetEvasion(uint32_t evasion)
{
    stats.evasion = evasion;
}

void Character::SetAccuracy(uint32_t accuracy)
{
    stats.accuracy = accuracy;
}

void Character::SetCritical(uint32_t critical)
{
    stats.critical = critical;
}

void Character::SetMAttack(uint32_t mAttack)
{
    stats.mAttack = mAttack;
}

void Character::SetCastSpd(uint32_t castingSpeed)
{
    stats.castingSpeed = castingSpeed;
}

void Character::SetMDefense(uint32_t mDefense)
{
    stats.mDefense = mDefense;
}

void Character::SetKarma(uint32_t karma)
{
    this->karma = karma;
}

void Character::SetPvpCount(uint32_t pvpCount)
{
    this->pvpCount = pvpCount;
}

void Character::SetPkCount(uint32_t pkCount)
{
    this->pkCount = pkCount;
}

void Character::SetInventorySize(uint32_t inventorySize)
{
    this->inventorySize = inventorySize;
}
