#include "PacketParser.h"

#include <QList>
#include <QPair>

#include "Glob.h"

PacketParser::PacketParser()
{

}

void PacketParser::SkipBytes(size_t& position, size_t count)
{
    position += count;
}

uint8_t PacketParser::ReadBytes8(size_t& position, BYTE* packet)
{
    uint8_t value;
    memcpy(&value, &packet[position], sizeof(value));
    position++;

    return value;
}

uint16_t PacketParser::ReadBytes16(size_t& position, BYTE* packet)
{
    uint16_t value;
    memcpy(&value, &packet[position], sizeof(value));
    position += 2;

    return value;
}

uint32_t PacketParser::ReadBytes32(size_t& position, BYTE* packet)
{
    uint32_t value;
    memcpy(&value, &packet[position], sizeof(value));
    position += 4;

    return value;
}

uint64_t PacketParser::ReadBytes64(size_t& position, BYTE* packet)
{
    uint64_t value;
    memcpy(&value, &packet[position], sizeof(value));
    position += 8;

    return value;
}

QString PacketParser::ReadBytesStr(size_t& position, BYTE* packet)
{
    QString string;

    string.append(QString::fromUtf16((char16_t*)(&packet[position])));
    SkipBytes(position, string.size() * sizeof(char16_t));
    position += 2;

    return string;
}

