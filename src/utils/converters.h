#pragma once

#include <QString>
#include <QVariant>

#include "aloAPI_global.h"

class QStringView;
class QByteArray;

namespace alo::Utils
{
	/**
	 * 
	 */
	template<typename QEnum>
	QString EnumToStr(QEnum value) {
		return QString();
	}
	/**
	 * 
	 */
    template<typename QEnum>
	QString StrToEnum(QStringView value) {
		return QString();
	}
	/**
	 *	
	 */
	QByteArray ALO_API
	IntToArray(qint32 source);
	/**
	 *	
	 */
	qint32 ALO_API
	ArrayToInt(QByteArray source);
	/**
	 * 
	 */
	QString ALO_API
	CurrentDateTimeStr(const QString& format = "hh:mm:ss dd.MM.yyyy");
	std::string ALO_API
	CurrentDateTimeStr_STD(const std::string& format = "hh:mm:ss dd.MM.yyyy");
}
