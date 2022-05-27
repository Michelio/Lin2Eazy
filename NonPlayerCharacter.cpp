#include "NonPlayerCharacter.h"
#include "Tables.h"

#include <QDataStream>

NonPlayerCharacter::NonPlayerCharacter()
{
    npcId = 0;
    aggroRange = 0;
    isChampion = false;
    hpRate = 1;
}

NonPlayerCharacter::NonPlayerCharacter(uint32_t objectId, Coordinates position, QString name, EntityType eType,
         QString title, bool alive, ActionType aType, uint32_t npcId)
    : AliveEntity(objectId, position, name, eType, title, alive, aType)
{
    this->npcId = npcId;
}

void NonPlayerCharacter::CalculateStats(const NPCData &npcData)
{
    if (curHp == maxHp)
    {
        maxHp = npcData.baseHpMax * mod_CON[npcData.baseCON] * hpRate;
        curHp = maxHp;
    }

    else maxHp = npcData.baseHpMax * mod_CON[npcData.baseCON] * hpRate;

    if (curMp == maxMp)
    {
        maxMp = npcData.baseMpMax * mod_MEN[npcData.baseMEN];
        curMp = maxMp;
    }

    else maxMp = npcData.baseMpMax * mod_MEN[npcData.baseMEN];

    speed = npcData.baseSpeed;
    CalculateSpeed(npcData.baseDEX, 1, 0);
}

uint32_t NonPlayerCharacter::GetNPCId()
{
    return npcId;
}

uint32_t NonPlayerCharacter::GetAggroRange()
{
    return aggroRange;
}

uint32_t NonPlayerCharacter::GetHpRate()
{
    return hpRate;
}

bool NonPlayerCharacter::IsChampion()
{
    return isChampion;
}

void NonPlayerCharacter::SetNPCId(uint32_t npcId)
{
    this->npcId = npcId;
}

void NonPlayerCharacter::SetAggroRange(uint32_t aggroRange)
{
    this->aggroRange = aggroRange;
}

void NonPlayerCharacter::SetHpRate(uint32_t hpRate)
{
    this->hpRate = hpRate;
}

void NonPlayerCharacter::SetIsChampion(bool isChampion)
{
    this->isChampion = isChampion;
}

void NonPlayerCharacter::operator=(const NPCData &npcData)
{
    title = npcData.title;
    name = npcData.name;
    aggroRange = npcData.aggroRange;
    level = npcData.level;

    CalculateStats(npcData);
}
