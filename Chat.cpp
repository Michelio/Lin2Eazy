#include "Chat.h"

Chat::Chat(QObject *parent)
    : QObject{parent}
{
    tab[0].type = GENERAL;
    tab[1].type = SHOUT;
    tab[2].type = WHISPER;
    tab[3].type = PARTY;
    tab[4].type = CLAN;
    tab[5].type = TRADE;
    tab[6].type = ALLIANCE;
    tab[7].type = ANNOUNCEMENT;
}

ChatTab* Chat::GetTab(ChatType type)
{
    switch (type)
    {
        case GENERAL:
            return tab;

        case SHOUT:
            return tab + 1;

        case WHISPER:
            return tab + 2;

        case PARTY:
            return tab + 3;

        case CLAN:
            return tab + 4;

        case TRADE:
            return tab + 5;

        case ALLIANCE:
            return tab + 6;

        case ANNOUNCEMENT:
            return tab + 7;

        case LOG:
            return tab + 8;
    }
}


Message::Message()
{
    time = time.currentTime();
    owner = "Bot";
    text = "";
}

Message::Message(QString owner, QString text)
{
    time = time.currentTime();
    this->owner = owner;
    this->text = text;
}

ChatTab::ChatTab()
{
    type = LOG;
}

ChatTab::ChatTab(ChatType type, QList<Message> messages)
{
    this->type = type;
    this->massageList = messages;
}

QString ChatTab::GetLastMessage()
{
    return massageList.at(massageList.size() - 1).time.toString() + "   "
           + massageList.at(massageList.size() - 1).owner + ": "
           + massageList.at(massageList.size() - 1).text;
}
