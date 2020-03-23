//App
#include "catalag.h"

#include <Catalago/ReportGenerator/reportgenerator.h>
#include <Catalago/Utils/singleton.h>
#include <Catalago/View/Form/FormAnime.h>
#include <Catalago/View/Manager/toolcatalg.h>

//Qt
#include <QtConcurrent/QtConcurrent>

Catalag::Catalag(QWidget *parent) : QMainWindow(parent)
{
    DatabaseManager::startConnection();
    catalagManager = &Singleton<CatalagManager>::instance();
    imageScaling = new QFutureWatcher < QPair < QImage , QImage > > (this);
    loadAnimes();
    createInfoAnime();
    createToolBar();
    createDockList();
    createAnimeForm();
    connectSignals();
    createStatusBar();
    catalagManager->fillFilter();
    connectSignalsForFilter();
    loadFirstAnimeAvailable();

    filterByStateAct->setDisabled(true);
    filterByKindAct->setDisabled(true);
    filterByEmptyAct->setDisabled(true);

	toScalePhotos();
}


Catalag::~Catalag()
{
 imageScaling->cancel();
 imageScaling->waitForFinished();
}


void Catalag::finishedScaling()
{
    filterByStateAct->setEnabled(true);
    filterByKindAct->setEnabled(true);
    filterByEmptyAct->setEnabled(true);

}
void Catalag::toScalePhotos()
{

   QStringList files;
   for(Anime * anime : catalagManager -> getAnimes() )
   files.append(anime -> getAddressPhoto() );   
   imageScaling->setFuture(QtConcurrent::mapped(files,Display::scalePhotoAndThumbnail));
} 
void Catalag::loadPhotoAndThumbnail( int row )
{
    QPair < QImage, QImage >  cachePicture = imageScaling->resultAt(row);
	QImage pPhoto = cachePicture.first;
	QImage pThumbnail = cachePicture.second;
    if(listAnime->count() > row )
    listAnime->item(row) -> setData(Qt::DecorationRole , QPixmap::fromImage(pThumbnail));
    catalagManager->updatePhotoAndThumbnail( row , pPhoto, pThumbnail);
}
	
void Catalag::initListAnime()
{
    listAnime = new Gallery(this);
    listAnime->setGridSize(Display::GridSizeOfListAnime());
}

void Catalag::loadAnimes()
{

    initListAnime();
    int index = 0;
    for(Anime * anime : catalagManager->getAnimes())
    {
        listAnime->addItem(anime -> getTitle());
        listAnime->item(index) -> setData(Qt::DecorationRole ,anime->getThumbnail());
        index++;
    }
}

/*---------------------------------------------QToolBar---------------------------------------*/

void Catalag::createToolBar()
{
    bar = new QToolBar();
    bar->setMovable(false);
    addToolBar(Qt::TopToolBarArea , bar);


    addAnimeAct  = new QAction();
    editAnimeAct  = new QAction();
    deleteAnimeAct  = new QAction();

    kindManagerAct = new QAction();
    stateManagerAct = new QAction();

    showOrHideList = new QAction();
    showOrHideList->setCheckable(true);
    showOrHideList->setChecked(true);


    exportAct = new QAction();

    chartPieAct = new QAction("ChartPie");
    chartPieAct->setCheckable(true);
    chartPieAct->setChecked(false);

    filterByStateAct = new QAction("States");
    
    filterByKindAct =  new QAction("Kinds");
    filterByKindAct->setCheckable(true);
    filterByKindAct->setChecked(false);


    filterByEmptyAct = new QAction("Empty Fields");
    filterByEmptyAct->setCheckable(true);
    filterByEmptyAct->setChecked(false);

    toolCatalagAct = new QAction("Administration Catalg");


    createAndLoadMenu();

    QMenu * menuChartPie = new QMenu();

    stateChartPieAct = new QAction("State Chart Pie") ;
    kindChartPieAct = new QAction("Kind Chart Pie") ;


    menuChartPie->addAction(stateChartPieAct);
    menuChartPie->addAction(kindChartPieAct);

    chartPieAct->setMenu(menuChartPie);

    loadPhotoIcon();
    loadTextIcon();
    loadStatusTip();
    loadToolTip();

    bar->addAction(showOrHideList);
    bar->addSeparator();
    bar->addAction(addAnimeAct);
    bar->addAction(editAnimeAct);
    bar->addAction(deleteAnimeAct);
    bar->addSeparator();
    bar->addAction(kindManagerAct);
    bar->addAction(stateManagerAct);
    bar->addSeparator();
    bar->addAction(filterByStateAct);
    bar->addAction(filterByKindAct);
    bar->addAction(filterByEmptyAct);
    bar->addSeparator();
    bar->addAction(chartPieAct);
    bar->addAction(exportAct);
    bar->addAction(toolCatalagAct);

    //ContextMenu
    createContextMenuByListAnime();
}

