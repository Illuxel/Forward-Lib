#include "ServerParser.h"

#include "utils/utils.h"

#include <QFile>

namespace server::Parser
{
    ServerParser::ServerParser(QObject* parent)
        : QObject(parent)
    {
        setObjectName("Parser");
    }
    ServerParser::~ServerParser()
    {

    }

    ServerParser* ServerParser::s_instance{ nullptr };
    ServerParser* ServerParser::Instance()
    {
        if (!s_instance) s_instance = new ServerParser();
        return s_instance;
    }

    void ServerParser::Init()
    {
        ServerParser* parser = Instance();

        sInfo("Reading config file...", parser);

        parser->m_ConfigFile = parser->readJsonFile("config/cfg.txt").object();
    }

    QJsonObject ServerParser::ParseIcomingCmd(const QByteArray& row)
    {
       return {};
    }

	QJsonObject ServerParser::ServerSettings(const QString& compName)
	{
        ServerParser* parser = Instance();
        return parser->m_ConfigFile[compName].toObject();
	}

    QJsonDocument ServerParser::readJsonFile(const QString& filePath) const
	{
		QFile file(filePath);
        if (!file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text))
            sError("Can not read the file from " + filePath, Instance());

        return QJsonDocument::fromJson(file.readAll());
	}
}