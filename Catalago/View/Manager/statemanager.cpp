#include "statemanager.h"
#include <Catalago/Utils/singleton.h>
#include <Catalago/View/Form/FormState.h>

StateManager::StateManager(QWidget *parent)
    : QMainWindow(parent)
{
    catalagManager = &Singleton<CatalagManager>::instance();
    setupWindow();
    init();
    loadStates();
    loadFirstState();
    createToolBar();
    createStatusBar();
    createCentralWigdet();
    createDockWidget();
    connectSignals();
}

void StateManager::setupWindow()
{
    //   setMinimumSize(600,600);
}

void StateManager::init()
{
    name = new QLabel();
    description = new QLabel();

    name->setFont(QFont("Times", 10, QFont::Bold));
    description->setFont(QFont("Times", 10, QFont::Bold));

    description->setAlignment(Qt::AlignJustify);
    description->setWordWrap(true);

    listState = new QListWidget();

    back = new QPushButton();
    next = new QPushButton();
}



void StateManager::updateInfoState(QListWidgetItem  * item )
{
    if(item != 0 )
        updateInfo(item->text());
    else
    {
        name->setText("N / A");
        description->setText("N / A");
    }
}

void StateManager::updateInfo(QString Name)
{
    State * temp = catalagManager->findStateByName(Name);
    name->setText(temp->getName());
    if(temp->getDescription().isEmpty())
        description->setText("N / A");
    else
        description->setText(temp->getDescription());

}

void StateManager::loadFirstState()
{
    if(listState->count() > 0)
    {
        listState->setCurrentRow(0);
        updateInfo(listState->currentItem()->text());
    }
}





void StateManager::addState()
{

    FramelessWindow  * frame  = new FramelessWindow();
    frame->setWindowTitle("Add State");
    frame->setWindowIcon(QIcon(ProcessAddress::getAddress("Icons/IconAdd.png")));

    FormState * formState = new FormState(frame);
    frame->setContent(formState);
    frame->show();

    formState->setFocus();

}

void StateManager::editState()
{
    if(listState->currentItem()== 0)
        return ;

    FramelessWindow  * frame  = new FramelessWindow();
    frame->setWindowTitle("Edit State");
    frame->setWindowIcon(QIcon(ProcessAddress::getAddress("Icons/IconEdit.png")));


    State* state = catalagManager->findStateByName(listState->currentItem()->text());

    FormState * formState = new FormState(frame , state);
    frame->setContent(formState);
    frame->show();
    formState->setFocus();
}



void StateManager::deleteState()
{
    QListWidgetItem * temp = listState->currentItem();
    if( temp == 0)
    {
        return;
    }
    QMessageBox  * msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setWindowTitle("Delete State");
    msgBox->setText(QString("The State \"%1\" is going to be deleted.").arg(temp->text()));
    msgBox->setInformativeText("Do you want to delete?");
    msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Ok);

    int ret = msgBox->exec();
    if ( ret == QMessageBox::Ok)
    {
        int IdState = catalagManager->findStateByName(temp->text())->getIdState();

        listState->takeItem(listState->currentRow());
        catalagManager->removeState(temp->text());
        DatabaseManager::deleteState(IdState);

        delete temp;
    }

    delete msgBox;
}



void StateManager::loadStates()
{
    for( State * state : catalagManager->getStates() )
        listState->addItem(state->getName());
}

void StateManager::createDockWidget()
{
    dockList = new QDockWidget(this);
    dockList->setFixedWidth(200);
    dockList->setFeatures(QDockWidget::DockWidgetClosable);
    dockList->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea);

    dockList->setWidget(listState);
    addDockWidget(Qt::RightDockWidgetArea, dockList);

}


void StateManager::createCentralWigdet()
{
    QWidget * central = new QWidget();

    central -> setContextMenuPolicy(Qt::ActionsContextMenu);

    central -> addAction(addStateAct);
    central -> addAction(editStateAct);
    central -> addAction(deleteStateAct);
    central -> addAction(showOrHideList);

    QVBoxLayout * centerLayout =  new QVBoxLayout();
    centerLayout->addWidget(createGroupBoxInfo());
    centerLayout->addLayout(createButtons());
    central->setLayout(centerLayout);

    setCentralWidget(central);

}

QGroupBox * StateManager::createGroupBoxInfo()
{
    QGroupBox * groupBox = new QGroupBox();
    QGridLayout * layoutInfo = new QGridLayout();
    layoutInfo->addWidget(createGroupBoxName(),0,0);
    layoutInfo->addWidget(createGroupBoxDescription(),1,0);
    groupBox->setLayout(layoutInfo);
    return  groupBox;
}

QGroupBox *StateManager::createGroupBoxName()
{
    QGroupBox * groupBox = new QGroupBox("Name");
    groupBox->setFixedHeight(70);
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(name);
    return groupBox;
}

QGroupBox *StateManager::createGroupBoxDescription()
{
    QGroupBox * groupBox = new QGroupBox("Description");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(description);
    return groupBox;

}

