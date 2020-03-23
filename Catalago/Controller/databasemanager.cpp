#include "databasemanager.h"
#include <QtAlgorithms>
#include <QSqlError>
#include <QFileInfo>
#include <Catalago/Utils/display.h>


bool DatabaseManager::OrderByTitleAnime(Anime * firstAnime ,Anime * secondAnime)
{
    return firstAnime->getTitle() < secondAnime->getTitle();
}

bool DatabaseManager::OrderByNameKind(Kind * firstKind , Kind * secondKind )
{
    return firstKind->getName() < secondKind->getName();
}

bool DatabaseManager::OrderByNameState(State * firstState , State * secondState )
{
    return firstState->getName() < secondState->getName();
}

char DatabaseManager::firstLetterFromName(QString name)
{
    return name.at(0).toLatin1();
}

bool DatabaseManager::startConnection()
{ 
    QSqlDatabase connection;
    connection = QSqlDatabase::addDatabase("QSQLITE");
    connection.setDatabaseName(ProcessAddress::getAddress("DataBase/DataBase.db"));
    return connection.open();
}

QList<QPair<QString, double> > DatabaseManager::valuesForStatesChartPie()
{

    QList < QPair < QString , double > > values;

    QString name;
    double quantity;

    QSqlQuery query;
    qDebug()<<query.exec("SELECT Animes.State , count(*) FROM Animes GROUP BY Animes.State ORDER BY Animes.State");

    while(query.next())
    {
        name = query.value(0).toString();
        quantity = query.value(1).toDouble();
        values.append(QPair < QString , double >(name, quantity));
    }
    return values;
}

QVector < QList < QPair < QString , double > > >  DatabaseManager::valuesForKindsChartPie()
{
   QVector < QList < QPair < QString , double > > >
           kindByLetter ( 26 , QList < QPair < QString , double > >());
   QList < QPair < QString , double > > values;



    QString name;
    double quantity;

    QSqlQuery query;
    query.exec("SELECT Kinds.Name , count(AnimeByKind.IdAnime) FROM Kinds , AnimeByKind WHERE Kinds.IdKind = AnimeByKind.IdKind GROUP BY Kinds.IdKind  ORDER BY Kinds.Name");



    while(query.next())
    {
        name = query.value(0).toString();
        quantity = query.value(1).toDouble();
        values.append(QPair < QString , double >(name, quantity));
    }
    int position;
    char letter;

    for(QPair < QString , double > value : values)
    {
        letter = firstLetterFromName(value.first);
        if(!QChar::isLetter(letter))
            continue;
        letter = QChar::toUpper(letter);
        position = letter - 'A';
        kindByLetter[position].append(value);
    }

    return kindByLetter;
}


QVector <Anime * > DatabaseManager::getAnimes()
{
    QString  AddressImage = ProcessAddress::getAddress("Photos/Non-Image.png");
    QPixmap Non_Image(AddressImage);
    int widthPhoto  = Display::WidthPhoto();
    int heightPhoto = Display::HeightPhoto();

    QPixmap pPhoto = Display::scaled(Non_Image,widthPhoto,heightPhoto);

    widthPhoto = Display::WidthPhotoOFItemList();
    heightPhoto = Display::HeightPhotoOFItemList();

    QPixmap pThumbnail = Display::scaled(Non_Image, widthPhoto , heightPhoto);


    QVector < Anime * > animes;
    Anime  * temp;

    int idAnime;
    QString title;
    QString state;
    QString synospsis;
    int chapter;
    int year;
    QString photo;

    QSqlQuery query("SELECT * FROM Animes");

    while (query.next()) {
        idAnime = query.value(0).toInt();
        title = query.value(1).toString();
        state = query.value(2).toString();
        chapter = query.value(3).toInt();
        synospsis = query.value(4).toString();
        year = query.value(5).toInt();
        photo = query.value(6).toString();

        temp = new Anime();
        temp->setIdAnime(idAnime);
        temp->setTitle(title);
        temp->setState(state);
        temp->setSynopsis(synospsis);
        temp->setChapter(chapter);
        temp->setYear(year);
        temp->setPhoto(pPhoto);
        temp->setThumbnail(pThumbnail);
        QFileInfo file (photo);

        if( !file.exists())
            temp->setAddressPhoto(AddressImage);
        else
            temp->setAddressPhoto(photo);

        temp->setKinds(getKindsByAnime(idAnime));
        animes.append(temp);
    }
    qSort(animes.begin(),animes.end() ,OrderByTitleAnime);
    return animes;
}

bool DatabaseManager::existAnimeWithTitle(QString title)
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM Animes WHERE Title = '%1' ;").arg(title));
    return query.next();
}

bool DatabaseManager::existPhotoWithName(QString name)
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM Animes WHERE Photo LIKE '%%1' ;").arg(name));
    return query.next();
}

