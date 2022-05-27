#ifndef ACCOUNTSDELEGATE_H
#define ACCOUNTSDELEGATE_H

#include <QStyledItemDelegate>
#include <QProgressBar>

class AccountsDelegate : public QStyledItemDelegate
{
private:
    QProgressBar cpBarStyled, hpBarStyled, mpBarStyled;

public:
    AccountsDelegate();
    void paint(QPainter*, const QStyleOptionViewItem&, const QModelIndex&) const override;
    void setupBar(QStyleOptionProgressBar&, int, int, int, QRect) const;
    void setupStyledBars();
};

#endif // ACCOUNTSDELEGATE_H
