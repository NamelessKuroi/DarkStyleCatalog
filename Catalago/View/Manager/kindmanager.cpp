#include "kindmanager.h"
#include <QIcon>
#include <Catalago/Utils/singleton.h>
#include <Catalago/View/Form/FormKind.h>

KindManager::KindManager(QWidget *parent)
    : QMainWindow(parent)
{
    catalagManager = &Singleton<CatalagManager>::instance();
    setupWindow();
    init();
    loadKinds();
    loadFirstKinds();
    createToolBar();
    createStatusBar();
    createCentralWigdet();
    createDockList();
    connectSignals();
}

void KindManager::changeVisibleStateList(bool active )
{
    dockList->setVisible(active);
}

void KindManager::updateInfoKind(QListWidgetItem  * item )
{
    if(item != 0 )
        updateInfo(item->text());
    else
    {
        name->setText(NON_AVAILABLE);
        description->setText(NON_AVAILABLE);
    }
}
void KindManager::init()
{
    name = new QLabel();
    description = new QLabel();

    name->setFont(QFont("Times", 10, QFont::Bold));
    description->setFont(QFont("Times", 10, QFont::Bold));

    description->setAlignment(Qt::AlignJustify);
    description->setWordWrap(true);

    listKind = new QListWidget();

    back = new QPushButton();
    next = new QPushButton();
}

void KindManager::addKind()
{
    FramelessWindow  * frame  = new FramelessWindow();
    frame->setWindowTitle("Add Kind");
    frame->setWindowIcon(QIcon(ProcessAddress::getAddress("Icons/IconAdd.png")));
    FormKind * formKind = new FormKind(frame);
    frame->setContent(formKind);
    frame->show();
    formKind->setFocus();
}

void KindManager::editKind()
{
    if(listKind->currentItem() == 0)
        return ;

    FramelessWindow  * frame  = new FramelessWindow();
    frame->setWindowTitle("Edit Kind");
    frame->setWindowIcon(QIcon(ProcessAddress::getAddress("Icons/IconEdit.png")));

    Kind * kind = catalagManager->findKindByName(listKind->currentItem()->text());

    FormKind * formKind = new FormKind(frame , kind);
    frame->setContent(formKind);
    frame->show();
    formKind->setFocus();

}


void KindManager::deleteKind()
{
    QListWidgetItem * temp = listKind->currentItem();
    if( temp == 0)
    {
        return;
    }
    QMessageBox  * msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setWindowTitle("Delete Kind");
    msgBox->setText(QString("The Kind \"%1\" is going to be deleted.").arg(temp->text()));
    msgBox->setInformativeText("Do you want to delete?");
    msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Ok);

    int ret = msgBox->exec();
    if (ret == QMessageBox::Ok)
    {
        int IdKind = catalagManager->findKindByName(temp->text())->getIdKind();

        listKind->takeItem(listKind->currentRow());
        catalagManager->removeKind(temp->text());
        DatabaseManager::deleteKind(IdKind);
        catalagManager->updateAnimeByKinds();

        delete temp;
    }
    delete msgBox;
}


void KindManager::setupWindow()
{
    setMinimumSize(600,600);
}

void KindManager::loadKinds()
{
    for( Kind * kind : catalagManager->getKinds())
    {
        listKind->addItem(kind->getName());
    }
    if(listKind ->count() > 0 )
        listKind->setCurrentRow(0);
}

void KindManager::createDockList()
{
    dockList = new QDockWidget(this);
    dockList->setFixedWidth(200);
    dockList->setFeatures(QDockWidget::DockWidgetClosable);
    dockList->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea);

    dockList->setWidget(new QWidget());

    dockList->widget()->setLayout(new QVBoxLayout());
    dockList->widget()->layout()->addWidget(listKind);

    addDockWidget(Qt::RightDockWidgetArea, dockList);


}


void KindManager::createCentralWigdet()
{
    QWidget * central = new QWidget();

    central -> setContextMenuPolicy(Qt::ActionsContextMenu);

    central -> addAction(addKindAct);
    central -> addAction(editKindAct);
    central -> addAction(deleteKindAct);
    central -> addAction(showOrHideList);

    QVBoxLayout * centerLayout =  new QVBoxLayout();
    centerLayout->addWidget(createGroupBoxInfo());
    centerLayout->addLayout(createButtons());
    central->setLayout(centerLayout);

    setCentralWidget(central);


}

QGroupBox * KindManager::createGroupBoxInfo()
{
    QGroupBox * groupBox = new QGroupBox();
    QGridLayout * layoutInfo = new QGridLayout();
    layoutInfo->addWidget(createGroupBoxName(),0,0);
    layoutInfo->addWidget(createGroupBoxDescription(),1,0);
    groupBox->setLayout(layoutInfo);
    return  groupBox;
}

QGroupBox *KindManager::createGroupBoxName()
{
    QGroupBox * groupBox = new QGroupBox("Name");
    groupBox->setFixedHeight(70);
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(name);
    return groupBox;
}

QGroupBox *KindManager::createGroupBoxDescription()
{
    QGroupBox * groupBox = new QGroupBox("Description");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(description);
    return groupBox;

}

QHBoxLayout *KindManager::createButtons()
{
    QHBoxLayout * buttonLayout = new QHBoxLayout();
    back = new QPushButton("Previous");
    next = new QPushButton("Next");

    buttonLayout->addWidget(back);
    buttonLayout->addStretch();
    buttonLayout->addWidget(next);

    return buttonLayout;
}


