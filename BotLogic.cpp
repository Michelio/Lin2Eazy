#include "BotLogic.h"
#include "datasource.h"

BotLogic::BotLogic()
{
    qDebug("BotLogic()");
    threadPool.setMaxThreadCount(30);
    inj.start();
    //inj->CreateTempDirectory();

    DataSource *s = DataSource::dataSource();

    connect(&inj, &Injector::DllInjected, this, &BotLogic::AddClient);
    connect(&ipc, &Socket::ClientConnected, this, &BotLogic::AddSession);
}

BotLogic::~BotLogic()
{
    qDebug("~BotLogic()");
    ipc.ClosePipes();
    threadPool.clear();
    threadPool.deleteLater();

    for (int i = 0; i < clients.count(); i++)
        delete clients[i];

    clients.clear();
    quit();
}

QList<GameLogic *> *BotLogic::GetClients()
{
    return &clients;
}


// Slots
void BotLogic::AddClient(DWORD id)
{
    clients.append(new GameLogic(id));
}

void BotLogic::AddSession(HANDLE receivePipe, HANDLE sendPipe)
{
    int id = clients.count() - 1;

    if (id >= 0)
    {
        Session *session = new Session(receivePipe, sendPipe);

        clients.at(id)->SetSession(session);
        clients.at(id)->GetSession()->setAutoDelete(true);
        clients.at(id)->SetupConnections();
        threadPool.start(clients.at(id)->GetSession());
        if (clients.count() == 1)
        {
            clients.at(id)->SetIsCurrent(true);
            emit SessionReady(clients.at(id));
        }

        emit ClientConnected(clients.at(id));
    }
}

void BotLogic::run()
{
    ipc.StartPipes();
}

