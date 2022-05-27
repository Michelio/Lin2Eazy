#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>

#include <Windows.h>

#define PIPE_RECEIVE_NAME L"\\\\.\\pipe\\PipeReceive"
#define PIPE_SEND_NAME L"\\\\.\\pipe\\PipeSend"
#define BUFSIZE 1 << 16

class Socket : public QObject
{
    Q_OBJECT

private:
    bool sendConnected;
    bool receiveConnected;
    bool runSocket = true;

protected:
    HANDLE sendPipe;
    HANDLE receivePipe;
    void stop();

public:
    Socket();
    ~Socket();
    virtual bool StartPipes();
    void ClosePipes();

signals:
    void ClientConnected(HANDLE, HANDLE);

};

#endif // SOCKET_H
