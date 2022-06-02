#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Character.h"
#include "NonPlayerCharacter.h"
#include "Tables.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->accountsTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(SelectAccount(QModelIndex)));
    connect(ui->accountsTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(MakeAccountCurrent(QModelIndex)));
    connect(ui->upArrowButton, SIGNAL(clicked()), this, SLOT(RaiseAccount()));
    connect(ui->downArrowButton, SIGNAL(clicked()), this, SLOT(LowerAccount()));

    ui->accountsTableView->setModel(&accounts);
    ui->accountsTableView->setItemDelegate(&accountsDelegate);
    ui->accountsTableView->horizontalHeader()->setMinimumSectionSize(ui->accountsTableView->minimumWidth()*0.25);
    ui->accountsTableView->horizontalHeader()->setMaximumSectionSize(ui->accountsTableView->minimumWidth()*0.75);
    ui->accountsTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->accountsTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    ui->npcListTableView->setModel(&npcs);
    ui->npcListTableView->setItemDelegate(&npcsDelegate);
    ui->npcListTableView->horizontalHeader()->setMinimumSectionSize(ui->accountsTableView->minimumWidth()*0.1);
    ui->npcListTableView->horizontalHeader()->setMaximumSectionSize(ui->accountsTableView->minimumWidth()*0.7);
    ui->npcListTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->npcListTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->npcListTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->npcListTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->npcListTableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    ui->hSplitter->setStretchFactor(0,0);
    ui->hSplitter->setStretchFactor(1,1);
    ui->hSplitter->setStretchFactor(2,1);

    ui->leftVSplitter->setStretchFactor(0,0);
    ui->leftVSplitter->setStretchFactor(1,1);

    ui->middleVSplitter->setStretchFactor(0,1);
    ui->middleVSplitter->setStretchFactor(1,0);

    ui->rightVSplitter->setStretchFactor(0,0);
    ui->rightVSplitter->setStretchFactor(1,1);
    ui->rightVSplitter->setStretchFactor(2,1);
}

MainWindow::~MainWindow()
{
    qDebug("~MainWindow()");
    delete ui;
}

void MainWindow::SetupClientConnections(GameLogic *client)
{
    currentClient = client;
    log.massageList.append(Message("BOT", "New client connected."));
    ui->logPTextEdit->appendPlainText(log.GetLastMessage());
    npcs.addValue(currentClient->GetNPCList());
    SetupConnections();
}

void MainWindow::UpdateLogChat(Message message)
{
    log.massageList.append(message);
    ui->logPTextEdit->setPlainText(log.GetLastMessage());
}

void MainWindow::SetupConnections(BotLogic *bl)
{
    accounts.AddValue(bl->GetClients());
    connect(bl, &BotLogic::SessionReady, this, &MainWindow::SetupClientConnections);
    connect(bl, &BotLogic::ClientConnected, this, &MainWindow::AddAccountToList);
    connect(ui->allCheckBox, &QCheckBox::clicked, this, &MainWindow::UpdateCheckBoxesState);
    connect(ui->accountsTableView->horizontalHeader(), &QHeaderView::geometriesChanged, this, &MainWindow::UpdateAccountsSectionsSizes);
    connect(ui->npcListTableView->horizontalHeader(), &QHeaderView::geometriesChanged, this, &MainWindow::UpdateNPCsSectionsSizes);
}

void MainWindow::DisconnectConnections()
{
    disconnect(currentClient, &GameLogic::CharacterInfoUpdated, this, &MainWindow::UpdateCharInfo);
    disconnect(currentClient, SIGNAL(MessageReceived(ChatType,Message)), this, SLOT(AddMessage(ChatType,Message)));
    disconnect(currentClient, &GameLogic::NPCListInfoChanged, this, &MainWindow::RefreshNPCList);
    disconnect(currentClient, &GameLogic::NPCPositionUpdated, this, &MainWindow::UpdateNPCPosition);
    disconnect(currentClient, &GameLogic::NPCListCleared, this, &MainWindow::RefreshNPCList);
    disconnect(currentClient, &GameLogic::TargetStatsUpdated, this, &MainWindow::UpdateTargetStats);
    disconnect(currentClient, &GameLogic::CurrentCharacterStatsChanged, this, &MainWindow::UpdateCurrentCharacterStats);
    disconnect(currentClient, &GameLogic::MyTargetSelected, this, &MainWindow::SelectMyTarget);
    disconnect(currentClient, &GameLogic::TargetUnselected, this, &MainWindow::UnselectTarget);
    disconnect(currentClient, &GameLogic::EntityStatsUpdated, this, &MainWindow::UpdateNPCInfo);
}

