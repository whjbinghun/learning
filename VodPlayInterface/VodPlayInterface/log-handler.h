#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QCoreApplication>

extern void message_output( QtMsgType type, const QMessageLogContext &context, const QString &msg );

#endif // LOGHANDLER_H