void Catalag::createAndLoadMenu()
{
    filterByStateAct->setMenu(catalagManager->getStateMenu());
    filterByKindAct->setMenu(catalagManager->getKindMenu());
    filterByEmptyAct->setMenu(catalagManager->getEmptyFieldMenu());
}

void Catalag::loadPhotoIcon()
{
    addAnimeAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconAdd.png")));
    editAnimeAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconEdit.png")));
    deleteAnimeAct->setIcon(QIcon( ProcessAddress::getAddress("Icons/IconDelete.png")));

    showOrHideList->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconList.png")));
    //chartPieAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconKinds.png")));

    //Manager
    kindManagerAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconKinds.png")));
    stateManagerAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconState.png")));

    exportAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconReport.ico")));

}

void Catalag::loadTextIcon()
{
    addAnimeAct->setIconText("Add");
    editAnimeAct->setIconText("Edit");
    deleteAnimeAct->setIconText("Delete");
    showOrHideList->setIconText("Show/Hide");
}

void Catalag::loadStatusTip()
{
    addAnimeAct->setStatusTip("Agregar Un Nuevo Anime.");
    editAnimeAct->setStatusTip("Editar El Anime Selecionado.");
    deleteAnimeAct->setStatusTip("Eliminar El Anime Selecionado.");
    showOrHideList->setStatusTip("Mostar/Ocultar Listado de Animes");
    chartPieAct->setStatusTip("Mostar/Ocultar ChartPie de States");
    kindManagerAct->setStatusTip("Gestionar Los Generos de los Animes");
    filterByStateAct->setStatusTip("Filter for State");
    filterByKindAct->setStatusTip("Filter for Kind");
    filterByEmptyAct->setStatusTip("Filter for Empty Field");
    exportAct->setStatusTip("Create a Report of Animes");

}

void Catalag::loadToolTip()
{
    addAnimeAct->setToolTip("Agregar Un Nuevo Anime.");
    editAnimeAct->setToolTip("Editar El Anime Selecionado.");
    deleteAnimeAct->setToolTip("Eliminar El Anime Selecionado.");
    showOrHideList->setToolTip("Mostar/Ocultar Listado de Animes");
    chartPieAct->setToolTip("Mostar/Ocultar ChartPie de States");
    kindManagerAct->setToolTip("Gestionar Los Generos de los Animes");
    filterByStateAct->setToolTip("Filter for State");
    filterByKindAct->setToolTip("Filter for Kind");
    filterByEmptyAct->setToolTip("Filter for Empty Field");
    exportAct->setToolTip("Create a Report of Animes");

}

void Catalag::createContextMenuByListAnime()
{
    listAnime->setContextMenuPolicy(Qt::ActionsContextMenu);
    listAnime->addAction(addAnimeAct);
    listAnime->addAction(editAnimeAct);
    listAnime->addAction(deleteAnimeAct);
}

/*---------------------------------------------QToolBar---------------------------------------*/

