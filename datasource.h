#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QFile>

struct NPCData
{
    QString title;
    QString name;
    uint32_t id;
    uint32_t aggroRange;
    uint32_t baseDEX;
    uint32_t baseCON;
    uint32_t baseMEN;
    uint32_t baseHpMax;
    uint32_t baseHpRate;
    uint32_t baseMpMax;
    uint32_t baseSpeed;
    uint32_t level;

    // Operators
    friend QDataStream &operator<<(QDataStream&, const NPCData);
    friend QDataStream &operator>>(QDataStream&, NPCData&);
    bool operator==(const NPCData&) const;
    bool operator==(uint32_t) const;
};

class DataSource;

class DataSourceDestroyer
{
    private:
        DataSource *ds;

    public:
        ~DataSourceDestroyer();
        void initialize(DataSource*);
};

class DataSource : public QObject
{
    Q_OBJECT

private:
    QList<NPCData> npcsDataList;
    static DataSource* ds;
    static DataSourceDestroyer dsDestroyer;

protected:
    explicit DataSource(QObject *parent = nullptr);
    DataSource(const DataSource&);
    ~DataSource();
    DataSource& operator=(DataSource&);

    friend class DataSourceDestroyer;

    void serialize(QDataStream&);
    void deserialize(QDataStream&);

public:
    static DataSource *dataSource();
    QList<NPCData> &GetNPCDataList();
    NPCData* GetNPCData(uint32_t);
    void parseNPCDataFromXML(QString);

};

#endif // DATASOURCE_H
