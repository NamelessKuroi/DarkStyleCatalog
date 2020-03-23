#ifndef FORMSTATE_H
#define FORMSTATE_H


#include <framelesswindow/framelesswindow.h>
#include <Catalago/Controller/databasemanager.h>
#include <Catalago/Model/state.h>
#include <Catalago/Controller/catalagmanager.h>

//QT
#include <QDebug>
#include <QWidget>
#include <QMessageBox>

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QListWidget>

#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>

#include <QVector>


class FormState : public QWidget
{
    Q_OBJECT

public:
    explicit FormState(QWidget *parent = 0 , State * pState = nullptr);

signals:
  void closeParent();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
public slots:
    void acceptChange();
    void checkForm(QString pText);
    void connectSignals();

private:

    void initFormState();
    void initNameState();
    void fillFormState();

    void addState();
    void editState();

    bool isModeEdit();

    void changeDescription();
    void changeName();

    void setupWindow();
    void createLayout();
    QGroupBox *createGroupBoxInfo();
    QGroupBox *createGroupBoxName();
    QGroupBox *createGroupBoxDescription();
    QHBoxLayout * createButtons();

    QLineEdit * name;
    QPlainTextEdit *description;

    QPushButton * apply;
    QPushButton * cancel;

    State * stateEdited;

    CatalagManager * catalagManager;


};

#endif