void Catalag::createInfoAnime()
{
    for (int index = 0; index < INFO_QUANTITY ; ++index) {
        infoAnime[index]  = new QLabel();
        infoAnime[index] -> setFont( Display::Typography() );
        infoAnime[index] -> setWordWrap(true);
        infoAnime[index] -> setAlignment(Qt::AlignJustify);
    }
    infoAnime[Anime::KIND]  -> setTextFormat(Qt::RichText);
    infoAnime[Anime::PHOTO] -> setAlignment(Qt::AlignCenter);
    infoAnime[Anime::PHOTO] -> setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}


void Catalag::createDockList()
{
    dockList = new QDockWidget(this);
    dockList->setFeatures(QDockWidget::DockWidgetClosable);
    dockList->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockList->setWidget(listAnime);
    addDockWidget(Qt::RightDockWidgetArea, dockList);
}


void Catalag::createAnimeForm()
{
    formAnime = new QWidget();

    dockCentral  = new QDockWidget();
    dockCentral -> setFeatures(QDockWidget::DockWidgetClosable);
    dockCentral -> setContextMenuPolicy(Qt::ActionsContextMenu);

    dockCentral -> addAction(addAnimeAct);
    dockCentral -> addAction(editAnimeAct);
    dockCentral -> addAction(deleteAnimeAct);
    dockCentral -> addAction(showOrHideList);

    QVBoxLayout  *  centerLayout =  new QVBoxLayout();
    centerLayout -> addWidget(createGroupBoxInfo());
    centerLayout -> addLayout(createButtons());

    formAnime->setLayout(centerLayout);
    dockCentral->setWidget(formAnime);
    setCentralWidget(dockCentral);
}


void Catalag::updateDockCentral(QListWidgetItem * pItem)
{
    if(pItem == nullptr)
    {
        emptyAnime();
        return;
    }

    Anime * temp = catalagManager->findAnimeByTitle(pItem->text());

    infoAnime[Anime::TITLE]->setText(temp->getTitle());
    int widhtPhoto  = Display::WidthPhoto();
    int heightPhoto = Display::HeightPhoto();
    infoAnime[Anime::PHOTO]->setFixedSize(widhtPhoto , heightPhoto);
    infoAnime[Anime::PHOTO]->setPixmap(temp->getPhoto());

    if(!temp->getSynopsis().isEmpty())
        infoAnime[Anime::SYNOPSIS]->setText(temp->getSynopsis());
    else
        infoAnime[Anime::SYNOPSIS]->setText(NON_AVAILABLE);


    infoAnime[Anime::STATE]->setText(temp->getState());

    if(temp->getChapter()!= 0)
        infoAnime[Anime::CHAPTER]->setText(QString::number(temp->getChapter()));
    else
        infoAnime[Anime::CHAPTER]->setText(NON_AVAILABLE);

    if(temp->getYear()!= 0)
        infoAnime[Anime::YEAR]->setText(QString::number(temp->getYear()));
    else
        infoAnime[Anime::YEAR]->setText(NON_AVAILABLE);

    infoAnime[Anime::KIND]->setText(temp->getListKind());

    listAnime->scrollToItem(listAnime->currentItem());

}

void Catalag::emptyAnime()
{
    for (int index = 0; index < INFO_QUANTITY ; index++) {
        if(index == Anime::PHOTO)
        {
            infoAnime[index]->setPixmap(QPixmap());
            continue;
        }
        infoAnime[index]->setText(NON_AVAILABLE);
    }
}

bool Catalag::AnimeHaveKind(QVector<Kind * > animeKinds, QString name)
{
    for (Kind * kind : animeKinds ) {
        if(kind->getName() == name )
            return true;
    }
    return false;
}


void Catalag::updateAnimeForm(QListWidgetItem * item)
{
    updateDockCentral(item);
    dockCentral->setVisible(true);
}

