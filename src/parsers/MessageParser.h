#pragma once

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

namespace server::Parser
{
    Q_NAMESPACE
    
    class ServerParser
        : public QObject
    {
        Q_OBJECT;

        explicit ServerParser(QObject* parent = nullptr);

    public:
        /**
		 *  @return instance of the server parser
         */
        static ServerParser* Instance();
		/// <summary><c>ServerParser</c> is a method in the <c>ServerParser</c> class.
        /// </summary>
        static void Init();
    
        static QJsonObject ServerSettings(const QString& componentName);

        ServerParser(const ServerParser&) = delete;
		void operator=(const ServerParser&) = delete;
		~ServerParser() override;

    private:
        QJsonDocument readJsonFile(const QString& file) const;
            
    public slots:
        static QJsonObject ParseIcomingCmd(const QByteArray& row);
   
    private:
        QJsonObject m_ConfigFile;
    };
}