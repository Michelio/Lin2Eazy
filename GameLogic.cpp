#include "GameLogic.h"
#include "datasource.h"

GameLogic::GameLogic()
{
    qDebug("GameLogic()");
    isCurrent = false;
    status = LOADING;
    clientId = 0;
    target = nullptr;
    session = nullptr;
    chat = new Chat();
}

GameLogic::GameLogic(DWORD id)
{
    qDebug("GameLogic(DWORD)");
    isCurrent = false;
    status = LOADING;
    clientId = id;
    target = nullptr;
    session = nullptr;
    chat = new Chat();
}

GameLogic::~GameLogic()
{
    qDebug("~GameLogic()");
    delete target;
    delete session;
    delete chat;
}

bool GameLogic::IsCurrent()
{
    return isCurrent;
}

void GameLogic::SetupConnections()
{
    connect(session->GetParser(), &PacketParser::SelectCharacter, this, &GameLogic::CharacterSelection);
    connect(session->GetParser(), &PacketParser::CharacterSelected, this, &GameLogic::SetupCharacterInfo);
    connect(session->GetParser(), &PacketParser::UserInfo, this, &GameLogic::UpdateCharInfo);
    connect(session->GetParser(), &PacketParser::NPCInfo, this, &GameLogic::AddNPCToList);
    connect(session->GetParser(), SIGNAL(ObjectMoved(uint32_t,Coordinates)), this, SLOT(UpdateNPCPosition(uint32_t,Coordinates)));
    connect(session->GetParser(), SIGNAL(MoveToPawn(uint32_t,uint32_t)), this, SLOT(UpdateNPCPosition(uint32_t,uint32_t)));
    connect(session->GetParser(), SIGNAL(MessageReceived(ChatType,Message)), this, SLOT(AddMessageToChat(ChatType,Message)));
    connect(session->GetParser(), &PacketParser::RemoveNPC, this, &GameLogic::RemoveNPCFromList);
    connect(session->GetParser(), &PacketParser::CharacterTeleported, this, &GameLogic::ClearNPCList);
    connect(session->GetParser(), &PacketParser::MyTargetSelected, this, &GameLogic::SelectMyTarget);
    connect(session->GetParser(), &PacketParser::TargetUnselected, this, &GameLogic::UnselectTarget);
    connect(session->GetParser(), &PacketParser::StatusUpdate, this, &GameLogic::UpdateEntityStats);
    connect(session->GetParser(), &PacketParser::EntityAttack, this, &GameLogic::EntityAttacked);
    connect(session->GetParser(), &PacketParser::KillEntity, this, &GameLogic::EntityDied);
    connect(session->GetParser(), &PacketParser::LoggedOut, this, &GameLogic::LogOut);
    connect(session->GetParser(), &PacketParser::PositionValidated, this, &GameLogic::UpdateCharacterPosition);
}

Session* GameLogic::GetSession()
{
    return session;
}

Status GameLogic::GetStatus()
{
    return status;
}

Character GameLogic::GetCharacter()
{
    return character;
}

AliveEntity *GameLogic::GetTarget()
{
    return target;
}

QList<NonPlayerCharacter> *GameLogic::GetNPCList()
{
    return &npcList;
}

Chat *GameLogic::GetChat()
{
    return chat;
}

DWORD GameLogic::GetClientId()
{
    return clientId;
}

void GameLogic::SetIsCurrent(bool def)
{
    isCurrent = def;
}

void GameLogic::SetSession(Session *session)
{
    this->session = session;
}

void GameLogic::SetStatus(Status status)
{
    this->status = status;
}

void GameLogic::SetCharacter(Character character)
{
    this->character = character;
}

void GameLogic::SetTarget(AliveEntity *target)
{
    this->target = target;
}

bool GameLogic::operator==(GameLogic &client)
{
    return clientId == client.clientId;
}

bool GameLogic::operator==(DWORD id)
{
    return clientId == id;
}

void GameLogic::CharacterSelection()
{
    status = CHARACTER_SELECTION;
    emit SelectingCharacter(clientId);
}

void GameLogic::SetupCharacterInfo()
{
    status = NOT_BOTTING;
    emit CharacterSelected(clientId);
}

void GameLogic::UpdateCharInfo(Character character)
{
    character.CalculateSpeed(character.GetStats().DEX, 1, 0);
    this->character = character;

    if (this->character.GetCurHp() == 0)
        this->character.SetActionType(DEAD);

    if (isCurrent)
        emit CharacterInfoUpdated(clientId);
}