void Catalag::backAnime()
{
    int All  = listAnime->count();
    if(All == 0) return;
    listAnime->setCurrentRow(  ( listAnime->currentRow() + All - 1 ) % All );
    updateAnimeForm(listAnime->currentItem());
}

void Catalag::nextAnime()
{
    int All  = listAnime->count();
    if(All == 0) return;
    listAnime->setCurrentRow( (listAnime->currentRow() + 1 ) % All );
    updateAnimeForm(listAnime->currentItem());
}

void Catalag::addAnime()
{

    FramelessWindow  * frame  = new FramelessWindow();
    frame->setWindowTitle("Add Anime");
    frame->setWindowIcon(QIcon(ProcessAddress::getAddress("Icons/IconAdd.png")));

    FormAnime *  formAnime = new FormAnime(frame);

    frame->setContent(formAnime);
    frame->show();
    formAnime->setFocus();
}

void Catalag::deleteAnime()
{

    QListWidgetItem * temp = listAnime->currentItem();
    if( temp == 0)
    {
        return;
    }
    QMessageBox  * msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setWindowTitle("Delete Anime");
    msgBox->setText(QString("The Anime \"%1\" is going to be deleted.").arg(temp->text()));
    msgBox->setInformativeText("Do you want to delete?");
    msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Ok);

    int ret = msgBox->exec();
    if ( ret == QMessageBox::Ok)
    {
        int IdAnime = catalagManager->findAnimeByTitle(temp->text())->getIdAnime();
        listAnime->takeItem(listAnime->currentRow());
        DatabaseManager::deleteAnime(IdAnime);
        catalagManager->removeAnime(temp->text());
        delete temp;
    }
    delete msgBox;
}

void Catalag::editAnime()
{

    if(listAnime->currentItem() == 0)
        return;

    FramelessWindow  * frame  = new FramelessWindow();
    frame->setWindowTitle("Edit Anime");
    frame->setWindowIcon(QIcon(ProcessAddress::getAddress("Icons/IconEdit.png")));

    Anime * anime = catalagManager->findAnimeByTitle(listAnime->currentItem()->text());
    FormAnime  * formAnime = new FormAnime( frame , anime)  ;

    frame->setContent(formAnime);
    frame->show();

    formAnime->setFocus();
}

void Catalag::kindManager()
{
    FramelessWindow  * frame  = new FramelessWindow();
    frame->setWindowTitle("Kind Manager");

    frame->setWindowIcon(QIcon(ProcessAddress::getAddress("Icons/IconKinds.png")));

    KindManager * kindmanager = new KindManager();
    frame->setContent(kindmanager);
    frame->show();
}

void Catalag::stateManager()
{
    FramelessWindow  * frame  = new FramelessWindow();
    frame->setWindowTitle("State Manager ");
    frame->setWindowIcon(QIcon(ProcessAddress::getAddress("Icons/IconState.png")));
    StateManager * statemanager = new StateManager(this);
    frame->setContent(statemanager);
    frame->show();

}

void Catalag::showToolTip(QString pName )
{
    Kind * kind = catalagManager->findKindByName(pName);
    if(kind == nullptr)
    {   infoAnime[Anime::KIND]->setToolTip("");
        return;
    }
    if( !kind->getDescription().isEmpty())
        infoAnime[Anime::KIND]->setToolTip(Display::FitToolTip(kind->getDescription()));
}

