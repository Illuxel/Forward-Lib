#include "fl/utils/converters.hpp"

#include <QDataStream>
#include <QByteArray>

#include <QDateTime>

namespace fl::utils {
	
	QByteArray IntToArray(qint32 source)
	{
		QByteArray temp;
		QDataStream data(&temp, QDataStream::WriteOnly);
		data << source;
		return temp;
	}
	qint32 ArrayToInt(QByteArray source)
	{
		qint32 temp;
		QDataStream data(&source, QDataStream::ReadOnly);
		data >> temp;
		return temp;
	}

	QString CurrentDateTimeStr(const QString& format)
	{
		return QDateTime::currentDateTime().toString(format);
	}
	std::string CurrentDateTimeStr_STD(const std::string& format)
	{
		return QDateTime::currentDateTime().toString(QString::fromStdString(format)).toStdString();
	}
}
