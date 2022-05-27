#include "AccountsDelegate.h"

#include <QApplication>
#include <QStyleOptionProgressBar>
#include <QPainter>

#include "GameLogic.h"

AccountsDelegate::AccountsDelegate()
{
    setupStyledBars();
}

void AccountsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    GameLogic *client = index.data().value<GameLogic*>();
    int height = option.rect.height(),
        width = option.rect.width();
    int x = option.rect.x(),
        y = option.rect.y();

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    if (index.column() == 0)
    {
        QPixmap *pic;
        QRect statusRec(x + 1, y + 1, height - 2, height - 2);
        int leftWidth = width - statusRec.width() - 7;
        QRect nameRec(statusRec.x() + statusRec.width() + 4, y + 2, leftWidth - height - 3, height - 2);
        QString name = "";

        painter->setFont(QFont("Times New Roman", 12, QFont::Normal));

        if (option.state & QStyle::State_Selected)
            painter->setPen(Qt::white);

        else painter->setPen(Qt::black);

        switch (client->GetStatus())
        {
        case ACTIVE_BOTTING:
        case BACKGROUND_BOTTING:
        {
            QRect levelRec(nameRec.x() + nameRec.width() + 2, y + 2, height - 2, height - 2);

            switch (client->GetCharacter().GetActionType())
            {
            case SITTING:
            case STANDING:
            case RUNNING:
                pic = new QPixmap(":/Icon/green-circle-40px.png");
                break;
            case CASTING:
            case ATTACKING:
                pic = new QPixmap(":/Icon/crossed-swords-30px.png");
                break;
            case DEAD:
                pic = new QPixmap(":/Icon/skull-30px.png");
                break;
            }

            name = painter->fontMetrics().elidedText(client->GetCharacter().GetName(), Qt::ElideRight, nameRec.width());

            QApplication::style()->drawItemText(painter, levelRec, Qt::AlignRight, option.palette, true,
                                                QString::number(client->GetCharacter().GetLevel()));
            break;
        }
        case LOADING:
        case CHARACTER_SELECTION:
            name = painter->fontMetrics().elidedText("LOADING...", Qt::ElideRight, nameRec.width());
            pic = new QPixmap(":/Icon/gray-circle-40px.png");
            break;
        case NOT_BOTTING:
            QRect levelRec(nameRec.x() + nameRec.width() + 2, y + 2, height - 2, height - 2);
            name = painter->fontMetrics().elidedText(client->GetCharacter().GetName(), Qt::ElideRight, nameRec.width());
            QApplication::style()->drawItemText(painter, levelRec, Qt::AlignRight, option.palette, true,
                                                QString::number(client->GetCharacter().GetLevel()));

            switch (client->GetCharacter().GetActionType())
            {
            case SITTING:
            case STANDING:
            case RUNNING:
                pic = new QPixmap(":/Icon/orange-circle-40px.png");
                break;
            case CASTING:
            case ATTACKING:
                pic = new QPixmap(":/Icon/crossed-swords-30px.png");
                break;
            case DEAD:
                pic = new QPixmap(":/Icon/skull-30px.png");
                break;
            }
        }

        QApplication::style()->drawItemPixmap(painter, statusRec, Qt::AlignCenter,
                                              pic->scaled(statusRec.height(), statusRec.height(), Qt::KeepAspectRatio));
        QApplication::style()->drawItemText(painter, nameRec, Qt::AlignLeft, option.palette, true, name);
    }

    else
    {
        QStyleOptionProgressBar cpBar, hpBar, mpBar;

        int leftWidth = width - 12;
        QRect cpBarRec(x + 3, y + 4, leftWidth/3, height - 8);
        QRect hpBarRec(cpBarRec.x() + cpBarRec.width() + 3, y + 4, leftWidth/3, height - 8);
        QRect mpBarRec(hpBarRec.x() + hpBarRec.width() + 3, y + 4, leftWidth/3, height - 8);

        switch (client->GetStatus())
        {
        case ACTIVE_BOTTING:
        case BACKGROUND_BOTTING:
        case NOT_BOTTING:
            this->setupBar(cpBar, 0, client->GetCharacter().GetMaxCp(), client->GetCharacter().GetCurCp(), cpBarRec);
            this->setupBar(hpBar, 0, client->GetCharacter().GetMaxHp(), client->GetCharacter().GetCurHp(), hpBarRec);
            this->setupBar(mpBar, 0, client->GetCharacter().GetMaxMp(), client->GetCharacter().GetCurMp(), mpBarRec);
            break;
        case LOADING:
        case CHARACTER_SELECTION:
            this->setupBar(cpBar, 0, 100, 1, cpBarRec);
            this->setupBar(hpBar, 0, 100, 1, hpBarRec);
            this->setupBar(mpBar, 0, 100, 1, mpBarRec);
        }

        switch (client->GetCharacter().GetActionType())
        {
        case SITTING:
        case STANDING:
        case RUNNING:
        case CASTING:
        case ATTACKING:
            QApplication::style()->drawControl(QStyle::CE_ProgressBar, &cpBar, painter, &cpBarStyled);
            QApplication::style()->drawControl(QStyle::CE_ProgressBar, &hpBar, painter, &hpBarStyled);
            QApplication::style()->drawControl(QStyle::CE_ProgressBar, &mpBar, painter, &mpBarStyled);
            break;
        case DEAD:
            painter->save();
            painter->setPen(Qt::red);
            painter->setFont(QFont("Times New Roman", 12, QFont::Normal));
            QApplication::style()->drawItemText(painter, QRect(x + 4, y + 2, width, height - 4),
                                                Qt::AlignLeft, option.palette, true, "DEAD");
            painter->restore();
            break;
        }
    }
}

void AccountsDelegate::setupBar(QStyleOptionProgressBar &bar, int min, int max, int val, QRect rect) const
{
    bar.minimum = min;
    bar.maximum = max;
    bar.rect = rect;
    bar.progress = val;
}

void AccountsDelegate::setupStyledBars()
{
    cpBarStyled.setStyleSheet("QProgressBar { border: 1px solid rgb(219, 199, 44);"
                              "border-radius: 5px;"
                              "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,"
                              " stop: 0 rgb(168, 153, 35), stop: 1 rgb(219, 199, 44));}"
                              "QProgressBar::chunk {"
                              "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,"
                              " stop: 0 rgb(207, 196, 43), stop: 1 rgb(255, 242, 52));}");

    hpBarStyled.setStyleSheet("QProgressBar { border: 1px solid rgb(230, 78, 81);"
                              "border-radius: 5px;"
                              "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,"
                              " stop: 0 rgb(166, 53, 55), stop: 1 rgb(230, 78, 81));}"
                              "QProgressBar::chunk {"
                              "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,"
                              " stop: 0 rgb(212, 34, 34), stop: 1 rgb(250, 42, 42));}");

    mpBarStyled.setStyleSheet("QProgressBar { border: 1px solid rgb(79, 58, 199);"
                              "border-radius: 5px;"
                              "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,"
                              " stop: 0 rgb(51, 38, 130), stop: 1 rgb(79, 58, 199));}"
                              "QProgressBar::chunk {"
                              "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,"
                              " stop: 0 rgb(70, 73, 219), stop: 1 rgb(82, 85, 255));}");
}