void Catalag::filterByStateAndKind()
{
    if(listAnime->currentItem() != nullptr)
        currentTitleAnime = listAnime->currentItem()->text();
    else
        currentTitleAnime = "";

    QStringList animesFiltered;
    listAnime->clear();

    bool FilterByEmptyActived = false;
    bool State , Kind, Empty;

    for(QAction * action : catalagManager->getActionEmptyFields())
    {
        FilterByEmptyActived |= action->isChecked();
    }

    for (Anime * anime : catalagManager->getAnimes())
    {
        State = false;
        Empty = !filterByEmptyAct->isChecked();
        Kind = !filterByKindAct->isChecked();

        for ( QAction *action : catalagManager->getActionStates() ) {
            if(action->isChecked() && anime->getState() == action->text())
                State = true;
        }

        for ( QAction *action : catalagManager->getActionKinds()) {
            if(action->isChecked())
            {
                if(!filterByKindAct->isChecked())
                    Kind &= AnimeHaveKind(anime->getKinds(),action->text());
                else
                    Kind |= AnimeHaveKind(anime->getKinds(),action->text());
            }

        }

        for(QAction * action : catalagManager->getActionEmptyFields())
        {
            if(action->isChecked() == false)
                continue;

            if(action->text() == "Chapter" )
            {
                if(!filterByEmptyAct->isChecked())
                    Empty &= anime->getChapter() == 0;
                else
                    Empty |= anime->getChapter() == 0;
            }

            else if(action->text() == "Synopsis" )
            {
                if(!filterByEmptyAct->isChecked())
                    Empty &= anime->getSynopsis() == "";
                else
                    Empty |= anime->getSynopsis() == "";
            }

            else if( action->text() == "Year" )
            {
                if(!filterByEmptyAct->isChecked())
                    Empty &= anime->getYear() == 0;
                else
                    Empty |= anime->getYear() == 0;
            }
            else if( action->text() == "Photo" )
            {
                if(!filterByEmptyAct->isChecked())
                    Empty &= anime->getAddressPhoto().contains("Photos/Non-Image.png");
                else
                    Empty |= anime->getAddressPhoto().contains("Photos/Non-Image.png");
            }
            else if(action->text() == "Kinds" )
            {
                if(!filterByEmptyAct->isChecked())
                    Empty &= anime->getKinds().isEmpty();
                else
                    Empty |= anime->getKinds().isEmpty();
            }
        }

        if(FilterByEmptyActived)
        {
            if(State && Kind && Empty)
                animesFiltered<<anime->getTitle();
        }
        else
        {
            if(State && Kind )
                animesFiltered<<anime->getTitle();
        }
    }

    qSort(animesFiltered.begin(),animesFiltered.end());

    listAnime->addItems(animesFiltered);

    int row = -1;
    Anime * anime;

    for (int index = 0; index < listAnime->count(); ++index) {

        anime = catalagManager->findAnimeByTitle(animesFiltered.at(index));
        listAnime -> item(index) -> setData(Qt::DecorationRole ,anime->getThumbnail());
        if(anime->getTitle() == currentTitleAnime)
            row = index;
    }

    if( listAnime->count() > 0 )
    {
        if(row == -1 )
            listAnime->setCurrentRow(0);
        else
            listAnime->setCurrentRow(row);

        emit(catalagManager->refreshForm());
    }
}
//-------------------------------------------updateData-------------------------------------------------------

void Catalag::updateInfomationShowed(Anime * pAnime)
{
    catalagManager->saveStateOfFilter();

    for(Anime * anime : catalagManager->getAnimes())
    {
        anime->setKinds(DatabaseManager::getKindsByAnime(anime->getIdAnime()));
    }
    catalagManager->restoreStateOfFilter();
    connectSignalsForFilter();

    filterByStateAndKind();

    QList< QListWidgetItem*  > founded = listAnime->findItems(pAnime->getTitle(),Qt::MatchExactly);

    QListWidgetItem * curtentItem = nullptr;
    if(!founded.isEmpty())
        curtentItem = founded.first();

    if(curtentItem != nullptr)
    {
        listAnime->setCurrentItem(curtentItem);
        listAnime->scrollToItem(curtentItem);
    }
    else if(listAnime->count() >  0)
    {
        listAnime->setCurrentRow(0);
    }

    emit(refreshAnimeForm());

}


//-------------------------------------------updateData-------------------------------------------------------

