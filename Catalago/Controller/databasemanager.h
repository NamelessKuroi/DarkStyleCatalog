#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QDebug>

#include <Catalago/Model/anime.h>
#include <Catalago/Model/kind.h>
#include <Catalago/Model/state.h>
#include <Catalago/Utils/processaddress.h>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QVector>

#include <QList>
#include <QPair>
#include <QString>

class DatabaseManager
{
public:

    static bool startConnection();
 
    //util
    static bool OrderByTitleAnime(Anime * firstAnime ,Anime * secondAnime);
    static bool OrderByNameKind(Kind * firstKind , Kind * secondKind );
    static bool OrderByNameState(State * firstState , State * secondState );
    static char firstLetterFromName(QString name);

    //Chart
    static QList < QPair<QString, double> > valuesForStatesChartPie();
    static QVector < QList < QPair < QString , double > > >  valuesForKindsChartPie();

    /*------------------------------------------Anime---------------------------------*/
    static QVector < Anime * > getAnimes();
    static bool existAnimeWithTitle(QString title);
    static bool existPhotoWithName(QString name);

    static int  addAnime( Anime *);
    static void deleteAnime(int);
    static void SaveTitleAnime(int ,QString );
    static void SaveStateAnime(int ,QString );
    static void SaveSynopsisAnime(int ,QString );
    static void SaveChapterAnime(int , int );
    static void SaveYearAnime(int , int );
    static void SavePhotoAnime(int ,QString );

 /*------------------------------------------Anime---------------------------------*/
 
 /*------------------------------------------Kind---------------------------------*/ 
    //Kind
    static QVector<Kind *> getKinds();
    static bool existKindWithName(QString name);

    static void addKind( Kind * );
    static void deleteKind(int);
    static void SaveNameKind(int ,QString );
    static void SaveDescriptionKind(int ,QString );

    //AnimeByKind;
    static QVector <Kind * > getKindsByAnime(int);
    static void AddKindToAnime(int , int);
    static void RemoveKindOfAnime(int , int);
    static void RemoveKindsOfAnime(int);
	
 /*------------------------------------------Kind---------------------------------*/ 

 /*------------------------------------------State---------------------------------*/ 
    static QVector<State *> getStates();
    static bool existStateWithName(QString name);

    static void addState( State * );
    static void deleteState(int);
    static void SaveNameState(int ,QString );
    static void SaveDescriptionState(int ,QString );


    // AnimeByState;
    static State *  getStateByAnime(int);
    static void AddStateToAnime(int , int);
    static void RemoveStateOfAnime(int , int);
    static void RemoveStatesOfAnime(int);

/*------------------------------------------State---------------------------------*/ 	
   


};

#endif
