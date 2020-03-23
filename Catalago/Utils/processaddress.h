#ifndef PROCESSADDRESS_H
#define PROCESSADDRESS_H

#include <QString>


class ProcessAddress
{
public:
    static QString getAddress(const QString );
    static QString getAddress(const QString pDirectory , const QString pFile  );
    static const char *getRawAddress(const QString );
};

#endif
