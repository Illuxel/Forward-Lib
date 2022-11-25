#pragma once

#include <QtGlobal>

#if defined(ALO_EXPORT)
#  define ALO_API Q_DECL_EXPORT
#else
#  define ALO_API Q_DECL_IMPORT
#endif
