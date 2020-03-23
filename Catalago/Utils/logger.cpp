#include "logger.h"

//Qt
#include <QFile>
#include <QTextStream>
#include <QDateTime>

void Logger::messageHander(QtMsgType type,
                   const QMessageLogContext&/* context */,
                   const QString& message) {
    QString levelText;
    switch (type) {
    case QtDebugMsg:
        levelText = "Debug";
        break;
    case QtInfoMsg:
        levelText = "Info";
        break;
    case QtWarningMsg:
        levelText = "Warning";
        break;
    case QtCriticalMsg:
        levelText = "Critical";
        break;
    case QtFatalMsg:
        levelText = "Fatal";
        break;
    }
    QString text = QString("[%3] - [%1] : %2")
            .arg(levelText)
            .arg(message)
            .arg(QDateTime::currentDateTime().toString());

    QFile file("Catalago.log");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append ))
        return;

    QTextStream textStream(&file);
    textStream << text << endl;

    file.close();
}

void Logger::emptyLog()
{
    QFile file("Catalago.log");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
        return ;
    file.close();

}
