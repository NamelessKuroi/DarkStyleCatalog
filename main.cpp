#include "DarkStyle.h"
#include "framelesswindow.h"
#include <Catalago/Utils/processaddress.h>
#include <Catalago/Utils/display.h>
#include <Catalago/catalag.h>
#include <Catalago/Utils/logger.h>
#include <Catalago/Utils/randomsplash.h>

//Qt
#include <QSplashScreen>
#include <QTime>

#include <QApplication>
#include <QPixmap>
#include <QImageReader>

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>

#include <QThread>
#include <QObject>

//#define CONSOLE
#define FILE

#define SECOND(X) ( X * 1000)

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

#ifdef CONSOLE
    qInstallMessageHandler(0);
#endif

#ifdef FILE
    qInstallMessageHandler(Logger::messageHander);
#endif

    Logger::emptyLog();

    a.setStyle(new DarkStyle());

    QIcon icon(ProcessAddress::getAddress("Icons/IconCatalag.png"));
    a.setWindowIcon(icon);


    QPixmap pixmap(RandomSplash::randomSplash());

    QSplashScreen *splash = new QSplashScreen() ;

    int widhtSplash = Display::WidthSplash();
    int heightSplash = Display::HeightSplash();

    splash->setPixmap(Display::scaled(pixmap,widhtSplash,heightSplash));
    splash->setFont(QFont("Ink Free",30));
    splash->show();
    a.processEvents();
    splash->showMessage("Loading...", Qt::AlignCenter , Qt::green);

    QThread::msleep(SECOND(4));

    FramelessWindow * frame = new FramelessWindow() ;
    Catalag * catalag = new Catalag() ;
    catalag->showMaximized();
    frame->setWindowState(Qt::WindowFullScreen);
    frame->setWindowTitle("Anime Catalog");
    frame->setWindowIcon(icon);
    frame->setContent(catalag);
    frame->showFullScreen();
    splash->finish(frame);


    catalag->setFocus();
    return a.exec();
}


