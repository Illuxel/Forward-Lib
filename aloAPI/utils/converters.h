#pragma once

#include "aloAPI/AloAPI_global.h"

#include <QVariant>

namespace alo::Utils
{
	/**
	 * 
	 */
	template<typename QEnum>
	ALO_API QString 
	EnumToStr(QEnum value)
	{
		return QVariant::fromValue(value).toString();
	}
	/**
	 * 
	 */
    template<typename QEnum>
	ALO_API QString 
	StrToEnum(QStringView value)
	{
		return QString();
	}
	/**
	 *	
	 */
	ALO_API QByteArray 
	IntToArray(qint32 source);
	/**
	 *	
	 */
	ALO_API qint32 
	ArrayToInt(QByteArray source);
	/**
	 * 
	 */
	ALO_API QString 
	CurrentDateTimeStr(const QString& format = "hh:mm:ss dd.MM.yyyy");
}