void KindManager::nextKind()
{
    int total = listKind->count();
    if(total == 0 )
        return ;
    listKind->setCurrentRow((listKind->currentRow() + 1) % total);

}

void KindManager::backKind()
{
    int total = listKind->count();
    if(total == 0 )
        return ;
    listKind->setCurrentRow((listKind->currentRow() + total - 1) % total);

}



/*---------------------------------------------QToolBar---------------------------------------*/

void KindManager::createToolBar()
{
    QToolBar *bar = new QToolBar();
    bar->setMovable(false);
    addToolBar( Qt::TopToolBarArea , bar);

    addKindAct  = new QAction();
    editKindAct  = new QAction();
    deleteKindAct  = new QAction();


    showOrHideList = new QAction();
    showOrHideList->setCheckable(true);
    showOrHideList->setChecked(true);


    loadPhotoIcon();
    loadTextIcon();
    loadStatusTip();
    loadToolTip();


    bar->addAction(showOrHideList);
    bar->addSeparator();
    bar->addAction(addKindAct);
    bar->addAction(editKindAct);
    bar->addAction(deleteKindAct);


    createContextMenuByListKind();

}

void KindManager::loadPhotoIcon()
{
    addKindAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconAdd.png")));
    editKindAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconEdit.png")));
    deleteKindAct->setIcon(QIcon(ProcessAddress::getAddress("Icons/IconDelete.png")));
    showOrHideList->setIcon(QIcon(  ProcessAddress::getAddress("Icons/IconList.png")));

}

void KindManager::loadTextIcon()
{
    addKindAct->setIconText("Add");
    editKindAct->setIconText("Edit");
    deleteKindAct->setIconText("Delete");
    showOrHideList->setIconText("Show/Hide");

}

void KindManager::loadStatusTip()
{
    addKindAct->setStatusTip("Agregar Un Nuevo Kind.");
    editKindAct->setStatusTip("Editar El Kind Selecionado.");
    deleteKindAct->setStatusTip("Eliminar El Kind Selecionado.");
    showOrHideList->setStatusTip("Mostar/Ocultar Listado de Kinds");
}

void KindManager::loadToolTip()
{
    addKindAct->setToolTip("Agregar Un Nuevo Kind.");
    editKindAct->setToolTip("Editar El Kind Selecionado.");
    deleteKindAct->setToolTip("Eliminar El Kind Selecionado.");
    showOrHideList->setToolTip("Mostar/Ocultar Listado de Kinds");
}


void KindManager::createContextMenuByListKind()
{
    listKind->setContextMenuPolicy(Qt::ActionsContextMenu);
    listKind->addAction(addKindAct);
    listKind->addAction(editKindAct);
    listKind->addAction(deleteKindAct);
}

/*---------------------------------------------QToolBar---------------------------------------*/

void KindManager::createStatusBar()
{
    setStatusBar( new QStatusBar());
}

void KindManager::connectSignals()
{
    connect(listKind ,SIGNAL( itemClicked(QListWidgetItem* ) ) , this , SLOT(updateInfoKind(QListWidgetItem *)));
    connect(listKind,SIGNAL( currentItemChanged(QListWidgetItem*,QListWidgetItem*)) , this , SLOT(updateInfoKind(QListWidgetItem *)));

    connect(back , SIGNAL(clicked(bool)) , this , SLOT(backKind()));
    connect(next , SIGNAL(clicked(bool)) , this , SLOT( nextKind()));
    connect(addKindAct , SIGNAL(triggered(bool)) , this ,SLOT(addKind()));
    connect(editKindAct , SIGNAL(triggered(bool)) , this ,SLOT(editKind()));
    connect(deleteKindAct , SIGNAL(triggered(bool)) , this ,SLOT(deleteKind()));

    connect(showOrHideList , SIGNAL(toggled(bool) ),this , SLOT(changeVisibleStateList(bool)));
    connect(dockList,SIGNAL(visibilityChanged(bool)),showOrHideList,SLOT(setChecked(bool)));

    connect(catalagManager, SIGNAL(newKindAdded(Kind*)),this,SLOT(updateListKind(Kind*)));
    connect(catalagManager, SIGNAL(nameKindWasChanged(Kind*)) , this , SLOT(updateListKind(Kind*)));
    connect(catalagManager, SIGNAL(kindWasEdited(Kind*)) , this , SLOT(updateListKind(Kind*)));

}

void KindManager::updateInfo(QString Name)
{
    Kind * temp = catalagManager->findKindByName(Name);
    name->setText(temp->getName());
    if(temp->getDescription().isEmpty())
        description->setText(NON_AVAILABLE);
    else
        description->setText(temp->getDescription());

}

void KindManager::loadFirstKinds()
{
    if(listKind->count() > 0)
    {
        listKind->setCurrentRow(0);
        updateInfo(listKind->currentItem()->text());
    }

}

void KindManager::updateListKind(Kind *pKind)
{

    listKind->clear();

    for ( Kind *  kind : catalagManager->getKinds()) {
        listKind->addItem(kind->getName());
    }
    int position = catalagManager->findIndexOfKindByName(pKind->getName());
    listKind->setCurrentRow(position);
    listKind->scrollToItem(listKind->currentItem());
}


