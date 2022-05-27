#ifndef BOTLOGIC_H
#define BOTLOGIC_H

#include <QThreadPool>
#include <QThread>

#include "Injector.h"
#include "GameLogic.h"

class BotLogic : public QThread
{
    Q_OBJECT

public:
    BotLogic();
    ~BotLogic();

    QList<GameLogic *> *GetClients();

private:
    Injector inj;
    Socket ipc;
    QThreadPool threadPool;
    QList<GameLogic*> clients;


signals:
    void SessionReady(GameLogic*);
    void ClientConnected(GameLogic*);
    void stopInjector();
    void stopSocket();

private slots:
    void AddClient(DWORD);
    void AddSession(HANDLE, HANDLE);

protected:
    void run();

};

#endif // BOTLOGIC_H
