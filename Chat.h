#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QString>
#include <QList>
#include <QTime>

enum ChatType
{
    GENERAL = 0,
    SHOUT = 1,
    WHISPER = 2,
    PARTY = 3,
    CLAN = 4,
    TRADE = 8,
    ALLIANCE = 9,
    ANNOUNCEMENT = 10,
    LOG = 11
};

struct Message
{
    QTime time;
    QString owner;
    QString text;

    Message();
    Message(QString, QString);
};

struct ChatTab
{
    ChatType type;
    QList<Message> massageList;

    ChatTab();
    ChatTab(ChatType, QList<Message>);

    QString GetLastMessage();
};

class Chat : public QObject
{
    Q_OBJECT

private:
    ChatTab tab[8];

public:
    explicit Chat(QObject *parent = nullptr);
    ChatTab* GetTab(ChatType);

    // Operator

signals:

};

#endif // CHAT_H
