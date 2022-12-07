#pragma once

#include "aloAPI/alo-common.hpp"

class QStringView;
class QByteArray;

namespace alo::Utils
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
	inline ALO_API QByteArray IntToArray(qint32 source);
	/**
	 *	
	 */
	inline ALO_API qint32 ArrayToInt(QByteArray source);
	/**
	 * 
	 */
	inline ALO_API QString CurrentDateTimeStr(const QString& format = "hh:mm:ss dd.MM.yyyy");
	/**
	 *
	 */
	inline ALO_API std::string CurrentDateTimeStr_STD(const std::string& format = "hh:mm:ss dd.MM.yyyy");
}
