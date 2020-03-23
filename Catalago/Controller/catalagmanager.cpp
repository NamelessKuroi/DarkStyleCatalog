#include "catalagmanager.h"
#include <algorithm>

CatalagManager::CatalagManager(QObject *parent) : QObject(parent)
{
    stateMenu      = new QMenu();
    kindMenu       = new QMenu();
    emptyFieldMenu = new QMenu();
	
    animes = DatabaseManager::getAnimes();
    kinds  = DatabaseManager::getKinds();
    states = DatabaseManager::getStates();
}

	
void CatalagManager::updatePhotoAndThumbnail(int index, QImage & pPhoto, QImage & pThumbnail)
{
    animes.at(index) -> setPhoto(QPixmap::fromImage(pPhoto));
    animes.at(index) -> setThumbnail(QPixmap::fromImage(pThumbnail));
}

void CatalagManager::updateAnimes()
{
    qSort(animes.begin(),animes.end(),DatabaseManager::OrderByTitleAnime);
}

void CatalagManager::updateKinds()
{
    qSort(kinds.begin(),kinds.end(),DatabaseManager::OrderByNameKind);
}

void CatalagManager::updateStates()
{
    qSort(states.begin(),states.end(),DatabaseManager::OrderByNameState);
}

void CatalagManager::updateAnimeByKinds()
{
    for(Anime * anime : animes)
        anime->setKinds(DatabaseManager::getKindsByAnime(anime->getIdAnime()));
}

void CatalagManager::appendAnime(Anime *pAnime)
{
    animes.append(pAnime);
    updateAnimes();
    emit(newAnimeAdded(pAnime));
}

void CatalagManager::appendKind(Kind *pKind)
{
    kinds.append(pKind);
    updateKinds();
    emit(newKindAdded(pKind));
}

void CatalagManager::appendState(State *pState)
{
    states.append(pState);
    updateStates();
    emit(newStateAdded(pState));
}

void CatalagManager::removeAnime(QString pTitle)
{
    int index = findIndexOfAnimeByTitle(pTitle);
    animes.removeAt(index);
}

void CatalagManager::removeKind(QString pName)
{
    int index = findIndexOfKindByName(pName);
    kinds.removeAt(index);
}

void CatalagManager::removeState(QString pName)
{
    int index = findIndexOfStateByName(pName);
    states.removeAt(index);
}


Anime *CatalagManager::findAnimeByTitle(QString pTitle)
{
    for (Anime * anime : animes) {
        if ( anime->getTitle() == pTitle )
            return anime;
    }
    return nullptr;
}

Kind *CatalagManager::findKindByName(QString pName)
{
    for (Kind * kind  : kinds) {
        if ( kind->getName() == pName )
            return kind;
    }
    return nullptr;
}

State *CatalagManager::findStateByName(QString pName )
{
    for (State * state : states) {
        if ( state->getName() == pName )
            return state;
    }
    return nullptr;
}

int CatalagManager::findIndexOfAnimeByTitle(QString pTitle)
{
    for (int index = 0 ; index < animes.size() ; index++) {
        if ( animes[index]->getTitle() == pTitle )
            return index;
    }
    return -1;
}

int CatalagManager::findIndexOfKindByName(QString pName)
{
    for (int index = 0 ; index < kinds.size() ; index++) {
        if ( kinds[index]->getName() == pName )
            return index;
    }
    return -1;
}

int CatalagManager::findIndexOfStateByName(QString pName)
{
    for (int index = 0 ; index < states.size() ; index++) {
        if ( states[index]->getName() == pName )
            return index;
    }
    return -1;
}

QString CatalagManager::getRecentDirectory() const
{
    return recentDirectory;
}

void CatalagManager::setRecentDirectory(const QString &value)
{
    recentDirectory = value;
}

void CatalagManager::fillFilter()
{
    loadStates();
    loadKinds();
    loadEmptyField();
}

void CatalagManager::saveStateOfFilter()
{
    ActionFilter.clear();
    saveStateOfFilterByKind();
    saveStateOfFilterByStatte();
    for(QAction * action : actionEmptyFields )
        ActionFilter[action->text()] = action->isChecked();
}

void CatalagManager::restoreStateOfFilter()
{
    fillFilter();
    restoreStateOfFilterByKind();
    restoreStateOfFilterByStatte();

    for(QAction * action : actionEmptyFields )
        action->setChecked(ActionFilter[action->text()]);
}