void MainWindow::SetupConnections()
{
    connect(currentClient, &GameLogic::CharacterInfoUpdated, this, &MainWindow::UpdateCharInfo);
    connect(currentClient, SIGNAL(MessageReceived(ChatType,Message)), this, SLOT(AddMessage(ChatType,Message)));
    connect(currentClient, &GameLogic::NPCListInfoChanged, this, &MainWindow::RefreshNPCList);
    connect(currentClient, &GameLogic::NPCPositionUpdated, this, &MainWindow::UpdateNPCPosition);
    connect(currentClient, &GameLogic::NPCListCleared, this, &MainWindow::RefreshNPCList);
    connect(currentClient, &GameLogic::TargetStatsUpdated, this, &MainWindow::UpdateTargetStats);
    connect(currentClient, &GameLogic::CurrentCharacterStatsChanged, this, &MainWindow::UpdateCurrentCharacterStats);
    connect(currentClient, &GameLogic::MyTargetSelected, this, &MainWindow::SelectMyTarget);
    connect(currentClient, &GameLogic::TargetUnselected, this, &MainWindow::UnselectTarget);
    connect(currentClient, &GameLogic::EntityStatsUpdated, this, &MainWindow::UpdateNPCInfo);
}

void MainWindow::TargetUICleanUp()
{
    ui->targetNameLabel->setText("No target");
    ui->targetHpBar->setMaximum(100);
    ui->targetHpBar->setValue(0);
    ui->targetMpBar->setMaximum(100);
    ui->targetMpBar->setValue(0);
    ui->targetTitleLabel->setMaximumHeight(0);
    ui->targetTitleLabel->setText("");
    ui->targetLevelLabel->setText("00");
}

void MainWindow::CharacterUICleanUp()
{
    ui->characterNameLabel->setText("No character");
    ui->characterLevelLabel->setText("00");
    ui->characterTitleLabel->setMaximumHeight(0);
    ui->characterCpBar->setMaximum(100);
    ui->characterCpBar->setValue(0);
    ui->characterHpBar->setMaximum(100);
    ui->characterHpBar->setValue(0);
    ui->characterMpBar->setMaximum(100);
    ui->characterMpBar->setValue(0);
    ui->characterExpBar->setMinimum(0);
    ui->characterExpBar->setMaximum(100);
    ui->characterExpBar->setValue(0);

    ui->pAttackLabel->setText("0");
    ui->mAttackLabel->setText("0");
    ui->pDefLabel->setText("0");
    ui->mDefLabel->setText("0");
    ui->accuracyLabel->setText("0");
    ui->evasionLabel->setText("0");
    ui->critRateLabel->setText("0");
    ui->speedLabel->setText("0");
    ui->attackSpeedLabel->setText("0");
    ui->castingSpeedLabel->setText("0");
    ui->karmaLabel->setText("0");
    ui->strLabel->setText("0");
    ui->conLabel->setText("0");
    ui->dexLabel->setText("0");
    ui->witLabel->setText("0");
    ui->menLabel->setText("0");
    ui->intLabel->setText("0");
}

void MainWindow::ChatCleanUp()
{
    ui->localPTextEdit->clear();
    ui->tradePTextEdit->clear();
    ui->partyPTextEdit->clear();
    ui->clanPTextEdit->clear();
}

void MainWindow::UpdateTargetLayout()
{
    if (currentClient->GetTarget())
    {
        ui->targetHpBar->setMaximum(currentClient->GetTarget()->GetMaxHp());
        ui->targetHpBar->setValue(currentClient->GetTarget()->GetCurHp());
        ui->targetMpBar->setMaximum(currentClient->GetTarget()->GetMaxMp());
        ui->targetMpBar->setValue(currentClient->GetTarget()->GetCurMp());
    }
}

void MainWindow::UpdateTargetNameLayout()
{
    if (currentClient->GetTarget())
    {
        ui->targetNameLabel->setText(currentClient->GetTarget()->GetName());
        ui->targetLevelLabel->setText(QString::number(currentClient->GetTarget()->GetLevel()));

        if (currentClient->GetTarget()->GetTitle().length() > 0)
        {
            ui->targetTitleLabel->setMaximumHeight(25);
            ui->targetTitleLabel->setText(currentClient->GetTarget()->GetTitle());
        }

        else ui->targetTitleLabel->setMaximumHeight(0);
    }

    else
        TargetUICleanUp();
}

