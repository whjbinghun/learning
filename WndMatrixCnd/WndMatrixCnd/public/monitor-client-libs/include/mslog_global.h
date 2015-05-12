#ifndef MS_LOG_GLOBAL_H
#define MS_LOG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MS_LOG_LIBRARY)
#  define MS_LOG_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define MS_LOG_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#endif // MS_LOG_GLOBAL_H
