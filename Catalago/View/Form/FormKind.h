#ifndef FORMKIND_H
#define FORMKIND_H


#include <Catalago/Controller/databasemanager.h>
#include <Catalago/Model/kind.h>
#include <Catalago/Controller/catalagmanager.h>
#include <framelesswindow/framelesswindow.h>

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

class FormKind : public QWidget
{
    Q_OBJECT

public:
    explicit FormKind(QWidget *parent = 0 , Kind * pKind = nullptr);

signals:
    void closeParent();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
public slots:
    void acceptChange();
    void checkForm(QString pText);
    void connectSignals();


private:

    void initFormKind();
    void initNameKind();
    void fillFormKind();

    void addKind();
    void editKind();


    void changeName();
    void changeDescription();

    bool isModeEdit();
    void setupWindow();
    void createLayout();

    QGroupBox *createGroupBoxInfo();
    QGroupBox *createGroupBoxName();
    QGroupBox *createGroupBoxDescription();
    QHBoxLayout * createButtons();

    QLineEdit * name;
    QPlainTextEdit *description;

    Kind *  kindEdited;

    QPushButton * apply;
    QPushButton * cancel;
    CatalagManager * catalagManager;


};

#endif
