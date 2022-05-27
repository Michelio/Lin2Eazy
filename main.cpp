#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "BotLogic.h"

QString readTextFile(QString path)
{
    QFile file(path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        return in.readAll();
    }
    return "";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString css = readTextFile(":/Styles/style.css");

    if (css.length() > 0)
        a.setStyleSheet(css);

    qRegisterMetaType<DWORD>("DWORD");
    qRegisterMetaType<NonPlayerCharacter>("NonPlayerCharacter");
    qRegisterMetaType<Character>("Character");
    qRegisterMetaType<Coordinates>("Coordinates");
    qRegisterMetaType<uint32_t>("uint32_t");
    qRegisterMetaType<QList<QPair<uint32_t,uint32_t>>>("QList<QPair<uint32_t,uint32_t>>");
    qRegisterMetaType<HANDLE>("HANDLE");
    qRegisterMetaType<GameLogic*>("GameLogic*");

    BotLogic bl;
    bl.start();

    MainWindow w;
    w.show();
    w.SetupConnections(&bl);

    return a.exec();
}
