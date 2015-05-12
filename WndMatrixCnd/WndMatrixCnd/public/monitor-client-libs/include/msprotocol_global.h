﻿#ifndef MS_PROTOCOL_GLOBAL_H
#define MS_PROTOCOL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MS_PROTOCOL_LIBRARY)
#  define MS_PROTOCOL_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define MS_PROTOCOL_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#endif // MS_PROTOCOL_GLOBAL_H
