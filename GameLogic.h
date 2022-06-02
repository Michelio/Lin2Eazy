#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>

#include "Session.h"
#include "Character.h"
#include "datasource.h"
#include "Chat.h"

enum Status
{
    ACTIVE_BOTTING = 0,
    BACKGROUND_BOTTING = 1,
    LOADING = 2,
    CHARACTER_SELECTION = 3,
    NOT_BOTTING = 4
};

class GameLogic : public QObject
{
    Q_OBJECT

private:
    DWORD clientId;
    bool isCurrent;
    Status status;
    Session *session;
    Character character;
    AliveEntity *target;
    QList<NonPlayerCharacter> npcList;
    Chat *chat;

public:
    GameLogic();
    GameLogic(DWORD);
    ~GameLogic();
    bool IsCurrent();
    void SetupConnections();

    // Getters
    Session *GetSession();
    Status GetStatus();
    Character GetCharacter();
    AliveEntity *GetTarget();
    QList<NonPlayerCharacter> *GetNPCList();
    Chat *GetChat();
    DWORD GetClientId();

    // Setters
    void SetIsCurrent(bool);
    void SetSession(Session*);
    void SetStatus(Status);
    void SetCharacter(Character);
    void SetTarget(AliveEntity*);

    // Operators
    bool operator==(GameLogic&);
    bool operator==(DWORD);

signals:
    void SelectingCharacter(DWORD);
    void CharacterSelected(DWORD);
    void CharacterInfoUpdated(DWORD);
    void CharacterStatsUpdated(DWORD);
    void NPCListInfoChanged();
    void NPCPositionUpdated(uint32_t);
    void MessageReceived(ChatType, Message);
    void NPCListCleared();
    void TargetSelected();
    void TargetUnselected();
    void MyTargetSelected();
    void TargetStatsUpdated();
    void CurrentCharacterStatsChanged(int);
    void EntityStatsUpdated(int);
    void CharacterActionChanged(DWORD);
    void EntityActionChanged(uint32_t);
    void AccountLoggedOut(GameLogic*);



private slots:
    void CharacterSelection();
    void SetupCharacterInfo();
    void UpdateCharInfo(Character);
    void AddNPCToList(NonPlayerCharacter);
    void RemoveNPCFromList(NonPlayerCharacter); // TODO RemoveObject instead
    void UpdateNPCPosition(uint32_t, Coordinates);
    void UpdateNPCPosition(uint32_t, uint32_t);
    void AddMessageToChat(ChatType, Message);
    void ClearNPCList(uint32_t);
    void SelectTarget(uint32_t, uint32_t);
    void UnselectTarget(uint32_t);
    void SelectMyTarget(uint32_t);
    void UpdateEntityStats(uint32_t, QList<QPair<uint32_t, uint32_t>>);
    void EntityAttacked(uint32_t, uint32_t, uint32_t);
    void EntityDied(uint32_t);
    void LogOut();

    void UpdateCharacterPosition(Coordinates);
};

Q_DECLARE_METATYPE(GameLogic*);
#endif // GAMELOGIC_H