void CatalagManager::loadStates()
{
    actionStates.clear();
    stateMenu->clear();


    for (State * state : states) {
        QAction * tempAction = new QAction(state->getName());
        tempAction->setCheckable(true);
        tempAction->setChecked(true);
        actionStates.append(tempAction);
        stateMenu->addAction(tempAction);
    }
    clearState = new QAction("Clear States");
    stateMenu->addSeparator();
    stateMenu->addAction(clearState);
}

void CatalagManager::loadKinds()
{
    actionKinds.clear();
    kindMenu->clear();

    for (Kind * kind : kinds) {

        QAction * tempAction = new QAction(kind->getName());
        tempAction->setCheckable(true);
        actionKinds.append(tempAction);
        kindMenu->addAction(tempAction);
    }

}

void CatalagManager::loadEmptyField()
{
    actionEmptyFields.clear();
    emptyFieldMenu->clear();

    actionEmptyFields.append(emptyFieldMenu->addAction("Chapter"));
    actionEmptyFields.append(emptyFieldMenu->addAction("Synopsis"));
    actionEmptyFields.append(emptyFieldMenu->addAction("Year"));
    actionEmptyFields.append(emptyFieldMenu->addAction("Photo"));
    actionEmptyFields.append(emptyFieldMenu->addAction("Kinds"));

    for(QAction  * action  : actionEmptyFields)
    {
        action->setCheckable(true);
        action->setChecked(false);

    }
}

QAction *CatalagManager::getClearState() const
{
    return clearState;
}

void CatalagManager::setClearState(QAction *value)
{
    clearState = value;
}

QMenu *CatalagManager::getEmptyFieldMenu() const
{
    return emptyFieldMenu;
}

void CatalagManager::setEmptyFieldMenu(QMenu *value)
{
    emptyFieldMenu = value;
}

QMenu *CatalagManager::getKindMenu() const
{
    return kindMenu;
}

void CatalagManager::setKindMenu(QMenu *value)
{
    kindMenu = value;
}

QMenu *CatalagManager::getStateMenu() const
{
    return stateMenu;
}

void CatalagManager::setStateMenu(QMenu *value)
{
    stateMenu = value;
}


void CatalagManager::saveStateOfFilterByKind()
{
    if(actionKinds.size() != kinds.size())
    {
        qDebug()<<"Error Kinds";
        return;
    }
    for(int index = 0 ;  index < actionKinds.size() ; index++)
    {
        kinds[index]->setChecked(actionKinds[index]->isChecked());
    }
}

void CatalagManager::saveStateOfFilterByStatte()
{
    if(actionStates.size() != states.size())
    {
        qDebug()<<"Error States";
        return;
    }
    for(int index = 0 ;  index <actionStates.size() ; index++)
    {
        states[index]->setChecked(actionStates[index]->isChecked());
    }
}

void CatalagManager::restoreStateOfFilterByKind()
{
    if(actionKinds.size() != kinds.size())
    {
        qDebug()<<"Error Kinds";
        return;
    }
    for(int index = 0 ;  index < actionKinds.size() ; index++)
    {
        actionKinds[index]->setChecked(kinds[index]->isChecked());
    }
}

void CatalagManager::restoreStateOfFilterByStatte()
{
    if(actionStates.size() != states.size())
    {
        qDebug()<<"Error States";
        return;
    }
    for(int index = 0 ;  index < actionStates.size() ; index++)
    {
        actionStates[index]->setChecked(states[index]->isChecked());
    }
}

QVector<QAction *> CatalagManager::getActionStates() const
{
    return actionStates;
}

void CatalagManager::setActionStates(const QVector<QAction *> &value)
{
    actionStates = value;
}

QVector<QAction *> CatalagManager::getActionKinds() const
{
    return actionKinds;
}

void CatalagManager::setActionKinds(const QVector<QAction *> &value)
{
    actionKinds = value;
}

QVector<QAction *> CatalagManager::getActionEmptyFields() const
{
    return actionEmptyFields;
}

void CatalagManager::setActionEmptyFields(const QVector<QAction *> &value)
{
    actionEmptyFields = value;
}

QVector<State *> CatalagManager::getStates() const
{
    return states;
}

QVector<Kind *> CatalagManager::getKinds() const
{
    return kinds;
}

QVector<Anime *> CatalagManager::getAnimes() const
{
    return animes;
}

