#include "Socket.h"

Socket::Socket()
{
    qDebug("Socket()");
    sendPipe = INVALID_HANDLE_VALUE;
    receivePipe = INVALID_HANDLE_VALUE;
    sendConnected = false;
    receiveConnected = false;
}

Socket::~Socket()
{
    qDebug("~Socket()");
}

void Socket::ClosePipes()
{
    runSocket = false;

    HANDLE rPipe = CreateFileW(PIPE_SEND_NAME, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    HANDLE sPipe = CreateFileW(PIPE_RECEIVE_NAME, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (rPipe != INVALID_HANDLE_VALUE)
        CloseHandle(rPipe);

    if (sPipe != INVALID_HANDLE_VALUE)
        CloseHandle(sPipe);
}

bool Socket::StartPipes()
{
    while(runSocket)
    {
        sendPipe = CreateNamedPipeW(PIPE_SEND_NAME, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            30, 0, BUFSIZE, NMPWAIT_USE_DEFAULT_WAIT, NULL);

        if (sendPipe == INVALID_HANDLE_VALUE)
            return false;

        else qDebug("good senPipe");

        receivePipe = CreateNamedPipeW(PIPE_RECEIVE_NAME, PIPE_ACCESS_INBOUND, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            30, BUFSIZE, 0, NMPWAIT_USE_DEFAULT_WAIT, NULL);

        if (receivePipe == INVALID_HANDLE_VALUE)
        {
            CloseHandle(sendPipe);
            return false;
        }

        else qDebug("good receivePipe");

        sendConnected = ConnectNamedPipe(sendPipe, NULL) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        receiveConnected = ConnectNamedPipe(receivePipe, NULL) ?
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (sendConnected && receiveConnected)
        {
            qDebug("emit ClientConnected");
            emit ClientConnected(receivePipe, sendPipe);
        }
    }

    return true;
}
