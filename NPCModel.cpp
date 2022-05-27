#include "NPCModel.h"

#include <QSize>

NPCModel::NPCModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    npcList = new QList<NonPlayerCharacter>();
}

NPCModel::~NPCModel()
{
    qDebug("~NPCModel()");
    delete npcList;
}

int NPCModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    if (npcList)
        return npcList->count();

    else return 0;
}

int NPCModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 5;
}

QVariant NPCModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return QString("");
        case 1:
            return QString("Name");
        case 2:
            return QString("Id");
        case 3:
            return QString("Dist");
        case 4:
            return QString("Stats");
        }
    }

    return QVariant();
}

void NPCModel::addValue(QList<NonPlayerCharacter> *npcList)
{
    this->npcList = npcList;

    emit layoutChanged();
}

QVariant NPCModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    NonPlayerCharacter mNpc = npcList->at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return QVariant::fromValue(mNpc);
    default:
        return QVariant();
    }
}

void NPCModel::updateRow(uint32_t objectId)
{
    QModelIndex startOfRow;
    QModelIndex endOfRow;
    for (int i = 0; i < npcList->count(); i++)
        if((Entity)npcList->at(i) == objectId)
        {
            startOfRow = this->index(i, 0);
            endOfRow   = this->index(i, 1);
            break;
        }

    emit dataChanged(startOfRow, endOfRow);
}

void NPCModel::updateTable()
{
    emit layoutChanged();
}

void NPCModel::clear()
{
    npcList->clear();
    npcList = nullptr;
    emit layoutChanged();
}

QList<NonPlayerCharacter> *NPCModel::GetNPCList()
{
    return npcList;
}
