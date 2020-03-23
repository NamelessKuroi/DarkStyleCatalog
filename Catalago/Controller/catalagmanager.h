#ifndef CATALAGMANAGER_H
#define CATALAGMANAGER_H

#include <QObject>

#include <Catalago/Controller/databasemanager.h>
#include <Catalago/Model/anime.h>
#include <Catalago/Model/kind.h>
#include <Catalago/Model/state.h>


#include <QVector>
#include <QListWidget>
#include <QAction>
#include <QMenu>

class CatalagManager : public QObject
{
    Q_OBJECT
public:
    explicit CatalagManager(QObject *parent = 0);

    void updatePhotoAndThumbnail(int index, QImage &, QImage & );
    void updateAnimes();
    void updateKinds();
    void updateStates();
    void updateAnimeByKinds();

    Anime *  findAnimeByTitle(QString pTitle);
    Kind  *  findKindByName(QString pName);
    State  * findStateByName(QString pName);

    int findIndexOfAnimeByTitle(QString pTitle);
    int findIndexOfKindByName(QString pName);
    int findIndexOfStateByName(QString pName);


    void appendAnime( Anime * pAnime );
    void appendKind(Kind * pKind);
    void appendState(State * pState);


    QVector<Anime *> getAnimes() const;
    QVector<Kind *> getKinds() const;
    QVector<State *> getStates() const;

    void removeAnime(QString pTitle);
    void removeKind(QString pName);
    void removeState(QString pName);

    QString getRecentDirectory() const;
    void setRecentDirectory(const QString &value);


    void fillFilter();

    void saveStateOfFilter();
    void saveStateOfFilterByKind();
    void saveStateOfFilterByStatte();

    void restoreStateOfFilter();
    void restoreStateOfFilterByKind();
    void restoreStateOfFilterByStatte();

    QVector<QAction *> getActionStates() const;
    void setActionStates(const QVector<QAction *> &value);

    QVector<QAction *> getActionKinds() const;
    void setActionKinds(const QVector<QAction *> &value);

    QVector<QAction *> getActionEmptyFields() const;
    void setActionEmptyFields(const QVector<QAction *> &value);

    QMenu *getStateMenu() const;
    void setStateMenu(QMenu *value);

    QMenu *getKindMenu() const;
    void setKindMenu(QMenu *value);

    QMenu *getEmptyFieldMenu() const;
    void setEmptyFieldMenu(QMenu *value);

    QAction *getClearState() const;
    void setClearState(QAction *value);

signals:

    void newAnimeAdded(Anime * pAnime);
    void newKindAdded(Kind  * pKind);
    void newStateAdded(State * pState);

    void titleAnimeWasChanged(Anime * pAnime);
    void nameKindWasChanged(Kind  * pKind);
    void nameStateWasChanged(State * pState);

    void animeWasEdited(Anime * pAnime);
    void kindWasEdited(Kind  * pKind);
    void stateWasEdited(State * pState);

    void animeWasRemoved();
    void kindWasRemoved();
    void stateWasRemoved();

    void refreshForm();
    void updateFilter();

private:

    void loadStates();
    void loadKinds();
    void loadEmptyField();

    QVector < Anime * > animes;
    QVector < Kind * > kinds;
    QVector < State * > states;

    QVector < QAction *> actionStates;
    QVector < QAction *> actionKinds;
    QVector < QAction *> actionEmptyFields;
    QVector < QAction *> actionAuthors;
    QVector < QAction *> actionStudios;

    QMap < QString , bool>  ActionFilter ;

    QMenu * stateMenu;
    QMenu * kindMenu;
    QMenu * emptyFieldMenu;
	QMenu * AuthorMenu;
    QMenu * StudioMenu;

    QAction * clearState;
	QAction * clearKind;
	QAction * clearAuthor;
	QAction * clearStudio;

    QString  recentDirectory;

};

#endif // CATALAGMANAGER_H
