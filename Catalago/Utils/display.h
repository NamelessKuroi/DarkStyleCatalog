#ifndef DISPLAY_H
#define DISPLAY_H

//Qt
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QSize>
#include <QFont>
#include <QFontMetrics>
#include <QPair>

#define NON_AVAILABLE "N / A"


class Display
{
	
public:

    static QPair < QImage , QImage > scalePhotoAndThumbnail ( QString  );

    static QPixmap scaled(QPixmap & , int , int );
	
	static QFont Typography();

    static int WidthPhotoOFItemList();
	static int HeightPhotoOFItemList();
	
    static int WidthPhoto();
    static int HeightPhoto();

    static int WidthSplash();
    static int HeightSplash();

	static  QSize GridSizeOfListAnime();
    static  QString FitToolTip(QString);
    static  QString buildBlock( QString );
};

#endif // DISPLAY_H