void MainWindow::UpdateMainLayout()
{
    Character activeCharacter = currentClient->GetCharacter();

    ui->characterCpBar->setMaximum(activeCharacter.GetMaxCp());
    ui->characterCpBar->setValue(activeCharacter.GetCurCp());

    ui->characterHpBar->setMaximum(activeCharacter.GetMaxHp());
    ui->characterHpBar->setValue(activeCharacter.GetCurHp());

    ui->characterMpBar->setMaximum(activeCharacter.GetMaxMp());
    ui->characterMpBar->setValue(activeCharacter.GetCurMp());

    ui->characterExpBar->setMinimum(level.value(activeCharacter.GetLevel()));
    ui->characterExpBar->setMaximum(level.value(activeCharacter.GetLevel() + 1));
    ui->characterExpBar->setValue(activeCharacter.GetExp());
}

void MainWindow::UpdateNameLayout()
{
    ui->characterNameLabel->setText(currentClient->GetCharacter().GetName());
    ui->characterLevelLabel->setText(QString::number(currentClient->GetCharacter().GetLevel()));

    if (currentClient->GetCharacter().GetTitle().length() > 0)
    {
        ui->characterTitleLabel->setMaximumHeight(25);
        ui->characterTitleLabel->setText(currentClient->GetCharacter().GetTitle());
    }

    else ui->characterTitleLabel->setMaximumHeight(0);
}

void MainWindow::UpdateSubLayout()
{
    Character activeCharacter = currentClient->GetCharacter();
    ui->pAttackLabel->setText(QString::number(activeCharacter.GetPAttack()));
    ui->mAttackLabel->setText(QString::number(activeCharacter.GetMAttack()));
    ui->pDefLabel->setText(QString::number(activeCharacter.GetPDefense()));
    ui->mDefLabel->setText(QString::number(activeCharacter.GetMDefense()));
    ui->accuracyLabel->setText(QString::number(activeCharacter.GetAccuracy()));
    ui->evasionLabel->setText(QString::number(activeCharacter.GetEvasion()));
    ui->critRateLabel->setText(QString::number(activeCharacter.GetCritical()));
    ui->speedLabel->setText(QString::number(activeCharacter.GetSpeed()));
    ui->attackSpeedLabel->setText(QString::number(activeCharacter.GetPAttackSpd()));
    ui->castingSpeedLabel->setText(QString::number(activeCharacter.GetCastSpd()));
    ui->karmaLabel->setText(QString::number(activeCharacter.GetKarma()));
    ui->strLabel->setText(QString::number(activeCharacter.GetStats().STR));
    ui->conLabel->setText(QString::number(activeCharacter.GetStats().CON));
    ui->dexLabel->setText(QString::number(activeCharacter.GetStats().DEX));
    ui->witLabel->setText(QString::number(activeCharacter.GetStats().WIT));
    ui->menLabel->setText(QString::number(activeCharacter.GetStats().MEN));
    ui->intLabel->setText(QString::number(activeCharacter.GetStats().INT));
}

void MainWindow::SetCurrentClient(GameLogic *client)
{
    currentClient = client;
}

void MainWindow::UpdateCharInfo()
{
    Character activeCharacter = currentClient->GetCharacter();
    UpdateMainLayout();
    UpdateNameLayout();
    ui->pvpCountLabel->setText(QString::number(activeCharacter.GetPvpCount()));
    ui->pkCountLabel->setText(QString::number(activeCharacter.GetPkCount()));

    UpdateSubLayout();

    ui->spLabel->setText(QString::number(activeCharacter.GetSp()));

    accounts.UpdateTable();
}

void MainWindow::UpdateCurrentCharacterStats(int updatePriority)
{
    switch (updatePriority)
    {
        case 1:
        UpdateMainLayout();
        break;
        case 2:
        UpdateSubLayout();
        break;
        case 3:
        UpdateMainLayout();
        UpdateSubLayout();
        break;
    }
}

void MainWindow::UpdateCharacterStats(DWORD clientId)
{
    accounts.UpdateRow(clientId);
}

void MainWindow::UpdateCheckBoxesState()
{
    if (ui->allCheckBox->checkState())
    {
        ui->npcCheckBox->setCheckState(Qt::Checked);
        ui->npcCheckBox->setEnabled(false);
        ui->enemyCheckBox->setCheckState(Qt::Checked);
        ui->enemyCheckBox->setEnabled(false);
    }

    else
    {
        ui->npcCheckBox->setEnabled(true);
        ui->enemyCheckBox->setEnabled(true);
    }
}

void MainWindow::RefreshNPCList()
{
    npcs.updateTable();
}

void MainWindow::UpdateNPCInfo(int row)
{
    npcs.updateRow(row);
}

void MainWindow::UpdateNPCPosition(uint32_t objectId)
{

}

