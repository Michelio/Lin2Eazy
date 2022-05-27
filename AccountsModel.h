#ifndef ACCOUNTSMODEL_H
#define ACCOUNTSMODEL_H

#include <QAbstractTableModel>
#include <QObject>

#include "Character.h"
#include "GameLogic.h"

class AccountsModel : public QAbstractTableModel
{
private:
    QList<GameLogic*> *accountsList;

public:
    explicit AccountsModel(QObject *parent = nullptr);
    ~AccountsModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int , Qt::Orientation, int) const override;
    void AddValue(QList<GameLogic*>*);
    void UpdateRow(DWORD);
    void UpdateRow(DWORD, int);
    void UpdateTable();
    void Remove(GameLogic*);
    void Remove(DWORD);

    QList<GameLogic *> *GetAccountsList();
};

#endif // ACCOUNTSMODEL_H