void PacketParser::ParsePacket(BYTE* packet)
{
    size_t pos = 0;
    uint8_t packetType = ReadBytes8(pos, packet);
    uint8_t packetID = ReadBytes8(pos, packet);

    if (packetType == 0x00)
    {
        if (packetID == 0x48)
        {
            uint32_t x = ReadBytes32(pos, packet);
            uint32_t y = ReadBytes32(pos, packet);
            uint32_t z = ReadBytes32(pos, packet);

            Coordinates currentPosition(x, y, z);

            emit PositionValidated(currentPosition);
        }
    }

    else if (packetType == 0x01)
    {
        if (packetID == 0x01)
        {
            uint32_t objId = ReadBytes32(pos, packet);
            uint32_t x = ReadBytes32(pos, packet);
            uint32_t y = ReadBytes32(pos, packet);
            uint32_t z = ReadBytes32(pos, packet);

            Coordinates destPosition(x, y, z);

            emit ObjectMoved(objId, destPosition);
        }

        else if (packetID == 0x04)
        {
            Character character;

            uint32_t x = ReadBytes32(pos, packet);
            uint32_t y = ReadBytes32(pos, packet);
            uint32_t z = ReadBytes32(pos, packet);

            Coordinates position(x, y, z);
            character.SetPosition(position);

            SkipBytes(pos, sizeof(uint32_t));

            character.SetObjectId(ReadBytes32(pos, packet));
            character.SetName(ReadBytesStr(pos, packet));

            SkipBytes(pos, 3 * sizeof(uint32_t));

            character.SetLevel(ReadBytes32(pos, packet));
            character.SetExp(ReadBytes32(pos, packet));

            SkipBytes(pos, sizeof(uint32_t));

            uint32_t STR, DEX, CON,
                     INT, WIT, MEN;

            STR = ReadBytes32(pos, packet);
            DEX = ReadBytes32(pos, packet);
            CON = ReadBytes32(pos, packet);
            INT = ReadBytes32(pos, packet);
            WIT = ReadBytes32(pos, packet);
            MEN = ReadBytes32(pos, packet);

            character.SetStats(STR, DEX, CON,
                               INT, WIT, MEN);

            character.SetMaxHp(ReadBytes32(pos, packet));
            character.SetCurHp(ReadBytes32(pos, packet));
            character.SetMaxMp(ReadBytes32(pos, packet));
            character.SetCurMp(ReadBytes32(pos, packet));
            character.SetSp(ReadBytes32(pos, packet));
            character.SetCurWeight(ReadBytes32(pos, packet));
            character.SetMaxWeight(ReadBytes32(pos, packet));

            SkipBytes(pos, 35 * sizeof(uint32_t));
            SkipBytes(pos, 68 * sizeof(uint8_t));

            character.SetPAttack(ReadBytes32(pos, packet));
            character.SetPAttackSpd(ReadBytes32(pos, packet));
            character.SetPDefense(ReadBytes32(pos, packet));
            character.SetEvasion(ReadBytes32(pos, packet));
            character.SetAccuracy(ReadBytes32(pos, packet));
            character.SetCritical(ReadBytes32(pos, packet));
            character.SetMAttack(ReadBytes32(pos, packet));
            character.SetCastSpd(ReadBytes32(pos, packet));

            SkipBytes(pos, sizeof(uint32_t));

            character.SetMDefense(ReadBytes32(pos, packet));

            SkipBytes(pos, sizeof(uint32_t));

            character.SetKarma(ReadBytes32(pos, packet));

            character.SetSpeed(ReadBytes32(pos, packet));

            SkipBytes(pos, 19 * sizeof(uint32_t));

            character.SetTitle(ReadBytesStr(pos, packet));

            SkipBytes(pos, 3 * sizeof(uint64_t));

            character.SetPkCount(ReadBytes32(pos, packet));
            character.SetPvpCount(ReadBytes32(pos, packet));

            uint16_t cubesCount = ReadBytes16(pos, packet);

            if (cubesCount > 0)
                SkipBytes(pos, cubesCount * sizeof(uint16_t));

            SkipBytes(pos, sizeof(uint8_t));
            SkipBytes(pos, sizeof(uint16_t));
            SkipBytes(pos, 6 * sizeof(uint32_t));

            character.SetMaxCp(ReadBytes32(pos, packet));
            character.SetCurCp(ReadBytes32(pos, packet));

            emit UserInfo(character);
        }

        else if (packetID == 0x05)
        {
            uint32_t objectId = ReadBytes32(pos, packet);
            uint32_t targetObjectId = ReadBytes32(pos, packet);
            uint32_t damage = ReadBytes32(pos, packet);

            emit EntityAttack(objectId, targetObjectId, damage);
        }

        else if (packetID == 0x06)
        {
            uint32_t objectId = ReadBytes32(pos, packet);

            emit KillEntity(objectId);
        }

        else if (packetID == 0x0E)
        {
            QList<QPair<uint32_t, uint32_t>> stats;

            uint32_t objectId = ReadBytes32(pos, packet);
            uint32_t statsCount = ReadBytes32(pos, packet);
            uint32_t statId = 0;
            uint32_t statValue = 0;

            for (int i = 0; i < (int)statsCount; i++)
            {
                statId = ReadBytes32(pos, packet);
                statValue = ReadBytes32(pos, packet);
                stats.append(qMakePair(statId, statValue));
            }

            emit StatusUpdate(objectId, stats);
        }

        else if (packetID == 0x12)
        {
            NonPlayerCharacter npc;

            npc.SetObjectId(ReadBytes32(pos, packet));

            emit RemoveNPC(npc);
        }

        else if (packetID == 0x13)
            emit SelectCharacter();

        else if (packetID == 0x15)
        {
            Character character;

            character.SetName(ReadBytesStr(pos, packet));
            SkipBytes(pos, sizeof(uint32_t));
            character.SetTitle(ReadBytesStr(pos, packet));

            SkipBytes(pos, 7 * sizeof(uint32_t));

            uint32_t x = ReadBytes32(pos, packet);
            uint32_t y = ReadBytes32(pos, packet);
            uint32_t z = ReadBytes32(pos, packet);

            Coordinates position(x, y, z);
            character.SetPosition(position);

            emit CharacterSelected();
            emit UserInfo(character);
        }

        else if (packetID == 0x16)
        {
            NonPlayerCharacter npc;

            npc.SetObjectId(ReadBytes32(pos, packet));
            npc.SetNPCId(ReadBytes32(pos, packet) - 1000000);
            npc.SetAttackable(ReadBytes32(pos, packet));
            uint32_t x = ReadBytes32(pos, packet);
            uint32_t y = ReadBytes32(pos, packet);
            uint32_t z = ReadBytes32(pos, packet);
            Coordinates position(x, y, z);
            npc.SetPosition(position);

            SkipBytes(pos, 24 * sizeof(uint32_t));
            SkipBytes(pos, sizeof(uint8_t));

            npc.SetName(ReadBytesStr(pos, packet));
            npc.SetTitle(ReadBytesStr(pos, packet));

            npc.SetIsChampion(ReadBytes32(pos, packet));

            emit NPCInfo(npc);
        }

        else if (packetID == 0x28)
        {
            uint32_t charId = ReadBytes32(pos, packet);

            emit CharacterTeleported(charId);
        }

        else if (packetID == 0x29)
        {
            uint32_t objectId = ReadBytes32(pos, packet);
            uint32_t targetObjId = ReadBytes32(pos, packet);

            emit TargetSelected(objectId, targetObjId);
        }

        else if (packetID == 0x2A)
        {
            uint32_t objectId = ReadBytes32(pos, packet);

            emit TargetUnselected(objectId);
        }

        else if (packetID == 0x47)
        {
            uint32_t objId = ReadBytes32(pos, packet);
            uint32_t x = ReadBytes32(pos, packet);
            uint32_t y = ReadBytes32(pos, packet);
            uint32_t z = ReadBytes32(pos, packet);

            Coordinates destPosition(x, y, z);

            emit ObjectMoved(objId, destPosition);
        }

        else if (packetID == 0x4A)
        {
            uint32_t objId = ReadBytes32(pos, packet);
            ChatType chatType = (ChatType)ReadBytes32(pos, packet);
            QString owner = ReadBytesStr(pos, packet);
            QString text = ReadBytesStr(pos, packet);

            if (objId == 0)
            {
                chatType = ANNOUNCEMENT;
                owner = "Announcement";
            }

            emit MessageReceived(chatType, Message(owner, text));
        }

        else if (packetID == 0x60)
        {
            uint32_t objId = ReadBytes32(pos, packet);
            uint32_t targetObjId = ReadBytes32(pos, packet);

            emit MoveToPawn(objId, targetObjId);
        }

        else if (packetID == 0x7E)
        {
            emit LoggedOut();
        }

        else if (packetID == 0xA6)
        {
            uint32_t targetObjId = ReadBytes32(pos, packet);

            emit MyTargetSelected(targetObjId);
        }
    }
}
