#ifndef LOGGER_H
#define LOGGER_H

#include <QtMessageHandler>

class QString;

class Logger
{
public:
    static void messageHander(QtMsgType type , const QMessageLogContext & , const QString &message);
    static void emptyLog();
};

#endif // LOGGER_H
