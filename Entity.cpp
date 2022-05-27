#include "Entity.h"

Entity::Entity()
{
    objectId = -1;
    name = "";
    eType = UNKNOWN;
}

Entity::Entity(uint32_t objectId, Coordinates position)
{
    this->objectId = objectId;
    this->position = position;
}

Entity::Entity(uint32_t objectId, Coordinates position, QString name, EntityType eType)
{
    this->objectId = objectId;
    this->position = position;
    this->name = name;
    this->eType = eType;
}

uint32_t Entity::CalculateDistance(Coordinates position)
{
    uint32_t dx = position.x - this->position.x;
    uint32_t dy = position.y - this->position.y;
    uint32_t dz = position.z - this->position.z;
    this->position.distanceToCharacter = sqrt(dx*dx + dy*dy + dz*dz);

    return this->position.distanceToCharacter;
}

uint32_t Entity::GetObjectId()
{
    return objectId;
}

Coordinates Entity::GetPosition()
{
    return position;
}

QString Entity::GetName()
{
    return name;
}

EntityType Entity::GetEntityType()
{
    return eType;
}

void Entity::SetObjectId(uint32_t objectId)
{
    this->objectId = objectId;
}

void Entity::SetPosition(Coordinates position)
{
    this->position = position;
}

void Entity::SetPosition(Coordinates position, uint32_t offset)
{
    this->position.x = position.x + offset;
    this->position.y = position.y + offset;
}

void Entity::SetName(QString name)
{
    this->name = name;
}

void Entity::SetEntityType(EntityType eType)
{
    this->eType = eType;
}

bool Entity::operator<(const Entity &entity) const
{
    return position.distanceToCharacter < entity.position.distanceToCharacter;
}

bool Entity::operator==(const Entity &entity) const
{
    return objectId == entity.objectId;
}

bool Entity::operator!=(const Entity &entity) const
{
    return objectId != entity.objectId;
}

bool Entity::operator==(const uint32_t objectId) const
{
    return this->objectId == objectId;
}

bool Entity::operator!=(const uint32_t objectId) const
{
    return this->objectId != objectId;
}

Coordinates::Coordinates()
{
    x = 0;
    y = 0;
    z = 0;
    distanceToCharacter = 0;
}

Coordinates::Coordinates(uint32_t x, uint32_t y, uint32_t z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    distanceToCharacter = 0;
}
