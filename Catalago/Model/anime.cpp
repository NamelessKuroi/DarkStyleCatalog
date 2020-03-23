#include "anime.h"

QString Anime::getTitle() const
{
    return title;
}

void Anime::setTitle(const QString &value)
{
    title = value;
}

QString Anime::getAddressPhoto() const
{
    return addressPhoto;
}

void Anime::setAddressPhoto(const QString &value)
{
    addressPhoto = value;
}

QString Anime::getState() const
{
    return state;
}

void Anime::setState(const QString &value)
{
    state = value;
}

int Anime::getChapter() const
{
    return chapter;
}

void Anime::setChapter(int value)
{
    chapter = value;
}

QString Anime::getSynopsis() const
{
    return synopsis;
}

void Anime::setSynopsis(const QString &value)
{
    synopsis = value;
}

int Anime::getIdAnime() const
{
    return idAnime;
}

void Anime::setIdAnime(int value)
{
    idAnime = value;
}

int Anime::getYear() const
{
    return year;
}

void Anime::setYear(int value)
{
    year = value;
}

QString Anime::getListKind()
{
    if(kinds.isEmpty())
        return "N / A";

    QString listKinds = "";
    QString name = kinds[0]->getName();


    if(kinds[0]->getDescription()!="")
        listKinds += QString("<a href=\"%1\" style=\" text-decoration: none;\" >%1</a>").arg(name);
    else
        listKinds += QString("<a  style=\" text-decoration: none; color:white\" >%1</a>").arg(name);


    for( int index = 1 ; index < kinds.size();index++ )
    {
        name = kinds[index]->getName();
        listKinds+=", ";
        if(kinds[index]->getDescription()!="")
            listKinds+=QString("<a href=\"%1\" style=\" text-decoration: none;\" >%1</a>").arg(name);
        else
            listKinds+=QString("<a  style=\" text-decoration: none; color:white\" >%1</a>").arg(name);

    }
    return listKinds;
}


QPixmap Anime::getPhoto() const
{
    return photo;
}

void Anime::setPhoto(const QPixmap &value)
{
    photo = value;
}

QVector<Kind *> Anime::getKinds() const
{
    return kinds;
}

void Anime::setKinds(const QVector<Kind *> &value)
{
    kinds = value;
}

QPixmap Anime::getThumbnail() const
{
    return thumbnail;
}

void Anime::setThumbnail(const QPixmap &value)
{
    thumbnail = value;
}


void Anime::updatePhotoAndThumbnail()
{
	
	int widthPhoto  = Display::WidthPhoto();
    int heightPhoto = Display::HeightPhoto();
    
	QPixmap currentPhoto (addressPhoto);
    
    photo = Display::scaled(currentPhoto , widthPhoto , heightPhoto);

    widthPhoto = Display::WidthPhotoOFItemList();
    heightPhoto = Display::HeightPhotoOFItemList();

    thumbnail = Display::scaled(currentPhoto , widthPhoto , heightPhoto);
}


bool Anime::operator < ( const Anime  anime ) const
{
     return title < anime.getTitle();
}

bool Anime::operator ==( const Anime  anime )const
{
     return idAnime == anime.getIdAnime();
}