void Catalag::changeStateChartPie(bool Active)
{
    if( Active == false)
    {
        dockList->setVisible(true);
        showOrHideList->setEnabled(true);

        addAnimeAct->setEnabled(true);
        editAnimeAct->setEnabled(true);
        deleteAnimeAct->setEnabled(true);

        kindManagerAct->setEnabled(true);
        stateManagerAct->setEnabled(true);

        filterByStateAct->setEnabled(true);
        filterByKindAct->setEnabled(true);
        filterByEmptyAct->setEnabled(true);

        stateChartPieAct->setEnabled(true);
        kindChartPieAct->setEnabled(true);

        dockCentral->setWidget(formAnime);
    }
}

void Catalag::changeModeFilter(bool /*Active*/)
{
    filterByStateAndKind();
}

void Catalag::generateReport()
{
    QString addressDestiny = QFileDialog::getExistingDirectory(this, tr("Choose Directory"),
                                                               "",
                                                               QFileDialog::ShowDirsOnly
                                                               | QFileDialog::DontResolveSymlinks);
    if(!addressDestiny.isEmpty())
    {
        ReportGenerator report(catalagManager->getAnimes());
        report.setAddressDestiny(addressDestiny);
        report.generateReport();
    }
}

void Catalag::activeList(bool activedDockCentral )
{
    if( activedDockCentral )
    {
        connect(listAnime ,SIGNAL( itemClicked(QListWidgetItem*)) , this ,
                SLOT(updateAnimeForm(QListWidgetItem *)));
        connect(listAnime ,SIGNAL( currentItemChanged(QListWidgetItem*,QListWidgetItem*)) ,
                this , SLOT(updateAnimeForm(QListWidgetItem *)));
    }
    else
    {
        disconnect(listAnime ,SIGNAL(itemClicked(QListWidgetItem*)) , this ,
                   SLOT(updateAnimeForm(QListWidgetItem *)));
        disconnect(listAnime ,SIGNAL( currentItemChanged(QListWidgetItem*,QListWidgetItem*)) ,
                   this , SLOT(updateAnimeForm(QListWidgetItem *)));
        if(chartPieAct->isChecked())
        {
            chartPieAct->setChecked(false);
        }
    }
}

void Catalag::handleChartPie()
{
    QAction * chartPie = qobject_cast<QAction *>(sender());
    chartPie->setDisabled(true);

    if( chartPie->text() == "State Chart Pie" )
    {
        dockCentral->setWidget( new StateChartPie());
        dockCentral->widget()->setFocus();
        kindChartPieAct->setEnabled(true);
    }

    if( chartPie->text() == "Kind Chart Pie" )
    {
        dockCentral->setWidget( new KindChartPie());
        dockCentral->widget()->setFocus();
        stateChartPieAct->setEnabled(true);
    }

    dockCentral->setVisible(true);
    dockList->setVisible(false);
    showOrHideList->setEnabled(false);

    addAnimeAct->setEnabled(false);
    editAnimeAct->setEnabled(false);
    deleteAnimeAct->setEnabled(false);

    kindManagerAct->setEnabled(false);
    stateManagerAct->setEnabled(false);

    filterByStateAct->setEnabled(false);
    filterByKindAct->setEnabled(false);
    filterByEmptyAct->setEnabled(false);


    chartPieAct->setChecked(true);

}

void Catalag::updateFilter()
{
    catalagManager->saveStateOfFilter();
    catalagManager->restoreStateOfFilter();
    connectSignalsForFilter();

}

void Catalag::clearStates()
{
    for( QAction * action : catalagManager->getActionStates())
    {
        action->blockSignals(true);
        action->setChecked(false);
        action->blockSignals(false);
    }
    filterByStateAndKind();
}

void Catalag::showManagment()
{
    FramelessWindow  * frame  = new FramelessWindow();
    frame->setWindowTitle("Administration Catalag");
    //frame->setWindowIcon(QIcon(ProcessAddress::getAddress("Icons/IconAdd.png")));
    ToolCatalg *  toolCatalg = new ToolCatalg();
    frame->setContent(toolCatalg);
    frame->show();
    toolCatalg->setFocus();
}