void MainWindow::AddMessage(ChatType type, Message message)
{
    switch(type)
    {
        case GENERAL:
        case SHOUT:
        case WHISPER:
        case ANNOUNCEMENT:
            ui->localPTextEdit->appendPlainText(message.time.toString() + "   "
                                               + message.owner + ": "
                                               + message.text);
            break;

        case PARTY:
            ui->partyPTextEdit->appendPlainText(message.time.toString() + "   "
                                               + message.owner + ": "
                                               + message.text);
            break;

        case TRADE:
            ui->tradePTextEdit->appendPlainText(message.time.toString() + "   "
                                               + message.owner + ": "
                                               + message.text);
            break;

        case ALLIANCE:
        case CLAN:
            ui->tradePTextEdit->appendPlainText(message.time.toString() + "   "
                                               + message.owner + ": "
                                               + message.text);
            break;

        default:
            ui->logPTextEdit->appendPlainText(message.time.toString() + "   "
                                               + message.owner + ": "
                                               + message.text);
            break;
    }
}

void MainWindow::UpdateAccountsSectionsSizes()
{
    ui->accountsTableView->horizontalHeader()->setMinimumSectionSize(ui->accountsTableView->geometry().width() * 0.25);
    ui->accountsTableView->horizontalHeader()->setMaximumSectionSize(ui->accountsTableView->geometry().width() * 0.75);
}

void MainWindow::UpdateNPCsSectionsSizes()
{
    ui->npcListTableView->horizontalHeader()->setMinimumSectionSize(ui->accountsTableView->geometry().width() * 0.1);
    ui->npcListTableView->horizontalHeader()->setMaximumSectionSize(ui->accountsTableView->geometry().width() * 0.65);
}

void MainWindow::SelectTarget()
{

}

void MainWindow::UnselectTarget()
{
    TargetUICleanUp();
}

void MainWindow::SelectMyTarget()
{
    UpdateTargetNameLayout();
    UpdateTargetLayout();
}

void MainWindow::UpdateTargetStats()
{
    UpdateTargetLayout();
}

void MainWindow::AddAccountToList(GameLogic *account)
{
    connect(account, &GameLogic::SelectingCharacter, this, &MainWindow::UpdateAccountInfo);
    connect(account, &GameLogic::CharacterSelected, this, &MainWindow::UpdateAccountInfo);
    connect(account, &GameLogic::CharacterActionChanged, this, &MainWindow::UpdateAccountInfo);
    connect(account, &GameLogic::AccountLoggedOut, this, &MainWindow::RemoveAccountFromList);
    connect(account, &GameLogic::CharacterStatsUpdated, this, &MainWindow::UpdateAccountInfo);

    accounts.UpdateTable();
}

void MainWindow::UpdateAccountInfo(DWORD id)
{
    accounts.UpdateRow(id);
}

void MainWindow::RemoveAccountFromList(GameLogic* account)
{
    log.massageList.append(Message("BOT", "Client disconnected."));
    ui->logPTextEdit->appendPlainText(log.GetLastMessage());
    accounts.Remove(account);
    if (account->IsCurrent())
    {
        npcs.clear();
        TargetUICleanUp();
        ChatCleanUp();
        CharacterUICleanUp();
    }
}

void MainWindow::SelectAccount(const QModelIndex index)
{
    currentClientIndex = index;
}

void MainWindow::MakeAccountCurrent(const QModelIndex index)
{
    if (index.data().value<GameLogic*>()->IsCurrent())
        return;

    currentClient->SetIsCurrent(false);
    DisconnectConnections();
    currentClient = nullptr;
    currentClient = index.data().value<GameLogic*>();
    currentClient->SetIsCurrent(true);
    SetupConnections();
    UpdateNameLayout();
    UpdateMainLayout();
    UpdateSubLayout();
    UpdateTargetNameLayout();
    UpdateTargetLayout();
    npcs.addValue(currentClient->GetNPCList());
}

void MainWindow::RaiseAccount()
{
    if (currentClientIndex.row() - 1 >= 0)
    {
        accounts.GetAccountsList()->swapItemsAt(currentClientIndex.row(), currentClientIndex.row() - 1);
        currentClientIndex = ui->accountsTableView->model()->index(currentClientIndex.row() - 1, 0);
        ui->accountsTableView->setCurrentIndex(currentClientIndex);
    }

    accounts.UpdateTable();
}

void MainWindow::LowerAccount()
{
    if (currentClientIndex.row() + 1 <= accounts.GetAccountsList()->count() - 1)
    {
        accounts.GetAccountsList()->swapItemsAt(currentClientIndex.row(), currentClientIndex.row() + 1);
        currentClientIndex = ui->accountsTableView->model()->index(currentClientIndex.row() + 1, 0);
        ui->accountsTableView->setCurrentIndex(currentClientIndex);
    }

    accounts.UpdateTable();
}

