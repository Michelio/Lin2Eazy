#ifndef PLAYER_H
#define PLAYER_H

#include "AliveEntity.h"

class Player : public AliveEntity
{
protected:
    uint32_t curCp;
    uint32_t maxCp;
    uint32_t targetObjectId;
    bool pvpFlag;

public:
    Player();
    Player(uint32_t, Coordinates, QString, EntityType, QString, bool, ActionType, uint32_t);
    bool isTarget(uint32_t);

    // getters
    uint32_t GetCurCp();
    uint32_t GetMaxCp();
    uint32_t GetTargetObjectId();

    // setters
    void SetCurCp(uint32_t);
    void SetMaxCp(uint32_t);
    void SetCp(uint32_t, uint32_t);
    void SetTargetObjectId(uint32_t);


};

#endif // PLAYER_H
