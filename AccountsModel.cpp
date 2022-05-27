#include "AccountsModel.h"

#include <QSize>

AccountsModel::AccountsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    accountsList = new QList<GameLogic*>();
}

AccountsModel::~AccountsModel()
{
    qDebug("~AccountsModel()");
    //delete accountsList;
}

int AccountsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    if (accountsList)
        return accountsList->count();

    else return 0;
}

int AccountsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant AccountsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return QString("Account");
        case 1:
            return QString("Stats");
        }
    }

    return QVariant();
}

void AccountsModel::AddValue(QList<GameLogic *> *accountsList)
{
    this->accountsList = accountsList;

    emit layoutChanged();
}

QVariant AccountsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    GameLogic *client = accountsList->at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return QVariant::fromValue(client);
    default:
        return QVariant();
    }
}

void AccountsModel::UpdateRow(DWORD id)
{
    QModelIndex startOfRow;
    QModelIndex endOfRow;
    for (int i = 0; i < accountsList->count(); i++)
        if(accountsList->at(i)->GetClientId() == id)
        {
            startOfRow = this->index(i, 0);
            endOfRow   = this->index(i, 1);
            break;
        }

    emit dataChanged(startOfRow, endOfRow);
}

void AccountsModel::UpdateRow(DWORD id, int column)
{
    QModelIndex startOfRow;
    QModelIndex endOfRow;

    for (int i = 0; i < accountsList->count(); i++)
        if(accountsList->at(i)->GetClientId() == id)
        {
            startOfRow = this->index(i, column);
            endOfRow   = this->index(i, column);
            break;
        }

    emit dataChanged(startOfRow, endOfRow);
}

void AccountsModel::UpdateTable()
{
    emit layoutChanged();
}

void AccountsModel::Remove(GameLogic *account)
{
    accountsList->removeOne(account);

    emit layoutChanged();
}

void AccountsModel::Remove(DWORD id)
{
    for (int i = 0; i < accountsList->count(); i++)
        if(accountsList->at(i)->GetClientId() == id)
        {
            accountsList->removeAt(i);
            emit layoutChanged();
            break;
        }
}

QList<GameLogic *> *AccountsModel::GetAccountsList()
{
    return accountsList;
}
