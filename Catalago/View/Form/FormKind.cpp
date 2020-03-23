#include "formkind.h"
#include <QStringList>
#include <QCompleter>
#include <Catalago/Utils/singleton.h>
FormKind::FormKind(QWidget *parent, Kind *pKind)
    : QWidget(parent) , kindEdited(pKind)
{
    catalagManager = &Singleton < CatalagManager >::instance();
    initFormKind();
    fillFormKind();
    createLayout();
    connectSignals();
}

void FormKind::createLayout()
{
    QVBoxLayout * center = new QVBoxLayout();
    center->addWidget(createGroupBoxInfo());
    center->addLayout(createButtons());
    setLayout(center);
}


void FormKind::initFormKind()
{
    initNameKind();
    description = new QPlainTextEdit();
}

void FormKind::initNameKind()
{

    QStringList nameKinds;

    for(Kind *  kind: catalagManager->getKinds())
        nameKinds << kind->getName();

    name = new QLineEdit();
    name->setCompleter(new QCompleter(nameKinds,this));
    name->setClearButtonEnabled(true);
    name->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    name->setPlaceholderText("Enter Name");
}

void FormKind::fillFormKind()
{
    if(isModeEdit())
    {
        name->setText(kindEdited->getName());
        description->setPlainText(kindEdited->getDescription());
    }
}



QGroupBox * FormKind::createGroupBoxInfo()
{
    QGroupBox * groupBox = new QGroupBox();
    QGridLayout * layoutInfo = new QGridLayout();
    layoutInfo->addWidget(createGroupBoxName(),0,0);
    layoutInfo->addWidget(createGroupBoxDescription(),1,0);
    groupBox->setLayout(layoutInfo);
    return  groupBox;
}
QGroupBox * FormKind::createGroupBoxName()
{
    QGroupBox * groupBox = new QGroupBox("Name");
    groupBox->setFixedHeight(70);
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(name);
    return groupBox;

}
QGroupBox * FormKind::createGroupBoxDescription()
{
    QGroupBox * groupBox = new QGroupBox("Description");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(description);
    return groupBox;

}

QHBoxLayout * FormKind::createButtons()
{
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

    QHBoxLayout * buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(apply);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancel);

    return buttonLayout;

}

void FormKind::acceptChange()
{
    if(isModeEdit())
        editKind();
    else
        addKind();
}


void FormKind::connectSignals()
{
    connect(this ,SIGNAL( closeParent()) ,parentWidget(),SLOT(close()));
    connect(cancel, SIGNAL( clicked(bool) ) , parentWidget() ,SLOT(close()));
    connect(apply , SIGNAL(clicked(bool)),this , SLOT(acceptChange()));
    connect(name, SIGNAL(textChanged(QString)), this, SLOT(checkForm(QString)));
}

void FormKind::addKind()
{
    Kind * newKind = new Kind();

    newKind->setName(name->text());
    newKind->setDescription(description->document()->toPlainText());

    if( DatabaseManager::existKindWithName(newKind->getName()))
    {
        QMessageBox::critical(this,"Equal Name","Ya Existe Un Kind Con Ese Nombre.");
        return ;
    }

    DatabaseManager::addKind(newKind);
    catalagManager->appendKind(newKind);
    emit(catalagManager->updateFilter());
    emit(closeParent());

}

void FormKind::editKind()
{
    changeName();
    changeDescription();
    catalagManager->kindWasEdited(kindEdited);
    catalagManager->updateAnimeByKinds();
    emit( catalagManager->refreshForm());
    emit( closeParent());

}

void FormKind::checkForm(QString pText)
{
    if(pText.isEmpty())
        apply->setDisabled(true);
    else
        apply->setEnabled(true);
}

bool FormKind::isModeEdit()
{
    return kindEdited != nullptr;
}





void FormKind::changeName()
{
    if(name->isModified())
    {
        if( DatabaseManager::existKindWithName(name->text()))
        {
            QMessageBox::critical(this,"Equal Name","Ya Existe Un Kind Con Ese Nombre.");
            return;
        }

        kindEdited->setName(name->text());
        DatabaseManager::SaveNameKind(kindEdited->getIdKind(),name->text());
        catalagManager->updateKinds();
        emit ( catalagManager->nameKindWasChanged(kindEdited) );
        emit(catalagManager->updateFilter());
    }
}

void FormKind::changeDescription()
{
    if(description->document()->isModified())
    {
        kindEdited->setDescription(description->toPlainText());
        DatabaseManager::SaveDescriptionKind(kindEdited->getIdKind(),description->toPlainText());
    }
}


void FormKind::keyPressEvent(QKeyEvent *event)
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

