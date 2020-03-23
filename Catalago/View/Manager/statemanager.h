#ifndef STATEMANAGER_H
#define STATEMANAGER_H


#include <Catalago/Controller/databasemanager.h>
#include <framelesswindow/framelesswindow.h>
#include <Catalago/Model/state.h>
#include <Catalago/Controller/catalagmanager.h>

//QT
#include <QDebug>
#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QIcon>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QVector>


class StateManager : public QMainWindow
{
    Q_OBJECT
public:
    explicit StateManager(QWidget *parent = 0);

public slots:
    void changeVisibleStateList(bool);
    void updateInfoState(QListWidgetItem *);
    void addState();
    void editState();
    void deleteState();

    void nextState();
    void backState();

    void updateListState(State *pState);
protected:
    void closeEvent(QCloseEvent *event);
private:

    //setting  QToolBar
    void loadPhotoIcon();
    void loadTextIcon();
    void loadStatusTip();
    void loadToolTip();
    void createContextMenuByListState();

    void setupWindow();
    void init();

    void createToolBar();
    void createStatusBar();


    void loadStates();
    void createCentralWigdet();
    void createDockWidget();
    void connectSignals();

    void updateInfo( QString );
    void loadFirstState();




    QGroupBox *createGroupBoxInfo();
    QGroupBox *createGroupBoxName();
    QGroupBox *createGroupBoxDescription();
    QHBoxLayout * createButtons();

    //QAction
    QAction  * addStateAct;
    QAction  * editStateAct;
    QAction  * deleteStateAct;
    QAction  * showOrHideList;
    QAction  * closeWindow;


    //QPushButton
    QPushButton * back;
    QPushButton * next;

    QDockWidget * dockList;

    QLabel * name;
    QLabel * description;
    QListWidget * listState;

    CatalagManager * catalagManager;

};

#endif