QHBoxLayout *StateManager::createButtons()
{
    QHBoxLayout * buttonLayout = new QHBoxLayout();
    back = new QPushButton("Previous");
    next = new QPushButton("Next");

    buttonLayout->addWidget(back);
    buttonLayout->addStretch();
    buttonLayout->addWidget(next);

    return buttonLayout;
}


void StateManager::nextState()
{
    int total = listState->count();
    if(total == 0 )
        return ;
    listState->setCurrentRow((listState->currentRow() + 1) % total);

}

void StateManager::backState()
{
    int total = listState->count();
    if(total == 0 )
        return ;
    listState->setCurrentRow((listState->currentRow() + total - 1) % total);

}

void StateManager::closeEvent(QCloseEvent *event )
{
    event->accept();
}


void StateManager::createToolBar()
{
    QToolBar *bar = new QToolBar();
    bar->setMovable(false);
    addToolBar( Qt::TopToolBarArea , bar);
    addStateAct  = new QAction();
    editStateAct  = new QAction();
    deleteStateAct  = new QAction();


    showOrHideList = new QAction();
    showOrHideList->setCheckable(true);
    showOrHideList->setChecked(true);

    loadPhotoIcon();
    loadTextIcon();
    loadStatusTip();
    loadToolTip();

    bar->addAction(showOrHideList);
    bar->addSeparator();
    bar->addAction(addStateAct);
    bar->addAction(editStateAct);
    bar->addAction(deleteStateAct);

    createContextMenuByListState();

}

void StateManager::loadPhotoIcon()
{
    addStateAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconAdd.png")));
    editStateAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconEdit.png")));
    deleteStateAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconDelete.png")));
    showOrHideList->setIcon(QIcon(  ProcessAddress::getAddress("Icons/IconList.png")));

}

void StateManager::loadTextIcon()
{
    addStateAct->setIconText("Add");
    editStateAct->setIconText("Edit");
    deleteStateAct->setIconText("Delete");
    showOrHideList->setIconText("Show/Hide");
}

void StateManager::loadStatusTip()
{
    addStateAct->setStatusTip("Agregar Un Nuevo State.");
    editStateAct->setStatusTip("Editar El State Selecionado.");
    deleteStateAct->setStatusTip("Eliminar El State Selecionado.");
    showOrHideList->setStatusTip("Mostar/Ocultar Listado de States");
}

void StateManager::loadToolTip()
{
    addStateAct->setToolTip("Agregar Un Nuevo State.");
    editStateAct->setToolTip("Editar El State Selecionado.");
    deleteStateAct->setToolTip("Eliminar El State Selecionado.");
    showOrHideList->setToolTip("Mostar/Ocultar Listado de States");
}
void StateManager::createContextMenuByListState()
{
    listState->setContextMenuPolicy(Qt::ActionsContextMenu);
    listState->addAction(addStateAct);
    listState->addAction(editStateAct);
    listState->addAction(deleteStateAct);
}


void StateManager::createStatusBar()
{
    setStatusBar( new QStatusBar());
}


void StateManager::changeVisibleStateList(bool active )
{
    dockList->setVisible(active);
}


void StateManager::connectSignals()
{
    connect(listState ,SIGNAL( itemClicked(QListWidgetItem* ) ) , this , SLOT(updateInfoState(QListWidgetItem *)));
    connect(listState,SIGNAL( currentItemChanged(QListWidgetItem*,QListWidgetItem*)) , this , SLOT(updateInfoState(QListWidgetItem *)));

    connect(back , SIGNAL(clicked(bool)) , this , SLOT(backState()));
    connect(next , SIGNAL(clicked(bool)) , this , SLOT( nextState()));
    connect(addStateAct , SIGNAL(triggered(bool)) , this ,SLOT(addState()));
    connect(editStateAct , SIGNAL(triggered(bool)) , this ,SLOT(editState()));
    connect(deleteStateAct , SIGNAL(triggered(bool)) , this ,SLOT(deleteState()));

    connect(showOrHideList , SIGNAL(toggled(bool) ),this , SLOT(changeVisibleStateList(bool)));
    connect(dockList,SIGNAL(visibilityChanged(bool)),showOrHideList,SLOT(setChecked(bool)));

    connect(catalagManager, SIGNAL(newStateAdded(State*)),this,SLOT(updateListState(State*)));
    connect(catalagManager, SIGNAL(nameStateWasChanged(State*)) , this , SLOT(updateListState(State*)));
    connect(catalagManager, SIGNAL(stateWasEdited(State*)) , this , SLOT(updateListState(State*)));

}



void StateManager::updateListState(State *pState)
{

    listState->clear();

    for ( State *  State : catalagManager->getStates()) {
        listState->addItem(State->getName());
    }
    int position = catalagManager->findIndexOfStateByName(pState->getName());
    listState->setCurrentRow(position);
    listState->scrollToItem(listState->currentItem());
}




