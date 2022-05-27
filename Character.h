#ifndef CHARACTER_H
#define CHARACTER_H

#include <QMetaType>

#include "Player.h"

struct Characteristics
{
    uint32_t STR, DEX, CON,
             INT, WIT, MEN;
    uint32_t pAttack;
    uint32_t pAttackSpeed;
    uint32_t pDefense;
    uint32_t evasion;
    uint32_t accuracy;
    uint32_t critical;
    uint32_t mAttack;
    uint32_t castingSpeed;
    uint32_t mDefense;
};

class Character : public Player
{
private:
    uint32_t exp;
    Characteristics stats;
    uint32_t sp;
    uint32_t curWeight;
    uint32_t maxWeight;
    uint32_t karma;
    uint32_t pvpFlag;
    uint32_t pkCount;
    uint32_t pvpCount;
    uint32_t inventorySize;


public:
    Character();
    int UpdateStats(QList<QPair<uint32_t, uint32_t>>);

    // Getters
    uint32_t GetExp();
    Characteristics GetStats();
    uint32_t GetSp();
    uint32_t GetCurWeight();
    uint32_t GetMaxWeight();
    uint32_t GetPAttack();
    uint32_t GetPAttackSpd();
    uint32_t GetPDefense();
    uint32_t GetEvasion();
    uint32_t GetAccuracy();
    uint32_t GetCritical();
    uint32_t GetMAttack();
    uint32_t GetCastSpd();
    uint32_t GetMDefense();
    uint32_t GetKarma();
    uint32_t GetPkCount();
    uint32_t GetPvpCount();
    uint32_t GetInventorySize();

    // Setters
    void SetExp(uint32_t);
    void SetStats(Characteristics);
    void SetStats(uint32_t, uint32_t, uint32_t,
                  uint32_t, uint32_t, uint32_t);
    void SetSTR(uint32_t);
    void SetDEX(uint32_t);
    void SetCON(uint32_t);
    void SetINT(uint32_t);
    void SetWIT(uint32_t);
    void SetMEN(uint32_t);
    void SetSp(uint32_t);
    void SetCurWeight(uint32_t);
    void SetMaxWeight(uint32_t);
    void SetPAttack(uint32_t);
    void SetPAttackSpd(uint32_t);
    void SetPDefense(uint32_t);
    void SetEvasion(uint32_t);
    void SetAccuracy(uint32_t);
    void SetCritical(uint32_t);
    void SetMAttack(uint32_t);
    void SetCastSpd(uint32_t);
    void SetMDefense(uint32_t);
    void SetKarma(uint32_t);
    void SetPkCount(uint32_t);
    void SetPvpCount(uint32_t);
    void SetInventorySize(uint32_t);

};

Q_DECLARE_METATYPE(Character);
#endif // CHARACTER_H
