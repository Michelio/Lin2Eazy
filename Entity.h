#ifndef ENTITY_H
#define ENTITY_H

#include <QString>
#include <QtMath>

#include <stdint.h>
#include "datasource.h"

enum EntityType
{
    UNKNOWN = 0,
    NPC = 1,
    ATTACKABLE_NPC = 2,
    PLAYER = 3,
    CHARACTER = 4,
    ITEM = 5
};

struct Coordinates
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint32_t distanceToCharacter;
    Coordinates();
    Coordinates(uint32_t, uint32_t, uint32_t);
};

class Entity
{
protected:
    uint32_t objectId;
    Coordinates position;
    QString name;
    EntityType eType;

public:
    Entity();
    Entity(uint32_t, Coordinates);
    Entity(uint32_t, Coordinates, QString, EntityType);
    uint32_t CalculateDistance(Coordinates);

    // getters
    uint32_t GetObjectId();
    Coordinates GetPosition();
    QString GetName();
    EntityType GetEntityType();
    uint32_t GetDistance();

    // setters
    void SetObjectId(uint32_t);
    void SetPosition(Coordinates);
    void SetPosition(Coordinates, uint32_t);
    void SetName(QString);
    void SetEntityType(EntityType);

    // operators
    bool operator<(const Entity&) const;
    bool operator==(const Entity&) const;
    bool operator!=(const Entity&) const;
    bool operator==(const uint32_t) const;
    bool operator!=(const uint32_t) const;
};

#endif // ENTITY_H
