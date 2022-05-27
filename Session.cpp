#include "Session.h"

Session::Session()
{
    qDebug("Session()");
}

Session::Session(HANDLE receivePipe, HANDLE sendPipe)
{
    qDebug("Session(HANDLE, HANDLE)");
    this->receivePipe = receivePipe;
    this->sendPipe = sendPipe;
}

Session::Session(Session *session)
{
    qDebug("Session(Session&)");
    receivePipe = session->receivePipe;
    sendPipe = session->sendPipe;
}

Session::~Session()
{
    qDebug("~Session()");
}

bool Session::StartPipes()
{
    DWORD read;
    BYTE buffer[BUFSIZE];
    bool fSuccess = FALSE;

    if (receivePipe == NULL)
        qDebug("\nERROR - Pipe Receive Server Failure.\n");
    qDebug("InstanceThread created, receiving and processing messages.\n");


    while (1)
    {
        fSuccess = ReadFile(receivePipe, buffer, sizeof(buffer), &read, NULL);
        if (!fSuccess)
        {
            qDebug("!fSuccess");
            break;
        }

        else Parse(buffer);
    }

    return 1;
}

void Session::Parse(BYTE* packet)
{
    parser.ParsePacket(packet);
}

PacketParser *Session::GetParser()
{
    return &parser;
}

void Session::operator=(const Session *session)
{
    receivePipe = session->receivePipe;
    sendPipe = session->sendPipe;
}

void Session::run()
{
    this->StartPipes();
}
