#include "formanime.h"
#include <QtAlgorithms>
#include <QStringList>
#include <QCompleter>

#include <Catalago/Utils/singleton.h>

bool AnimeHaveThatKind(QVector<Kind *> pKinds , Kind * pKind)
{
    for ( Kind *kind : pKinds) {
        if(kind->getIdKind() == pKind->getIdKind())
            return true;
    }
    return false;
}


FormAnime::FormAnime(QWidget *parent, Anime * pAnime )
    : QWidget(parent) , animeEdited(pAnime)
{
    catalagManager = &Singleton<CatalagManager>::instance();
    setupWindow();
    AddressPhoto = "";
    initFormAnime();
    fillFormAnime();
    fillComboBox();
    createLayout();
    connectSignals();

}


void FormAnime::setupWindow()
{
    setMaximumSize(800,600);
}

void FormAnime::createLayout()
{
    QVBoxLayout * center = new QVBoxLayout();
    center->addWidget(createGroupBoxInfo());
    center->addLayout(createButtons());
    setLayout(center);
}

void FormAnime::initFormAnime()
{

    initTitleAnime();
    initButtons();

    photo = new QLabel();
    synopsis =  new QPlainTextEdit() ;

    stateBox  = new QComboBox() ;
    yearBox = new QComboBox() ;
    generoBox = new QComboBox();
    generoBox->setInsertPolicy(QComboBox::InsertAlphabetically);

    chapter = new QSpinBox();
    chapter->setRange(0,3000);

    generoList = new QListWidget();

}

void FormAnime::initTitleAnime()
{
    QStringList nameAnimes;
    for(Anime * anime  :  catalagManager->getAnimes())
        nameAnimes << anime->getTitle();

    title = new QLineEdit();
    title->setCompleter(new QCompleter(nameAnimes , this));
    title->setClearButtonEnabled(true);
    title->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    title->setPlaceholderText("Enter Title");

}

void FormAnime::initButtons()
{
    addkind = new QPushButton("Add");
    removekind = new QPushButton("Remove");

}


QGroupBox * FormAnime::createGroupBoxInfo()
{
    QGroupBox * groupBox = new QGroupBox();
    QGridLayout * layoutInfo = new QGridLayout();
    QVBoxLayout * col = new QVBoxLayout();
    col->addWidget(createGroupBoxState());
    col->addWidget(createGroupBoxChapter());
    col->addWidget(createGroupBoxYear());
    layoutInfo->addWidget(createGroupBoxTitle(),0,0,1,2);
    layoutInfo->addWidget(createGroupBoxPhoto(),1,0);
    layoutInfo->addWidget(createGroupBoxGenero(),1,1);
    layoutInfo->addWidget(createGroupBoxSynopsis(),2,0);
    layoutInfo->addLayout(col,2,1);
    groupBox->setLayout(layoutInfo);
    return  groupBox;
}

QGroupBox *FormAnime::createGroupBoxTitle()
{
    QGroupBox * groupBox = new QGroupBox("Title");
    groupBox->setLayout(new QVBoxLayout());

    groupBox->layout()->addWidget(title);
    return groupBox;

}

QGroupBox *FormAnime::createGroupBoxPhoto()
{
    editPhoto = new QPushButton("Load Photo");
    editPhoto->setStatusTip("Cargar La Imagen");
    QGroupBox * groupBox = new QGroupBox("Photo");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(photo);
    groupBox->layout()->addWidget(editPhoto);
    groupBox->layout()->setAlignment(Qt::AlignHCenter);
    return groupBox;

}

QGroupBox *FormAnime::createGroupBoxSynopsis()
{
    QGroupBox * groupBox = new QGroupBox("Synopsis");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(synopsis);
    return groupBox;

}

