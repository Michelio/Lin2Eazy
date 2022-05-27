#ifndef NPCMODEL_H
#define NPCMODEL_H

#include <QAbstractTableModel>

#include "NonPlayerCharacter.h"

class NPCModel : public QAbstractTableModel
{
private:
    QList<NonPlayerCharacter> *npcList;

public:
    explicit NPCModel(QObject *parent = nullptr);
    ~NPCModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int , Qt::Orientation, int) const override;
    void addValue(QList<NonPlayerCharacter>*);
    void updateRow(uint32_t);
    void updateTable();
    void clear();

    QList<NonPlayerCharacter> *GetNPCList();
};

#endif // NPCMODEL_H
