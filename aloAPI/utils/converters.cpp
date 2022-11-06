#include "converters.h"

#include <QByteArray>
#include <QIODevice>

#include <QDateTime>

#include <QString>
#include <QStringView>

#include <QVariant>

namespace alo::Utils
{
	QByteArray IntToArray(qint32 source)
	{
		QByteArray temp;
		QDataStream data(&temp, QIODevice::WriteOnly);
		data << source;
		return temp;
	}
	qint32 ArrayToInt(QByteArray source)
	{
		qint32 temp;
		QDataStream data(&source, QIODevice::ReadOnly);
		data >> temp;
		return temp;
	}

	QString CurrentDateTimeStr(const QString& format)
	{
		return QDateTime::currentDateTime().toString(format);
	}
}