int DatabaseManager::addAnime(Anime * newAnime)
{

    QSqlQuery query;
    int idAnime = 0;
    query.exec(QString("SELECT Max(IdAnime) FROM Animes"));
    while(query.next())
    {
        idAnime = query.value(0).toInt() + 1;
    }
    newAnime->setIdAnime(idAnime);

    query.prepare("INSERT INTO Animes (IdAnime , Title, State, Chapter ,Synopsis ,Year ,Photo) "
                  "VALUES ( :idAnime , :title , :state , :chapter ,:synopsis, :year , :photo ) ; ");
    query.bindValue(":idAnime",newAnime->getIdAnime());
    query.bindValue(":title" ,newAnime->getTitle().trimmed());
    query.bindValue(":state",newAnime->getState());
    query.bindValue(":chapter",newAnime->getChapter());
    query.bindValue(":synopsis",newAnime->getSynopsis());
    query.bindValue(":year",newAnime->getYear());
    query.bindValue(":photo",newAnime->getAddressPhoto());
    query.exec();

    return idAnime;
}

void DatabaseManager::deleteAnime(int idAnime)
{

    QSqlQuery query;
    query.exec(QString("DELETE FROM AnimeByKind WHERE IdAnime = %1").arg(idAnime));
    query.exec(QString("DELETE FROM Animes WHERE IdAnime = %1").arg(idAnime));
}

void DatabaseManager::SaveTitleAnime(int idAnime , QString title)
{


    QSqlQuery query;
    query.exec(QString("UPDATE Animes SET Title = '%1' WHERE IdAnime = %2").arg(title.trimmed()).arg(idAnime));
}

void DatabaseManager::SaveStateAnime(int idAnime , QString state)
{

    QSqlQuery query;
    query.exec(QString("UPDATE Animes SET State = '%1' WHERE IdAnime = %2").arg(state).arg(idAnime));

}

void DatabaseManager::SaveSynopsisAnime(int idAnime , QString synopsis)
{

    QSqlQuery query;
    query.exec(QString("UPDATE Animes SET Synopsis = '%1' WHERE IdAnime = %2").arg(synopsis).arg(idAnime));

}

void DatabaseManager::SaveChapterAnime(int idAnime ,  int chapter)
{
    ;
    QSqlQuery query;
    query.exec(QString("UPDATE Animes SET Chapter = %1 WHERE IdAnime = %2").arg(chapter).arg(idAnime));

}

void DatabaseManager::SaveYearAnime(int idAnime ,  int year )
{

    QSqlQuery query;
    query.exec(QString("UPDATE Animes SET Year = %1 WHERE IdAnime = %2").arg(year).arg(idAnime));

}

void DatabaseManager::SavePhotoAnime(int idAnime ,QString Address)
{

    QSqlQuery query;
    query.exec(QString("UPDATE Animes SET Photo = '%1' WHERE IdAnime = %2").arg(Address).arg(idAnime));

}



QVector<Kind * > DatabaseManager::getKinds()
{
    QVector < Kind * > kinds;
    int idKind;
    Kind *  temp;

    QString name;
    QString description;

    QSqlQuery query("SELECT * FROM Kinds");
    while (query.next()) {

        idKind = query.value(0).toInt();
        name = query.value(1).toString();
        description  = query.value(2).toString();
        temp = new Kind();
        temp->setIdKind(idKind);
        temp->setName(name);
        temp->setDescription(description);
        kinds.append(temp);
    }

    qSort(kinds.begin(), kinds.end(), OrderByNameKind);

    return kinds;

}

bool DatabaseManager::existKindWithName(QString name)
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM Kinds WHERE Name = '%1';").arg(name));
    return query.next();
}

void DatabaseManager::addKind(Kind * newKind)
{
    QSqlQuery query;
    int idKindMax = 0;
    query.exec(QString("SELECT Max(IdKind) FROM Kinds"));
    while(query.next())
    {
        idKindMax = query.value(0).toInt() + 1;
    }
    newKind->setIdKind(idKindMax);
    query.prepare("INSERT INTO Kinds (IdKind , Name, Description) "
                  "VALUES ( :idKind , :name , :description ) ; ");
    query.bindValue(":idKind",newKind->getIdKind());
    query.bindValue(":name" ,newKind->getName().trimmed());
    query.bindValue(":description",newKind->getDescription());
    query.exec();
}

void DatabaseManager::deleteKind(int idKind)
{
   QSqlQuery query;
   query.exec(QString("DELETE FROM AnimeByKind WHERE IdKind = %1").arg(idKind));
   query.exec(QString("DELETE FROM Kinds WHERE IdKind = %1").arg(idKind));
}

void DatabaseManager::SaveNameKind(int idKind, QString name)
{
    QSqlQuery query;
    query.exec(QString("UPDATE Kinds SET Name = '%1' WHERE IdKind = %2").arg(name.trimmed()).arg(idKind));

}

void DatabaseManager::SaveDescriptionKind(int idKind , QString description)
{
    QSqlQuery query;
    query.exec(QString("UPDATE Kinds SET Description = '%1' WHERE IdKind = %2").arg(description).arg(idKind));


}



