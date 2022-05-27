#ifndef NPCDELEGATE_H
#define NPCDELEGATE_H

#include <QStyledItemDelegate>
#include <QProgressBar>

class NPCDelegate : public QStyledItemDelegate
{
private:
    QProgressBar hpBarStyled;
public:
    NPCDelegate();
    void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const override;
    void setupBar(QStyleOptionProgressBar&, int, int, int, QRect) const;
    void setupStyledBar();
};

#endif // NPCDELEGATE_H