void Catalag::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    switch(event->key())
    {
    case Qt::Key_F2:
        editAnime();
        break;
    case Qt::Key_F3:
        changeVisibleStateList(!dockList->isVisible());
        break;
    }
}

void Catalag::updateListAnime(Anime *pAnime)
{

    listAnime->clear();

    int index = 0;
    for (Anime *  anime : catalagManager->getAnimes()) {
        listAnime -> addItem(anime->getTitle());
        listAnime -> item(index) -> setData(Qt::DecorationRole ,anime->getThumbnail());
        index++;
    }

    updateInfomationShowed(pAnime);

}

void Catalag::refreshAnimeForm()
{
    updateDockCentral(listAnime->currentItem());
}


QGroupBox *Catalag::createGroupBoxInfo()
{
    QGroupBox * groupBox = new QGroupBox();
    QGridLayout * layoutInfo = new QGridLayout();
    layoutInfo->addWidget(createGroupBoxTitle(),0,0,1,3);
    layoutInfo->addWidget(createGroupBoxPhoto(),1,0);
    layoutInfo->addWidget(createGroupBoxSynopsis(),1,1,1,2);
    layoutInfo->addWidget(createGroupBoxGenero(),2,0,1,3);
    layoutInfo->addWidget(createGroupBoxState(),3,0);
    layoutInfo->addWidget(createGroupBoxChapter(),3,1);
    layoutInfo->addWidget(createGroupBoxYear(),3,2);

    groupBox->setLayout(layoutInfo);
    return  groupBox;
}

QGroupBox *Catalag::createGroupBoxTitle()
{
    QGroupBox * groupBox = new QGroupBox("Title");
    groupBox->setFixedHeight(70);
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(infoAnime[Anime::TITLE]);
    return groupBox;

}

QGroupBox *Catalag::createGroupBoxPhoto()
{
    QGroupBox * groupBox = new QGroupBox("Photo");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(infoAnime[Anime::PHOTO]);
    groupBox->layout()->setAlignment(Qt::AlignHCenter);
    return groupBox;

}

QGroupBox *Catalag::createGroupBoxSynopsis()
{
    QGroupBox * groupBox = new QGroupBox("Synopsis");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(infoAnime[Anime::SYNOPSIS]);
    return groupBox;

}

QGroupBox *Catalag::createGroupBoxGenero()
{
    QGroupBox * groupBox = new QGroupBox("Kinds");
    groupBox->setFixedHeight(90);
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(infoAnime[Anime::KIND]);
    return groupBox;
}

QGroupBox *Catalag::createGroupBoxState()
{
    QGroupBox * groupBox = new QGroupBox("State");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(infoAnime[Anime::STATE]);
    return groupBox;
}

QGroupBox *Catalag::createGroupBoxChapter()
{
    QGroupBox * groupBox = new QGroupBox("Chapters");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(infoAnime[Anime::CHAPTER]);
    return groupBox;
}

QGroupBox *Catalag::createGroupBoxYear()
{
    QGroupBox * groupBox = new QGroupBox("Year");
    groupBox->setFixedHeight(70);
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(infoAnime[Anime::YEAR]);
    return groupBox;
}

QHBoxLayout *Catalag::createButtons()
{
    QHBoxLayout * buttonLayout = new QHBoxLayout();
    back = new QPushButton("Previous");
    next = new QPushButton("Next");

    buttonLayout->addWidget(back);
    buttonLayout->addStretch();
    buttonLayout->addWidget(next);

    return buttonLayout;
}