QGroupBox *FormAnime::createGroupBoxGenero()
{
    QGridLayout *grid = new QGridLayout();
    QGroupBox * groupBox = new QGroupBox("Kinds");
    grid->addWidget(generoList,0,0,1,3);
    grid->addWidget(generoBox,1,0,1,3);
    grid->addWidget(addkind,2,0);
    grid->addWidget(removekind,2,2);
    groupBox->setLayout(grid);
    return groupBox;
}

QGroupBox *FormAnime::createGroupBoxState()
{
    QGroupBox * groupBox = new QGroupBox("State");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(stateBox);
    return groupBox;
}

QGroupBox *FormAnime::createGroupBoxChapter()
{
    QGroupBox * groupBox = new QGroupBox("Chapters");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(chapter);
    return groupBox;
}

QGroupBox *FormAnime::createGroupBoxYear()
{
    QGroupBox * groupBox = new QGroupBox("Year");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(yearBox);
    return groupBox;
}

QHBoxLayout *FormAnime::createButtons()
{
    QHBoxLayout * buttonLayout = new QHBoxLayout();
    if(isModeEdit())
    {
        apply = new QPushButton("Edit");
        apply->setEnabled(true);
    }
    else
    {
        apply = new QPushButton("Add");
        apply->setDisabled(true);
    }



    cancel = new QPushButton("Back");
    cancel->setStatusTip("Volver Al Catalago");

    buttonLayout->addWidget(apply);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancel);

    return buttonLayout;
}


void FormAnime::acceptChange()
{
    if(isModeEdit())
        editAnime();
    else
        addAnime();
}

void FormAnime::addAnime()
{
    QString newAddressPhoto ;
    int IdAnime;
    Anime * newAnime = new Anime() ;
    newAnime->setTitle(title->text());
    newAnime->setState(stateBox->currentText());
    newAnime->setChapter(chapter->value());
    newAnime->setSynopsis(synopsis->document()->toPlainText());
    newAnime->setYear(yearBox->currentText().toInt());

    if( DatabaseManager::existAnimeWithTitle(title->text()))
    {
        QMessageBox::critical(this,"There Is  Title","Ya Existe Un Anime Con Ese Title.");
        return;
    }


    if(AddressPhoto != "" )
    {
        QString recentDir = extractDirFromFile(AddressPhoto);
        catalagManager->setRecentDirectory(recentDir) ;

        newAddressPhoto = ProcessAddress::getAddress(QString("Photos/%1")
                                                     .arg(extractFileName(AddressPhoto))) ;
        QFile::copy(AddressPhoto , newAddressPhoto);

        newAnime -> setAddressPhoto(newAddressPhoto);
    }
    else
    {
        newAnime->setAddressPhoto(ProcessAddress::getAddress("Photos/Non-Image.png"));
    }

	newAnime -> updatePhotoAndThumbnail();

    IdAnime = DatabaseManager::addAnime(newAnime);
    addKindsToAnime(IdAnime);
    catalagManager->appendAnime(newAnime);
    emit(closeParent());

}

void FormAnime::editAnime()
{
    changeTitle();
    changeState();
    changeSynopsis();
    changePhoto();
    changeChapter();
    changeYear();
    changeKind();
    emit(catalagManager->animeWasEdited(animeEdited));
    emit(closeParent());

}


void FormAnime::loadPhoto()
{
    AddressPhoto = QFileDialog::getOpenFileName(this,
                                                tr("Load Image"),catalagManager->getRecentDirectory()
                                                , tr("Image Files (*.png *.jpg *.bmp)"));
    if( !AddressPhoto.isEmpty() )
    {
        QPixmap temp (AddressPhoto);
        photo->setPixmap(Display::scaled(temp,400,400) );
    }
}


void FormAnime::fillComboBox()
{
    fillComboBoxState();
    fillComboBoxYear();
    fillComboBoxKind();
}

void FormAnime::fillComboBoxState()
{
    stateBox->setInsertPolicy(QComboBox::InsertAlphabetically);
    for(State * state : DatabaseManager::getStates())
        stateBox->addItem(state->getName());

    int index;
    if(isModeEdit())
        index = stateBox->findText(animeEdited->getState());
    else
        index = stateBox->findText("Wanted");

    stateBox->setCurrentIndex(index);

}

