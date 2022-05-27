#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

struct ServerPacketProtocol
{
    uint8_t MOVE_TO_LOCATION;
    uint8_t CHAR_INFO;
    uint8_t USER_INFO;
    uint8_t STATUS_UPDATE;
    uint8_t DELETE_OBJECT;
    uint8_t CHAR_SELECTED;
    uint8_t NPC_INFO;
    uint8_t TARGET_SELECTED;
    uint8_t TARGET_UNSELECTED;
};

static const ServerPacketProtocol protocol
{
    0x01,   //MOVE_TO_LOCATION
    0x03,   //CHAR_INFO
    0x04,   //USER_INFO
    0x0E,   //STATUS_UPDATE
    0x12,   //DELETE_OBJECT
    0x15,   //CHAR_SELECTED
    0x16,   //NPC_INFO
    0x29,   //TARGET_SELECTED
    0x2A    //TARGET_UNSELECTED
};

#endif // PROTOCOL_H