void Catalag::connectSignals()
{
    connect(imageScaling, SIGNAL(resultReadyAt(int)), SLOT(loadPhotoAndThumbnail(int))); 
	connect(imageScaling, SIGNAL(finished()), SLOT(finishedScaling()));
    connect(listAnime, SIGNAL( itemDoubleClicked(QListWidgetItem*) ) , this , SLOT(updateAnimeForm(QListWidgetItem *)));

    connect(back, SIGNAL(clicked()), this, SLOT( backAnime() ) );
    connect(next, SIGNAL(clicked()), this , SLOT( nextAnime() ) );

    connect(addAnimeAct, SIGNAL(triggered(bool)), this, SLOT(addAnime()));
    connect(editAnimeAct, SIGNAL(triggered(bool)), this, SLOT(editAnime()));
    connect(deleteAnimeAct, SIGNAL(triggered(bool)), this, SLOT(deleteAnime()));

    connect(kindManagerAct, SIGNAL(triggered(bool)), this, SLOT(kindManager()));
    connect(stateManagerAct, SIGNAL(triggered(bool)), this, SLOT(stateManager()));

    connect(showOrHideList, SIGNAL(toggled(bool)), this, SLOT(changeVisibleStateList(bool)));
    connect(dockList, SIGNAL(visibilityChanged(bool)), showOrHideList, SLOT(setChecked(bool)));
    connect(dockCentral, SIGNAL(visibilityChanged(bool)), this, SLOT(activeList(bool)));

    connect(exportAct, SIGNAL(triggered(bool)), this, SLOT(generateReport()));
    connect(chartPieAct, SIGNAL(toggled(bool)), this, SLOT(changeStateChartPie(bool)));

    connect(filterByKindAct, SIGNAL(toggled(bool)), this, SLOT(changeModeFilter(bool)));
    connect(filterByEmptyAct, SIGNAL(toggled(bool)), this, SLOT(changeModeFilter(bool)));

    connect(stateChartPieAct , SIGNAL(triggered(bool)), this, SLOT(handleChartPie()));
    connect(kindChartPieAct , SIGNAL(triggered(bool)), this, SLOT(handleChartPie()));

    connect(infoAnime[Anime::KIND], SIGNAL(linkHovered(QString)), this, SLOT(showToolTip(QString)));

    connect(toolCatalagAct, SIGNAL(triggered(bool) ), this, SLOT( showManagment() ) );

    connect(catalagManager , SIGNAL(newAnimeAdded(Anime*)),this,SLOT(updateListAnime(Anime*)));
    connect(catalagManager , SIGNAL(titleAnimeWasChanged(Anime*)) , this , SLOT(updateListAnime(Anime*)));
    connect(catalagManager , SIGNAL(animeWasEdited(Anime*)) , this , SLOT(updateListAnime(Anime*)));
    connect(catalagManager , SIGNAL(refreshForm()) , this  , SLOT(refreshAnimeForm()));
    connect(catalagManager , SIGNAL(updateFilter()), this , SLOT(updateFilter()));


}

void Catalag::connectSignalsForFilter()
{
    for(QAction * action : catalagManager->getActionKinds())
        connect( action , SIGNAL(triggered(bool)) , this , SLOT(filterByStateAndKind()) );

    for(QAction * action : catalagManager->getActionStates())
        connect( action , SIGNAL(triggered(bool)) , this , SLOT(filterByStateAndKind()) );

    for(QAction * action : catalagManager->getActionEmptyFields())
        connect( action , SIGNAL(triggered(bool)) , this , SLOT(filterByStateAndKind()) );

    connectSignalClear();
}

void Catalag::connectSignalClear()
{
    connect(catalagManager->getClearState(),SIGNAL(triggered(bool)),this,SLOT(clearStates()));
}

void Catalag::loadFirstAnimeAvailable()
{
    if( listAnime->count() > 0)
    {
        listAnime->setCurrentRow(0);
        updateDockCentral(listAnime->currentItem());
    }
}

void Catalag::createStatusBar()
{
    setStatusBar( new QStatusBar());
}


void Catalag::changeVisibleStateList(bool active)
{
    dockList->setVisible(active);
}


