#include "formstate.h"
#include <QStringList>
#include <QCompleter>

#include <Catalago/Utils/singleton.h>

FormState::FormState(QWidget *parent, State *pState)
    : QWidget(parent) , stateEdited(pState)
{
    catalagManager = &Singleton<CatalagManager>::instance();
    initFormState();
    fillFormState();
    createLayout();
    connectSignals();
}


void FormState::createLayout()
{
    QVBoxLayout * center = new QVBoxLayout();
    center->addWidget(createGroupBoxInfo());
    center->addLayout(createButtons());
    setLayout(center);
}


void FormState::initFormState()
{
    initNameState();
    description = new QPlainTextEdit();
}


void FormState::initNameState()
{
    QStringList nameStates;
    for(State * state :catalagManager->getStates())
        nameStates << state->getName();

    name = new QLineEdit();
    name->setCompleter(new QCompleter(nameStates,this));
    name->setClearButtonEnabled(true);
    name->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    name->setPlaceholderText("Enter State");
}

void FormState::fillFormState()
{
    if(isModeEdit())
    {
        name->setText(stateEdited->getName());
        description->setPlainText(stateEdited->getDescription());
    }

}

bool FormState::isModeEdit()
{
    return stateEdited != nullptr;
}



QGroupBox * FormState::createGroupBoxInfo()
{
    QGroupBox * groupBox = new QGroupBox();
    QGridLayout * layoutInfo = new QGridLayout();
    layoutInfo->addWidget(createGroupBoxName(),0,0);
    layoutInfo->addWidget(createGroupBoxDescription(),1,0);
    groupBox->setLayout(layoutInfo);
    return  groupBox;
}
QGroupBox * FormState::createGroupBoxName()
{
    QGroupBox * groupBox = new QGroupBox("Name");
    groupBox->setFixedHeight(70);
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(name);
    return groupBox;

}
QGroupBox * FormState::createGroupBoxDescription()
{
    QGroupBox * groupBox = new QGroupBox("Description");
    groupBox->setLayout(new QVBoxLayout());
    groupBox->layout()->addWidget(description);
    return groupBox;

}

QHBoxLayout * FormState::createButtons()
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

void FormState::acceptChange()
{
    if(isModeEdit())
        editState();
    else
        addState();

}

void FormState::checkForm(QString pText)
{
    if(pText.isEmpty())
        apply->setDisabled(true);
    else
        apply->setEnabled(true);
}

void FormState::addState()
{
    State * newState = new State();

    newState->setName(name->text());
    newState->setDescription(description->document()->toPlainText());

    if( DatabaseManager::existStateWithName(newState->getName()))
    {
        QMessageBox::critical(this,"Equal Name","Ya Existe Un State Con Ese Nombre.");
        return ;
    }

    DatabaseManager::addState(newState);
    catalagManager->appendState(newState);
}

void FormState::editState()
{
    changeName();
    changeDescription();
    emit(catalagManager->stateWasEdited(stateEdited));
    emit(closeParent());
}


void FormState::connectSignals()
{
    connect(this, SIGNAL( closeParent()) ,parentWidget(),SLOT(close()));
    connect(cancel, SIGNAL( clicked(bool)) , parentWidget() ,SLOT(close()));
    connect(apply, SIGNAL(clicked(bool)), this , SLOT(acceptChange()));
    connect(name, SIGNAL(textChanged(QString)), this, SLOT(checkForm(QString)));

}



void FormState::changeName()
{
    if(name->isModified())
    {
        if( DatabaseManager::existStateWithName(name->text()))
        {
            QMessageBox::critical(this,"Equal Name","Ya Existe Un State Con Ese Nombre.");
            return;
        }

        stateEdited->setName(name->text());
        DatabaseManager::SaveNameState(stateEdited->getIdState(),name->text());
        catalagManager->updateStates();
        emit(catalagManager->nameStateWasChanged(stateEdited));
    }
}

void FormState::changeDescription()
{
    if(description->document()->isModified())
    {
        stateEdited->setDescription(description->toPlainText());
        DatabaseManager::SaveDescriptionState(stateEdited->getIdState(),description->toPlainText());
    }
}



void FormState::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    switch (event->key()) {
    case Qt::Key_Escape :
        emit(cancel->clicked(true));
        break;
    case Qt::Key_Return :
    case Qt::Key_Enter :
        emit(apply->clicked(true));
        break;
    }
}

