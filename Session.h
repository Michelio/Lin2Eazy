#ifndef SESSION_H
#define SESSION_H

#include <QRunnable>

#include "Socket.h"
#include "PacketParser.h"

class Session : public Socket, public QRunnable
{

private:
    PacketParser parser;

public:
    Session();
    Session(HANDLE, HANDLE);
    Session(Session*);
    ~Session();
    bool StartPipes() override;
    void Parse(BYTE*);

    PacketParser *GetParser();
    void operator=(const Session*);

protected:
    void run() override;
};

#endif // SESSION_H
