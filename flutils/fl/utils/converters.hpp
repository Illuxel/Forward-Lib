#pragma once

#include <QString>

namespace fl::utils
{
	/**
	 * 
	 */
	template<typename QEnum>
	inline QString EnumToStr(QEnum value) 
	{
		return QVariant::fromValue(value).toString();
	}
	/**
	 * 
	 */
    template<typename QEnum>
	inline QString StrToEnum(QStringView value) 
	{
		return QString();
	}
	/**
	 *	
	 */
	inline QByteArray IntToArray(qint32 source);
	/**
	 *	
	 */
	inline qint32 ArrayToInt(QByteArray source);
	/**
	 * 
	 */
	inline QString CurrentDateTimeStr(const QString& format = "hh:mm:ss dd.MM.yyyy");
	/**
	 *
	 */
	inline std::string CurrentDateTimeStr_STD(const std::string& format = "hh:mm:ss dd.MM.yyyy");
}