﻿#ifndef MS_LOGICLAYER_GLOBAL_H
#define MS_LOGICLAYER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MS_LOGICLAYER_LIBRARY)
#  define MS_LOGICLAYER_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define MS_LOGICLAYER_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#endif // MS_LOGICLAYER_GLOBAL_H
