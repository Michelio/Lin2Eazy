#ifndef ALIVEENTITY_H
#define ALIVEENTITY_H

#include "Entity.h"
#include "Tables.h"

#include <QList>
#include <QPair>

enum ActionType {
    SITTING = 0,
    STANDING = 1,
    RUNNING = 2,
    CASTING = 3,
    ATTACKING = 4,
    DEAD = 5
};

class AliveEntity : public Entity
{
protected:
    uint32_t level;
    uint32_t curHp;
    uint32_t maxHp;
    uint32_t curMp;
    uint32_t maxMp;
    uint32_t speed;
    QString title;
    bool alive;
    bool attackable;
    ActionType aType;
    bool isTarget;

public:
    AliveEntity();
    AliveEntity(uint32_t, Coordinates, QString, EntityType, QString, bool, ActionType);
    void UpdateStats(QList<QPair<uint32_t, uint32_t>>);
    void CalculateSpeed(uint32_t, uint32_t, uint32_t);

    // Getters
    uint32_t GetLevel();
    uint32_t GetCurHp();
    uint32_t GetMaxHp();
    uint32_t GetCurMp();
    uint32_t GetMaxMp();
    uint32_t GetSpeed();
    QString GetTitle();
    bool GetAlive();
    bool GetAttackable();
    ActionType GetActionType();
    bool IsTarget();

    // Setters
    void SetLevel(uint32_t);
    void SetCurHp(uint32_t);
    void SetMaxHp(uint32_t);
    void SetHp(uint32_t, uint32_t);
    void SetCurMp(uint32_t);
    void SetMaxMp(uint32_t);
    void SetMp(uint32_t, uint32_t);
    void SetSpeed(uint32_t);
    void SetTitle(QString);
    void SetAlive(bool);
    void SetAttackable(bool);
    void SetActionType(ActionType);
    void SetIsTarget(bool);
};

#endif // ALIVEENTITY_H