void FormAnime::fillComboBoxYear()
{
    QStringList numberYear;

    numberYear.append(NON_AVAILABLE);

    int currentYear = QDate::currentDate().year();

    for (int value = currentYear; value >= INITIAL_YEAR ; value--) {
        numberYear.append(QString::number(value));
    }
    yearBox->addItems(numberYear);

    if(isModeEdit())
    {
        if( animeEdited->getYear() != 0)
            yearBox->setCurrentIndex(currentYear - animeEdited->getYear() + 1);
    }
    else
        yearBox->setCurrentIndex(0);

}

void FormAnime::fillComboBoxKind()
{
    QVector<Kind * > kindsOfAnime;
    if(isModeEdit())
        kindsOfAnime = DatabaseManager::getKindsByAnime(animeEdited->getIdAnime());

    for( Kind * kind :  DatabaseManager::getKinds())
    {
        if(AnimeHaveThatKind(kindsOfAnime,kind))
            generoList->addItem(kind->getName());
        else
            generoBox->addItem(kind->getName());
    }
    currentKind = generoBox->currentText();
}

QString FormAnime::extractFileName(QString fileName )
{
    return (QString)fileName.split("/").last();
}

void FormAnime::addKindChoosed(QString kindChoosed)
{
    if(generoBox->count() > 0)
    {
        if(kindChoosed != currentKind)
        {
            generoList->addItem(generoBox->currentText());
            generoBox->removeItem(generoBox->currentIndex());
            currentKind = generoBox->currentText();
        }

    }
}

void FormAnime::addKind()
{
    if(generoBox->count() > 0)
    {
        generoList->addItem(generoBox->currentText());
        generoBox->removeItem(generoBox->currentIndex());
        currentKind = generoBox->currentText();
    }
}

void FormAnime::EliminarKind()
{
    if ( generoList->currentItem()== 0)
        return;

    QStringList  data;
    for ( int  index = 0 ; index  <  generoBox->count() ; index++)
        data << generoBox->itemText(index);

    data << generoList->currentItem()->text();

    qSort(data.begin(),data.end());

    delete generoList->takeItem(generoList->currentRow());

    generoBox->clear();
    generoBox->addItems(data);
}

void FormAnime::checkForm(QString pText)
{
    if(pText.isEmpty())
        apply->setDisabled(true);
    else
        apply->setEnabled(true);

}


void FormAnime::addKindsToAnime(int IdAnime)
{
    int IdKind;
    DatabaseManager::RemoveKindsOfAnime(IdAnime);
    for (int index = 0; index < generoList->count(); ++index) {
        IdKind = catalagManager->findKindByName(generoList->item(index)->text())->getIdKind();
        DatabaseManager::AddKindToAnime(IdAnime,IdKind);
    }
}

bool FormAnime::isModeEdit()
{
    return animeEdited != nullptr;
}

QString FormAnime::extractDirFromFile(QString pathfile)
{
    int endDir = pathfile.lastIndexOf('/');
    return pathfile.remove(endDir , pathfile.size() - endDir );
}

void FormAnime::connectSignals()
{
    connect(this ,SIGNAL( closeParent()), parentWidget(), SLOT(close()));
    connect(cancel, SIGNAL( clicked(bool) ) , parentWidget() ,SLOT(close()));
    connect(apply , SIGNAL(clicked(bool)),this,SLOT(acceptChange()));
    connect(addkind, SIGNAL(clicked(bool) ) , this ,SLOT(addKind()));

    connect(generoBox, SIGNAL(activated(QString)), this, SLOT(addKindChoosed(QString)));

    connect(removekind, SIGNAL( clicked(bool) ) , this ,SLOT(EliminarKind()));
    connect(editPhoto , SIGNAL(clicked(bool)) ,this , SLOT(loadPhoto()));

    connect(title, SIGNAL(textChanged(QString)), this, SLOT(checkForm(QString)));

}

