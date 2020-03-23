#include "processaddress.h"

//#define BUILD
#define DEPLOY

QString ProcessAddress::getAddress( const QString address)
{
    #ifdef BUILD
    const QString  AddressGlobal = "../DarkStyleCatalog/Catalago/";
    #endif

    #ifdef DEPLOY
    const QString   AddressGlobal = "";
    #endif

    return QString("%1%2").arg(AddressGlobal)
            .arg(address);
}

QString ProcessAddress::getAddress(const QString pDirectory , const QString pFile  )
{
    #ifdef BUILD
    const QString  AddressGlobal = "../DarkStyleCatalog/Catalago/";
    #endif

    #ifdef DEPLOY
    const QString   AddressGlobal = "";
    #endif

    return QString("%1%2/%3").arg(AddressGlobal)
            .arg(pDirectory)
            .arg(pFile);
}


const char *  ProcessAddress::getRawAddress(const QString address )
{
    #ifdef BUILD
    const QString  AddressGlobal = "../DarkStyleCatalag/Catalago/";
    #endif

    #ifdef DEPLOY
    const QString   AddressGlobal = "";
    #endif

    return QString("%1%2")
            .arg(AddressGlobal)
            .arg(address)
            .toStdString().c_str();

}



