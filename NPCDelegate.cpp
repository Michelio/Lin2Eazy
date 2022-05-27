#include "NPCDelegate.h"

#include <QApplication>
#include <QStyleOptionProgressBar>
#include <QPainter>

#include"NonPlayerCharacter.h"

NPCDelegate::NPCDelegate()
{
    setupStyledBar();
}

void NPCDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NonPlayerCharacter npc = index.data().value<NonPlayerCharacter>();
    int height = option.rect.height(), width = option.rect.width();
    int x = option.rect.x(), y = option.rect.y();

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.light());

    painter->setFont(QFont("Times New Roman", 11, QFont::Normal));

    if (index.column() == 0)
    {
        QPixmap *pic;
        if (npc.GetEntityType() == ATTACKABLE_NPC)
            pic = new QPixmap(":/Icon/red-circle-40px.png");
        else pic = new QPixmap(":/Icon/green-circle-40px.png");
        QRect statusRec(x + 1, y + 1, height - 2, height - 2);
        QApplication::style()->drawItemPixmap(painter, statusRec, Qt::AlignCenter,
                                              pic->scaled(statusRec.height(), statusRec.height(), Qt::KeepAspectRatio));
    }

    else if (index.column() == 1)
    {
        QRect nameRec(x + 2, y + 3, width - 2, height - 2);
        QString name = painter->fontMetrics().elidedText(npc.GetName(), Qt::ElideRight, nameRec.width());
        QApplication::style()->drawItemText(painter, nameRec, Qt::AlignLeft, option.palette, true, name);
    }

    else if (index.column() == 2)
    {
        QRect idRec(x + 2, y + 3, width - 2, height - 2);
        QString id = painter->fontMetrics().elidedText(QString::number(npc.GetNPCId()), Qt::ElideRight, idRec.width());
        QApplication::style()->drawItemText(painter, idRec, Qt::AlignLeft, option.palette, true, id);
    }

    else if (index.column() == 3)
    {
        QRect distanceRec(x + 2, y + 3, width - 2, height - 2);
        QString distance = painter->fontMetrics().elidedText(QString::number(npc.GetPosition().distanceToCharacter), Qt::ElideRight, distanceRec.width());
        QApplication::style()->drawItemText(painter, distanceRec, Qt::AlignLeft, option.palette, true, distance);
    }

    else
    {
        QStyleOptionProgressBar hpBar;
        QRect hpBarRec(x + 2, y + 4, width - 2, height - 8);
        this->setupBar(hpBar, 0, npc.GetMaxHp(), npc.GetCurHp(), hpBarRec);
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &hpBar, painter, &hpBarStyled);
    }
}

void NPCDelegate::setupBar(QStyleOptionProgressBar &bar, int min, int max, int val, QRect rect) const
{
    bar.minimum = min;
    bar.maximum = max;
    bar.rect = rect;
    bar.progress = val;
}

void NPCDelegate::setupStyledBar()
{

    hpBarStyled.setStyleSheet("QProgressBar { border: 1px solid rgb(230, 78, 81);"
                              "border-radius: 5px;"
                              "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,"
                              " stop: 0 rgb(166, 53, 55), stop: 1 rgb(230, 78, 81));}"
                              "QProgressBar::chunk {"
                              "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5,"
                              " stop: 0 rgb(212, 34, 34), stop: 1 rgb(250, 42, 42));}");
}
