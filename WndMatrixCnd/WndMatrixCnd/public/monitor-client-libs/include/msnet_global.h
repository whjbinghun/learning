#ifndef MS_NET_GLOBAL_H
#define MS_NET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MS_NET_LIBRARY)
#  define MS_NET_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define MS_NET_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#endif // MS_NET_LIBRARY_EXPORT
