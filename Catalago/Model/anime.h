#ifndef ANIME_H
#define ANIME_H
#include <QDebug>
#include <QString>
#include <Catalago/Model/kind.h>

#include <QVector>
#include <QPixmap>

#include <Catalago/Utils/processaddress.h>
#include <Catalago/Utils/display.h>
class Anime
{
  public:
     enum Info{ TITLE , SYNOPSIS , PHOTO,
               CHAPTER , STATE ,  KIND , YEAR};

    int getIdAnime() const;
    void setIdAnime(int value);

    QString getTitle() const;
    void setTitle(const QString &value);

    QString getAddressPhoto() const;
    void setAddressPhoto(const QString &value);

    QString getState() const;
    void setState(const QString &value);

    int getChapter() const;
    void setChapter(int value);

    QString getSynopsis() const;
    void setSynopsis(const QString &value);


    int getYear() const;
    void setYear(int value);

    QString getListKind();


    QPixmap getPhoto() const;
    void setPhoto(const QPixmap &value);

     QPixmap getThumbnail() const;
    void setThumbnail(const QPixmap &value);
	
	QVector<Kind *> getKinds() const;
    void setKinds(const QVector<Kind *> &value);

    void updatePhotoAndThumbnail();
	
    bool operator < ( const Anime  anime ) const;  

    bool operator == ( const Anime  anime ) const;

private:
    int idAnime;
    QString title;
    QString synopsis;
    QString addressPhoto;
    QPixmap photo;
    QPixmap thumbnail;
    int chapter;
    QString state;
    int year;
    QVector < Kind * > kinds;
};

#endif
