#ifndef KINDMANAGER_H
#define KINDMANAGER_H


#include <Catalago/Utils/display.h>
#include <Catalago/Controller/databasemanager.h>
#include <framelesswindow/framelesswindow.h>
#include <Catalago/Model/kind.h>
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

class Catalag;

class KindManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit KindManager( QWidget *parent = 0);

public slots:
    void changeVisibleStateList(bool);
    void updateInfoKind(QListWidgetItem *);

    void addKind();
    void editKind();
    void deleteKind();


    void nextKind();
    void backKind();
    void updateListKind(Kind *pKind);

private:


    //setting QToolBar
    void loadPhotoIcon();
    void loadTextIcon();
    void loadStatusTip();
    void loadToolTip();
    void createContextMenuByListKind();

    void setupWindow();
    void init();

    void createToolBar();
    void createStatusBar();


    void loadKinds();

    void createCentralWigdet();
    void createDockList();
    void connectSignals();

    void updateInfo( QString );
    void loadFirstKinds();



    //QGroupBox
    QGroupBox *createGroupBoxInfo();
    QGroupBox *createGroupBoxName();
    QGroupBox *createGroupBoxDescription();

    QHBoxLayout * createButtons();

    //QAction
    QAction  * addKindAct;
    QAction  * editKindAct;
    QAction  * deleteKindAct;
    QAction  * showOrHideList;
    QAction  * closeWindow;


    //QPushButton
    QPushButton * back;
    QPushButton * next;

    QDockWidget * dockList;

    QLabel * name;
    QLabel * description;
    QListWidget * listKind;


    CatalagManager * catalagManager;


};

#endif
