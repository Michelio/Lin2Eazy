#include "datasource.h"

#include <QDir>
#include <QDataStream>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QDebug>

DataSource* DataSource::ds = 0;
DataSourceDestroyer DataSource::dsDestroyer;

DataSource::DataSource(QObject *parent) : QObject(parent)
{
    QDir directory;
    QString absolutePath = directory.absolutePath();

    QFileInfo file(absolutePath + "/L2NPC.dat");
    if (file.exists() && file.isFile())
    {
        QFile npcDataFile(absolutePath + "/L2NPC.dat");
        QByteArray buffer;
        npcDataFile.open(QIODevice::ReadOnly);

        if (npcDataFile.isOpen())
        {
            buffer = npcDataFile.readAll();

            QDataStream stream(buffer);

            deserialize(stream);
            npcDataFile.close();
        }
        // TODO: SEND MESSAGE TO LOG
    }

    else
    {
        directory.setPath("C://Users/Michelio/Documents/GitHub/L2Eazy/NPCDATA");
        QFile npcDataFile(absolutePath + "/L2NPC.dat");
        QStringList files = directory.entryList(QStringList() << "*.xml" << "*.XML", QDir::Files);
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        foreach(QString file, files)
            parseNPCDataFromXML(directory.path() + "/" + file);

        npcDataFile.open(QIODevice::WriteOnly);

        if (npcDataFile.isOpen())
        {
            serialize(stream);
            npcDataFile.write(buffer);
            npcDataFile.close();
        }
        // TODO: SEND MESSAGE TO LOG
    }
    qDebug("DataSource()");
}

DataSource::~DataSource()
{
    qDebug("~DataSource()");
}

void DataSource::serialize(QDataStream &out)
{
    out << npcsDataList.count();

    foreach (NPCData npc, npcsDataList)
        out << npc;
}

void DataSource::deserialize(QDataStream &in)
{
    int count = 0;

    in >> count;

    for (int i = 0; i < count; i++)
    {
        NPCData npc;
        in >> npc;
        npcsDataList.append(npc);
    }
}

DataSource *DataSource::dataSource()
{
    if(!ds)
    {
        ds = new DataSource();
        dsDestroyer.initialize(ds);
    }

    return ds;
}

QList<NPCData> &DataSource::GetNPCDataList()
{
    return npcsDataList;
}

NPCData *DataSource::GetNPCData(uint32_t npcId)
{
    for (int i = 0; i < npcsDataList.count(); i++)
        if (npcsDataList.at(i) == npcId)
            return &npcsDataList[i];

    return nullptr;
}

void DataSource::parseNPCDataFromXML(QString path)
{

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open XML-config: " + path;
        return;
    }

    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement && xml.name() == "npc")
        {
            NPCData npc;
            QXmlStreamAttributes attributes = xml.attributes();
            if (attributes.hasAttribute("id"))
            {
                npc.id = attributes.value("id").toUInt();
                npc.title = attributes.value("title").toString();
                npc.name = attributes.value("name").toString();

                while (true)
                {
                    token = xml.readNext();

                    if (token == QXmlStreamReader::StartElement && xml.name() == "set")
                    {
                        QXmlStreamAttributes attributes = xml.attributes();

                        if (attributes.value("name") == "aggroRange")
                            npc.aggroRange = attributes.value("value").toUInt();

                        else if (attributes.value("name") == "baseDEX")
                            npc.baseDEX = attributes.value("value").toUInt();

                        else if (attributes.value("name") == "baseCON")
                            npc.baseCON = attributes.value("value").toUInt();

                        else if (attributes.value("name") == "baseMEN")
                            npc.baseMEN = attributes.value("value").toUInt();

                        else if (attributes.value("name") == "baseHpMax")
                            npc.baseHpMax = attributes.value("value").toFloat();

                        else if (attributes.value("name") == "baseHpRate")
                            npc.baseHpRate = attributes.value("value").toFloat();

                        else if (attributes.value("name") == "baseMpMax")
                            npc.baseMpMax = attributes.value("value").toFloat();

                        else if (attributes.value("name") == "baseRunSpd")
                            npc.baseSpeed = attributes.value("value").toUInt();

                        else if (attributes.value("name") == "level")
                        {
                            npc.level = attributes.value("value").toUInt();
                            break;
                        }
                    }
                }

                npcsDataList.append(npc);
            }
        }
    }

    file.close();
}

DataSourceDestroyer::~DataSourceDestroyer()
{
    qDebug("~DataSourceDestroyer()");
    delete ds;
}

void DataSourceDestroyer::initialize(DataSource *ds)
{
    this->ds = ds;
}

QDataStream &operator<<(QDataStream& out, const NPCData npc)
{
    out << npc.title;
    out << npc.name;
    out << npc.id;
    out << npc.aggroRange;
    out << npc.baseDEX;
    out << npc.baseCON;
    out << npc.baseMEN;
    out << npc.baseHpMax;
    out << npc.baseHpRate;
    out << npc.baseMpMax;
    out << npc.baseSpeed;
    out << npc.level;

    return out;
}

QDataStream &operator>>(QDataStream& in, NPCData& npc)
{
    in >> npc.title;
    in >> npc.name;
    in >> npc.id;
    in >> npc.aggroRange;
    in >> npc.baseDEX;
    in >> npc.baseCON;
    in >> npc.baseMEN;
    in >> npc.baseHpMax;
    in >> npc.baseHpRate;
    in >> npc.baseMpMax;
    in >> npc.baseSpeed;
    in >> npc.level;

    return in;
}

bool NPCData::operator==(const NPCData &npcData) const
{
    return id == npcData.id;
}

bool NPCData::operator==(uint32_t npcId) const
{
    return id == npcId;
}
