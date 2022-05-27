#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BotLogic.h"
#include "AccountsDelegate.h"
#include "AccountsModel.h"
#include "NPCDelegate.h"
#include "NPCModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void SetupConnections(BotLogic*);
    void DisconnectConnections();
    void SetupConnections();
    void TargetUICleanUp();
    void CharacterUICleanUp();
    void UpdateTargetLayout();
    void UpdateTargetNameLayout();
    void UpdateMainLayout();
    void UpdateNameLayout();
    void UpdateSubLayout();

    void SetCurrentClient(GameLogic*);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    AccountsModel accounts;
    AccountsDelegate accountsDelegate;
    NPCModel npcs;
    NPCDelegate npcsDelegate;
    GameLogic *currentClient;
    QModelIndex currentClientIndex;

private slots:
    void SetupClientConnections(GameLogic*);
    void UpdateLogChat();
    void UpdateCharInfo();
    void UpdateCurrentCharacterStats(int);
    void UpdateCharacterStats(DWORD);
    void UpdateCheckBoxesState();
    void RefreshNPCList();
    void UpdateNPCInfo(int);
    void UpdateNPCPosition(uint32_t);
    void UpdateAccountsSectionsSizes();
    void UpdateNPCsSectionsSizes();
    void SelectTarget();
    void UnselectTarget();
    void SelectMyTarget();
    void UpdateTargetStats();
    void AddAccountToList(GameLogic*);
    void UpdateAccountInfo(DWORD);
    void RemoveAccountFromList(GameLogic*);
    void SelectAccount(const QModelIndex);
    void MakeAccountCurrent(const QModelIndex);
    void RaiseAccount();
    void LowerAccount();
};

#endif // MAINWINDOW_H
