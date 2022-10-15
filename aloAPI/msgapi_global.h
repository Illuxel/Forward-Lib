#pragma once

#include <QtCore/qglobal.h>
#include <QtCore/QObject>

#if defined(ALO_API_LIB)
#  define ALO_API Q_DECL_EXPORT
#else
#  define ALO_API Q_DECL_IMPORT
#endif