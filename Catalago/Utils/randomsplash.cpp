#include "randomsplash.h"

#include <QDir>
#include <QImageReader>
#include <QStringList>
#include <QTextStream>
#include <Catalago/Utils/processaddress.h>
QDir * RandomSplash::loadDirSplash()
{
    QString dirAddressSplahScreen =  ProcessAddress::getAddress("Splash");
    QDir * dirSplahScreen = new QDir(dirAddressSplahScreen);
    return  dirSplahScreen->exists() ? dirSplahScreen : nullptr;

}

int RandomSplash::loadPositionSplashScreen()
{
    QFile file("Splash_Position.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        return file.readAll().toInt();
    file.close();

    return 0;
}


void RandomSplash::savePositionSplashScreen(int positionSplashScreen )
{
    QFile file("Splash_Position.txt");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
        return ;

    QTextStream out(&file);
    out << positionSplashScreen + 1;
    file.close();

}

QStringList RandomSplash::loadFilterByImageType()
{
    QStringList filterByImageType;
    for (QByteArray format : QImageReader::supportedImageFormats()) {
        filterByImageType += "*." + format;
    }
    return filterByImageType;
}

QString RandomSplash::loadSplahScreenForPosition(QDir * dirSplahScreen , int positionSplashScreen)
{
    QStringList nameSplahScreen ;
    QStringList filterByImageType = loadFilterByImageType() ;

    for(  QString fileName : dirSplahScreen->entryList(filterByImageType , QDir::Files ) )
        nameSplahScreen << fileName;

    if(nameSplahScreen.isEmpty())
        return "";

    positionSplashScreen = positionSplashScreen %  nameSplahScreen.size();

    return ProcessAddress::getAddress(QString("Splash/%1").
                                      arg(nameSplahScreen.at(positionSplashScreen)));
}

QString RandomSplash::randomSplash()
{

    QDir * dirSplahScreen = loadDirSplash();
    if( dirSplahScreen == nullptr)
        return "";

    int  positionSplashScreen = loadPositionSplashScreen();
    savePositionSplashScreen(positionSplashScreen);
    return loadSplahScreenForPosition(dirSplahScreen , positionSplashScreen);

}
