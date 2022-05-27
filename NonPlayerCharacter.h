#ifndef NONPLAYERCHARACTER_H
#define NONPLAYERCHARACTER_H

#include "AliveEntity.h"

#include <QMetaType>

class NonPlayerCharacter : public AliveEntity
{
protected:
    uint32_t npcId;
    uint32_t aggroRange;
    uint32_t hpRate;
    bool isChampion;

public:
    NonPlayerCharacter();
    NonPlayerCharacter(uint32_t, Coordinates, QString, EntityType, QString, bool, ActionType, uint32_t);
    void CalculateStats(const NPCData&);

    // Getters
    uint32_t GetNPCId();
    uint32_t GetAggroRange();
    uint32_t GetHpRate();
    bool IsChampion();

    // Setters
    void SetNPCId(uint32_t);
    void SetAggroRange(uint32_t);
    void SetHpRate(uint32_t);
    void SetIsChampion(bool);

    // Operators
    void operator=(const NPCData&);
};

Q_DECLARE_METATYPE(NonPlayerCharacter);
#endif // NONPLAYERCHARACTER_H