QVector<Kind *> DatabaseManager::getKindsByAnime(int IdAnime)
{
    QVector<Kind * > kinds;
    int idAnimeKind;
    QString name;
    QString description;
    QSqlQuery query;

    query.exec(QString("SELECT Kinds.* FROM AnimeByKind , Kinds "
                       "WHERE AnimeByKind.IdAnime = %1 AND AnimeByKind.IdKind = Kinds.IdKind").arg(IdAnime));

    while(query.next())
    {

        idAnimeKind = query.value(0).toInt();
        name  = query.value(1).toString();
        description = query.value(2).toString();
        Kind * temp = new Kind();
        temp->setIdKind(idAnimeKind);
        temp->setName(name);
        temp->setDescription(description);
        kinds.append(temp);
    }

    qSort(kinds.begin(), kinds.end(), OrderByNameKind);

    return kinds;
}

void DatabaseManager::AddKindToAnime(int IdAnime , int IdKind )
{
    QSqlQuery query;
    query.prepare("INSERT INTO AnimeByKind ( IdAnime , IdKind) "
                  "VALUES (:IdAnime , :IdKind)");

    query.bindValue(":IdAnime",IdAnime);
    query.bindValue(":IdKind",IdKind);
    query.exec();


}

void DatabaseManager::RemoveKindOfAnime(int, int)
{

}

void DatabaseManager::RemoveKindsOfAnime(int IdAnime)
{
    QSqlQuery query;
    query.exec(QString("DELETE FROM AnimeByKind WHERE IdAnime = %1").arg(IdAnime));

}



QVector<State *> DatabaseManager::getStates()
{
    QVector < State * > states;
    int idState;
    QString name;
    QString description;

    QSqlQuery query("SELECT * FROM States");
    while (query.next()) {

        idState = query.value(0).toInt();
        name = query.value(1).toString();
        description  = query.value(2).toString();
        State *  temp = new State();
        temp->setIdState(idState);
        temp->setName(name);
        temp->setDescription(description);
        states.append(temp);
    }
    qSort(states.begin(),states.end());
    return states;
}

bool DatabaseManager::existStateWithName(QString name)
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM States WHERE Name = '%1';").arg(name));
    return query.next();
}

void DatabaseManager::addState(State * newState)
{
    QSqlQuery query;
    int idStateMax = 0;
    query.exec(QString("SELECT Max(IdState) FROM States"));
    while(query.next())
    {
        idStateMax = query.value(0).toInt() + 1;
    }
    newState->setIdState(idStateMax);
    query.prepare("INSERT INTO States (IdState , Name, Description) "
                  "VALUES ( :idState , :name , :description ) ; ");
    query.bindValue(":idState",newState->getIdState());
    query.bindValue(":name" ,newState->getName().trimmed());
    query.bindValue(":description",newState->getDescription());
    query.exec();
}

void DatabaseManager::deleteState(int idState)
{
    QSqlQuery query;
    query.exec(QString("DELETE FROM AnimeByState WHERE idState = %1").arg(idState));
    query.exec(QString("DELETE FROM States WHERE IdState = %1").arg(idState));
}

void DatabaseManager::SaveNameState(int idState, QString name)
{
    QSqlQuery query;
    query.exec(QString("UPDATE States SET Name = '%1' WHERE IdState = %2").arg(name).arg(idState));
}

void DatabaseManager::SaveDescriptionState(int idState , QString description)
{
    QSqlQuery query;
    query.exec(QString("UPDATE States SET Description = '%1' WHERE IdKind = %2").arg(description).arg(idState));

}

State * DatabaseManager::getStateByAnime(int IdAnime)
{
    State *  state = new State();
    int idState;
    QString name;
    QString description;
    QSqlQuery query;

    query.exec(QString("SELECT States.* FROM AnimeByState , States "
                       "WHERE AnimeByState.IdAnime = %1 AND AnimeByState.IdState = States.IdState")
               .arg(IdAnime));

    while(query.next())
    {

        idState = query.value(0).toInt();
        name  = query.value(1).toString();
        description = query.value(2).toString();
        state->setIdState(idState);
        state->setName(name);
        state->setDescription(description);
    }

    return state;
}

void DatabaseManager::AddStateToAnime(int IdAnime, int IdState)
{
    QSqlQuery query;
    query.prepare("INSERT INTO AnimeByState ( IdAnime , IdState) "
                  "VALUES (:IdAnime , :IdState)");
    query.bindValue(":IdAnime",IdAnime);
    query.bindValue(":IdState",IdState);
    query.exec();

}

void DatabaseManager::RemoveStateOfAnime(int, int)
{

}

void DatabaseManager::RemoveStatesOfAnime(int IdAnime)
{
    QSqlQuery query;
    query.exec(QString("DELETE FROM AnimeByState WHERE IdAnime = %1").arg(IdAnime));
}

