#ifndef CATALAG_H
#define CATALAG_H

//App
#include <Catalago/Utils/processaddress.h>
#include <Catalago/Utils/display.h>
#include <Catalago/Charts/statechartpie.h>
#include <Catalago/Charts/kindchartpie.h>
#include <Catalago/Controller/databasemanager.h>
#include <Catalago/View/Manager/kindmanager.h>
#include <Catalago/View/Manager/statemanager.h>
#include <Catalago/Controller/catalagmanager.h>
#include <Catalago/Model/anime.h>
#include <Catalago/View/Gallery/gallery.h>
#include <framelesswindow/framelesswindow.h>

//Qt
#include <QDebug>
#include <QMainWindow>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QGroupBox>
#include <QVector>
#include <QStringList>
#include <QtAlgorithms>
#include <QMenu>
#include <QPair>
#include <QImage>
#include <QFutureWatcher>

class Catalag : public QMainWindow
{
    Q_OBJECT
public:
    explicit Catalag(QWidget *parent = 0);
      ~Catalag();

private slots:

    void changeVisibleStateList(bool);
    void changeModeFilter(bool);
    void activeList(bool);
    void changeStateChartPie(bool Active);

    void backAnime();
    void nextAnime();

    void addAnime();
    void deleteAnime();
    void editAnime();
    void kindManager();
    void stateManager();

    void showToolTip(QString pName);

    void filterByStateAndKind();

    void updateAnimeForm(QListWidgetItem *);
    void updateInfomationShowed(Anime *pAnime);
    void updateListAnime(Anime *pAnime);
    void refreshAnimeForm();

    void generateReport();
    void handleChartPie();

    void updateFilter();
    void clearStates();

    void showManagment();
	
	
	 void finishedScaling();
     void loadPhotoAndThumbnail( int );
	 
protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:

    static constexpr int INFO_QUANTITY = 7;

    // Scaling
	void toScalePhotos();
	
	
    //setting fillFillter
    void saveStateOfFilter();
    void restoreStateOfFilter();

    //setting loadAnime
    void initListAnime();


    //setting QToolBar
    void loadPhotoIcon();
    void loadTextIcon();
    void loadStatusTip();
    void loadToolTip();
    void createAndLoadMenu();
    void createContextMenuByListAnime();


    //QGroupBox
    QGroupBox * createGroupBoxInfo();
    QGroupBox * createGroupBoxTitle();
    QGroupBox * createGroupBoxPhoto();
    QGroupBox * createGroupBoxSynopsis();
    QGroupBox * createGroupBoxGenero();
    QGroupBox * createGroupBoxState();
    QGroupBox * createGroupBoxChapter();
    QGroupBox * createGroupBoxYear();

    QHBoxLayout * createButtons();


    void createInfoAnime();
    void createAnimeForm();
    void createDockList();

    void createToolBar();
    void createStatusBar();

    void loadAnimes();
    void emptyAnime();
    void loadFirstAnimeAvailable();
    void updateDockCentral(QListWidgetItem *pItem );

    bool AnimeHaveKind(QVector<Kind *> , QString);

    void connectSignals();
    void connectSignalsForFilter();
    void connectSignalClear();



    //QDockWidget
    QDockWidget * dockList;
    QDockWidget * dockCentral;

    //QAction
    QAction  * addAnimeAct;
    QAction  * editAnimeAct;
    QAction  * deleteAnimeAct;

    QAction  * kindManagerAct;
    QAction  * stateManagerAct;

    QAction  * showOrHideList;
    QAction  * closeWindow;

    QAction  * filterByStateAct;
    QAction  * filterByKindAct;
    QAction  * filterByEmptyAct;

    QAction  * exportAct;
    QAction  * chartPieAct;
    QAction  * stateChartPieAct;
    QAction  * kindChartPieAct;

    QAction  * toolCatalagAct;


    //QToolBar
    QToolBar *bar;



    //QPushButton
    QPushButton * back;
    QPushButton * next;



    QLabel * infoAnime[7];

    QListWidget * listAnime;
    QWidget * formAnime;

    CatalagManager * catalagManager;
    QString   currentTitleAnime;
	
	QFutureWatcher < QPair < QImage , QImage > > *imageScaling;

};

#endif
