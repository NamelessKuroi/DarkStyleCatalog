#ifndef RANDOMSPLASH_H
#define RANDOMSPLASH_H


class QDir;
class QString;
class QStringList;
class RandomSplash
{
public:
    static QString  randomSplash();
private :
     static QDir * loadDirSplash();
     static int loadPositionSplashScreen();
     static void savePositionSplashScreen(int positionSplashScreen );
     static QStringList loadFilterByImageType();
     static QString loadSplahScreenForPosition(QDir * dirSplahScreen , int positionSplashScreen);
 };

#endif // RANDOMSPLASH_H
