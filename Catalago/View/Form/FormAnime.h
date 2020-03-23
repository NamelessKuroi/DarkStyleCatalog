#ifndef FORMANIME_H
#define FORMANIME_H

#include <QDebug>

#include <Catalago/Utils/processaddress.h>
#include <Catalago/Utils/display.h>

#include <Catalago/Model/anime.h>

#include <Catalago/Controller/databasemanager.h>
#include <Catalago/Controller/catalagmanager.h>

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

#include <QFileDialog>
#include <QFile>

#include <QVector>
#include <QDate>

class FormAnime : public QWidget
{
    Q_OBJECT
public:
    explicit FormAnime( QWidget *parent = 0,Anime * pAnime = nullptr );
  signals:
    void closeParent();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

public slots:
    void acceptChange();
    void loadPhoto();
    void addKindChoosed(QString);
    void addKind();
    void EliminarKind();
    void checkForm(QString pText);
    void addAnime();
    void editAnime();
private:
     const int  INITIAL_YEAR = 1970;
     QString currentKind;

    void setupWindow();

    QString extractDirFromFile(QString pathfile);

    QGroupBox * createGroupBoxInfo();
    QGroupBox * createGroupBoxTitle();
    QGroupBox * createGroupBoxPhoto();
    QGroupBox * createGroupBoxSynopsis();
    QGroupBox * createGroupBoxGenero();
    QGroupBox * createGroupBoxState();
    QGroupBox * createGroupBoxChapter();
    QGroupBox * createGroupBoxYear();
    QHBoxLayout * createButtons();

    void createLayout();

    void connectSignals();

    void initFormAnime();
    void initTitleAnime();
    void initButtons();

    void fillFormAnime();


    void fillComboBox();
    void fillComboBoxState();
    void fillComboBoxYear();
    void fillComboBoxKind();


    void changeTitle();
    void changeState();
    void changeSynopsis();
    void changePhoto();
    void changeChapter();
    void changeYear();
    void changeKind();

    void addKindsToAnime(int IdAnime);

    bool isModeEdit();
    QString extractFileName ( QString );

    QLineEdit  * title;

    QPushButton * editPhoto;
    QLabel * photo;
    QString AddressPhoto;
    QPlainTextEdit * synopsis;

    QSpinBox * chapter;


    QComboBox * stateBox;
    QComboBox * yearBox;
    QComboBox * generoBox;

    QListWidget * generoList;

    QPushButton * apply;
    QPushButton * cancel;
    QPushButton * addkind;
    QPushButton * removekind;

    Anime * animeEdited;

    CatalagManager * catalagManager;



};


#endif
