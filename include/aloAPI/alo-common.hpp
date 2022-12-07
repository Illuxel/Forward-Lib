#pragma once

#include <QtGlobal>

#include <QtCore>
#include <QtNetwork>
#include <QtSql>

#if defined(ALO_EXPORT)
#  define ALO_API Q_DECL_EXPORT
#else
#  define ALO_API Q_DECL_IMPORT
#endif