void FormAnime::fillFormAnime()
{
    if(isModeEdit())
    {
        QPixmap temp(animeEdited->getAddressPhoto() );
        title->setText(animeEdited->getTitle());
        photo->setPixmap(Display::scaled(temp, 400, 400 ) );
        synopsis->setPlainText(animeEdited->getSynopsis());
        chapter->setValue(animeEdited->getChapter());
    }
    else
    {
        QPixmap temp (ProcessAddress::getAddress("Photos/Non-Image.png"));
        photo->setPixmap(Display::scaled(temp, 400, 400 ) );

    }
}




void FormAnime::changeTitle()
{
    if(title->isModified())
    {
        if( DatabaseManager::existAnimeWithTitle(title->text()))
        {
            QMessageBox::critical(this,"Equal Title","Ya Existe Un Anime Con Ese Title.");
            return;
        }

        animeEdited->setTitle(title->text());
        DatabaseManager::SaveTitleAnime(animeEdited->getIdAnime(),title->text());
        catalagManager->updateAnimes();
        emit(catalagManager->titleAnimeWasChanged(animeEdited));
    }
}

void FormAnime::changeState()
{
    if( animeEdited->getState()!= stateBox->currentText())
    {
        animeEdited->setState(stateBox->currentText());
        DatabaseManager::SaveStateAnime(animeEdited->getIdAnime(),stateBox->currentText());
    }
}

void FormAnime::changeSynopsis()
{
    if( animeEdited->getSynopsis() != synopsis->document()->toPlainText() )
    {
        animeEdited->setSynopsis(synopsis->toPlainText());
        DatabaseManager::SaveSynopsisAnime(animeEdited->getIdAnime(),synopsis->toPlainText());
    }
}

void FormAnime::changePhoto()
{
    QString newAddressPhoto ;
    if(AddressPhoto != "" )
    {
        QString recentDir = extractDirFromFile(AddressPhoto);
        catalagManager->setRecentDirectory(recentDir) ;
        newAddressPhoto = ProcessAddress::getAddress(QString("Photos/%1").arg(extractFileName(AddressPhoto))) ;
        QFile::copy(AddressPhoto , newAddressPhoto);
        animeEdited -> setAddressPhoto(newAddressPhoto);
		animeEdited -> updatePhotoAndThumbnail();
        DatabaseManager::SavePhotoAnime(animeEdited->getIdAnime(),newAddressPhoto);
    }


}

void FormAnime::changeChapter()
{
    if ( chapter->value() != animeEdited->getChapter() )
    {
        animeEdited->setChapter(chapter->value());
        DatabaseManager::SaveChapterAnime(animeEdited->getIdAnime() , chapter->value());
    }

}

void FormAnime::changeYear()
{
    if(yearBox->currentText().toInt() != animeEdited->getYear() )
    {
        animeEdited->setYear(yearBox->currentText().toInt());
        DatabaseManager::SaveYearAnime(animeEdited->getIdAnime(),yearBox->currentText().toInt());
    }
}

void FormAnime::changeKind()
{
    int IdKind;
    DatabaseManager::RemoveKindsOfAnime(animeEdited->getIdAnime());
    for (int index = 0; index < generoList->count(); index++) {
        IdKind = catalagManager->findKindByName(generoList->item(index)->text())->getIdKind();
        DatabaseManager::AddKindToAnime(animeEdited->getIdAnime(),IdKind);
        animeEdited->setKinds(DatabaseManager::getKindsByAnime(animeEdited->getIdAnime()));

    }
}



void FormAnime::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);

    switch (event->key()) {
    case Qt::Key_Escape:
        emit(cancel->clicked(true));
        break;
    case Qt::Key_Return :
    case Qt::Key_Enter :
        emit(apply->clicked(true));
        break;
    }
}
