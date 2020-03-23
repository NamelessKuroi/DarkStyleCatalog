#include "display.h"
#include <QImage>
#include <QStringList>

QPair < QImage , QImage > Display::scalePhotoAndThumbnail(QString  AddressPhoto)
{

    int widthPhoto  = WidthPhoto();
    int heightPhoto = HeightPhoto();

    QImage currentPhoto (AddressPhoto);

    QImage photo = currentPhoto.scaled(widthPhoto , heightPhoto, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    widthPhoto = WidthPhotoOFItemList();
    heightPhoto = HeightPhotoOFItemList();

    QImage thumbnail =  currentPhoto.scaled(widthPhoto , heightPhoto, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


    return QPair < QImage , QImage > ( photo , thumbnail);

}

QPixmap Display::scaled(QPixmap & photo, int width, int height)
{
    return photo.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}


QFont Display::Typography()
{
    int LETTER_SIZE = 10;
    QString LETTER_TYPE = "Times";

    return QFont ( LETTER_TYPE, LETTER_SIZE, QFont::Bold );
}


int Display::WidthPhotoOFItemList()
{
    QDesktopWidget * desktop = qApp->desktop();
    return desktop->width() * 0.30 ;
}

int Display::HeightPhotoOFItemList()
{
    QDesktopWidget * desktop = qApp->desktop();
    return desktop->height() * 0.27;
}

QSize Display::GridSizeOfListAnime()
{
    int widthOffset  = 15;
    int heightOffset = 32;

    int widthGrid = WidthPhotoOFItemList() + widthOffset;
    int heightGrid = HeightPhotoOFItemList() + heightOffset;

    return QSize(widthGrid , heightGrid);
}

QString Display::FitToolTip(QString text)
{
    QString fitText;
    fitText.clear();
    QStringList blocks  = text.split("\n");
    fitText+="\n";

    for(QString block : blocks)
    {
        fitText += buildBlock(block);
        fitText += "\n";
    }
    return fitText;

}

QString Display::buildBlock(QString block)
{
    QStringList tokens = block.split(" ");
    QString fitTokens;
    fitTokens.clear();

    int QuantityOfLetterbyWord = 6;
    int QuantityOfWordbySentence = 10;
    int sizeSentence =  QuantityOfLetterbyWord * QuantityOfWordbySentence;
    int currentQuantityOfLetter = 0;

    for(QString token :tokens)
    {
        if(currentQuantityOfLetter + token.size() < sizeSentence )
        {

            fitTokens += token;
            fitTokens+=" ";
            currentQuantityOfLetter += token.size();

        }
        else
        {
            currentQuantityOfLetter = token.size();
            fitTokens +="\n";
            fitTokens += token;
            fitTokens +=" ";
        }
    }
    return fitTokens;
}


int Display::WidthPhoto()
{
    QDesktopWidget * desktop = qApp->desktop();
    return desktop->width() * 0.5;
}

int Display::HeightPhoto()
{
    QDesktopWidget * desktop = qApp->desktop();
    return desktop->height() * 0.35;
}


int Display::WidthSplash()
{
    QDesktopWidget * desktop = qApp->desktop();
    return desktop->width() * 0.58;
}

int Display::HeightSplash()
{
    QDesktopWidget * desktop = qApp->desktop();
    return desktop->height() * 0.52;
}





