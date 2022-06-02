#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <QObject>

#include <windows.h>
#include "Protocol.h"
#include "Character.h"
#include "NonPlayerCharacter.h"
#include "Chat.h"

class PacketParser : public QObject
{
    Q_OBJECT

private:
    //ServerPacketProtocol *protocol;

signals:
    // client received
    void CharacterLoaded(QString);
    void UserInfo(Character);
    void NPCInfo(NonPlayerCharacter);
    void RemoveNPC(NonPlayerCharacter);
    void CharacterTeleported(uint32_t);
    void ObjectMoved(uint32_t, Coordinates);
    void MessageReceived(ChatType, Message);
    void MoveToPawn(uint32_t, uint32_t);
    void TargetSelected(uint32_t, uint32_t);
    void TargetUnselected(uint32_t);
    void MyTargetSelected(uint32_t);
    void StatusUpdate(uint32_t, QList<QPair<uint32_t, uint32_t>>);
    void SelectCharacter();
    void CharacterSelected();
    void EntityAttack(uint32_t, uint32_t, uint32_t);
    void KillEntity(uint32_t);
    void LoggedOut();

    // client sent
    void PositionValidated(Coordinates);

public:
    PacketParser();
    void SkipBytes(size_t&, size_t);
    uint8_t ReadBytes8(size_t&, BYTE*);
    uint16_t ReadBytes16(size_t&, BYTE*);
    uint32_t ReadBytes32(size_t&, BYTE*);
    uint64_t ReadBytes64(size_t&, BYTE*);
    QString ReadBytesStr(size_t&, BYTE*);
    void ParsePacket(BYTE*);
};

#endif // PACKETPARSER_H