void GameLogic::AddNPCToList(NonPlayerCharacter npc)
{
    if (npcList.contains(npc))
        return;

    DataSource *s = DataSource::dataSource();
    NPCData data;
    data.id = npc.GetNPCId();
    if (s->GetNPCDataList().contains(data))
    {
        data = *s->GetNPCData(npc.GetNPCId());
        npc = data;
    }

    if (npc.GetAttackable())
        npc.SetEntityType(ATTACKABLE_NPC);

    else npc.SetEntityType(NPC);

    npc.CalculateDistance(character.GetPosition());
    npcList.append(npc);
    std::sort(npcList.begin(), npcList.end());

    if (isCurrent)
        emit NPCListInfoChanged();
}

void GameLogic::RemoveNPCFromList(NonPlayerCharacter npc)
{
    npcList.removeOne(npc);

    if (isCurrent)
        emit NPCListInfoChanged();
}

void GameLogic::UpdateNPCPosition(uint32_t objectId, Coordinates destPosition)
{
    if (character == objectId)
        return;

    for (int i = 0; i < npcList.count(); i++)
        if ((Entity)npcList.at(i) == objectId)
        {
            npcList[i].SetPosition(destPosition);
            npcList[i].CalculateDistance(character.GetPosition());
            std::sort(npcList.begin(), npcList.end());

            emit NPCListInfoChanged();
        }
}

void GameLogic::UpdateNPCPosition(uint32_t objectId, uint32_t targetObjectId)
{
    if (character == objectId)
        return;

    for (int i = 0; i < npcList.count(); i++)
        if ((Entity)npcList.at(i) == objectId)
        {
            if (character == targetObjectId)
            {
                npcList[i].SetPosition(character.GetPosition());
                npcList[i].CalculateDistance(character.GetPosition());
            }

            std::sort(npcList.begin(), npcList.end());

            emit NPCListInfoChanged();
        }
}

void GameLogic::AddMessageToChat(ChatType chatType, Message message)
{
    chat->GetTab(chatType)->massageList.append(message);

    if (isCurrent)
        emit MessageReceived(chatType, message);
}

void GameLogic::ClearNPCList(uint32_t charId)
{
    if (character == charId)
    {
        npcList.clear();

        if (isCurrent)
            emit NPCListInfoChanged();
    }
}

void GameLogic::SelectTarget(uint32_t /*objectId*/, uint32_t /*targetObjId*/)
{
    //if (objectId != activeCharacter.GetObjectId())  // TODO
}

void GameLogic::UnselectTarget(uint32_t objectId)
{
    if (character == objectId)
    {
        target = nullptr;

        emit TargetUnselected();
    }
}

void GameLogic::SelectMyTarget(uint32_t targetObjectId)
{
    for (int i = 0; i < npcList.count(); i++)
        if ((Entity)npcList.at(i) == targetObjectId)
        {
            npcList[i].SetIsTarget(true);
            target = dynamic_cast<AliveEntity*>(&npcList[i]);

            emit MyTargetSelected();
        }
}

void GameLogic::UpdateEntityStats(uint32_t objectId, QList<QPair<uint32_t, uint32_t>> stats)
{
    if (character == objectId)
    {
        int priority = character.UpdateStats(stats);
        if (isCurrent)
            emit CurrentCharacterStatsChanged(priority);

        emit CharacterStatsUpdated(clientId);
    }

    else
        for (int i = 0; i < npcList.count(); i++)
            if ((Entity)npcList.at(i) == objectId)
            {
                npcList[i].UpdateStats(stats);

                if (isCurrent)
                {
                    if (npcList[i].IsTarget())
                        emit TargetStatsUpdated();

                    emit EntityStatsUpdated(i);
                }
            }

}

void GameLogic::EntityAttacked(uint32_t entityId, uint32_t targetObjectId, uint32_t damage)
{
    if (character != entityId)
        for (int i = 0; i < npcList.count(); i++)
            if ((Entity)npcList.at(i) == targetObjectId)
            {
                uint32_t curHp = 0;
                if (npcList[i].IsChampion())
                    curHp = npcList[i].GetCurHp() - damage / 3;

                else curHp = npcList[i].GetCurHp() - damage;

                npcList[i].SetCurHp(curHp);

                if (isCurrent)
                {
                    if (npcList[i].IsTarget())
                        emit TargetStatsUpdated();

                    emit EntityStatsUpdated(i);
                }
            }
}

void GameLogic::EntityDied(uint32_t entityId)
{
    if (character == entityId)
    {
        character.SetActionType(DEAD);

        emit CharacterActionChanged(clientId);
    }

    else
    {
        emit EntityActionChanged(entityId);
    }

}

void GameLogic::LogOut()
{
    emit AccountLoggedOut(this);
}

void GameLogic::UpdateCharacterPosition(Coordinates currentPosition)
{
    character.SetPosition(currentPosition);

    for (int i = 0; i < npcList.count(); i++)
        npcList[i].CalculateDistance(currentPosition);

    std::sort(npcList.begin(), npcList.end());

    emit NPCListInfoChanged();
}
