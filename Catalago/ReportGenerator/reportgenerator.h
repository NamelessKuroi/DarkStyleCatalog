#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QObject>
#include <QDebug>

#include <QDomDocument>
#include <QDomElement>

#include <QList>
#include <QPair>

#include <QProgressDialog>

#include <vector>

#include <Catalago/Model/anime.h>
#include <Catalago/Controller/databasemanager.h>
#include <QMap>


class ReportGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ReportGenerator(QVector < Anime *>  animes , QObject *parent = 0);
    enum button{ONLY_BACK = 1 , ONLY_NEXT , BOTH};

    void generateReport();

    QString getAddressDestiny() const;
    void setAddressDestiny(const QString &value);

private:
    //Util
    void toCreateDirForReport();

    void createIndex();
    void AnimeToHtml();

    void generateAnimesByInitialLetter();

    QChar initialLetter(QString word);


    //Html
    QDomDocument createHtmlIndex();
    QDomDocument createHtmlAnime();

    //Head
    void createHeadIndex(QDomElement & html ,QDomDocument & doc);
    void createHeadAnime(QDomElement & html ,QDomDocument & doc);

    void createTitleHeadIndex(QDomElement & html , QDomDocument & doc);
    void createTitleHeadAnime(QDomElement & html , QDomDocument & doc);

    void createMeta(QDomElement & head, QDomDocument & doc ,
                    QList < QPair < QString, QString > > attributes);
    void createLink(QDomElement & head , QDomDocument & doc ,
                    QList < QPair < QString, QString > > attributes);


    //Body
    void createBodyIndex(QDomElement & html , QDomDocument & doc);
    void createBodyAnime(QDomElement & html , QDomDocument & doc);

    void createBlockIndex(QDomElement & body , QDomDocument & doc);
    void createBlockAnime(QDomElement & body , QDomDocument & doc);

    void createIndexByInitialLetter(QDomElement & block , QDomDocument & doc,
                                    QChar letter);


    void createTitleAnime(QDomElement & block , QDomDocument & doc);
    void createPhotoAnime(QDomElement & block , QDomDocument & doc);
    void createSynopsisAnime(QDomElement & block , QDomDocument & doc);
    void createKindsAnime(QDomElement & block , QDomDocument & doc);
    void createExtrasAnime(QDomElement & block , QDomDocument & doc);
    void createButtons(QDomElement & body , QDomDocument & doc , button state = BOTH );


    Anime * beforeAnime;
    Anime * nextAnime;
    Anime * currentAnime;

    QString addressDestiny;
    QVector < Anime * > animes;
    QMap < QChar , QVector < Anime * > > AnimesByInitialLetter;

};

#endif // REPORTGENERATOR_H
