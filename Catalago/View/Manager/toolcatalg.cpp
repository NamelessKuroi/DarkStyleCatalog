#include "toolcatalg.h"

#include <Catalago/Utils/display.h>
#include <Catalago/View/Gallery/gallery.h>
#include <Catalago/Utils/processaddress.h>
#include <Catalago/Controller/databasemanager.h>

//Qt
#include <QDir>
#include <QPixmap>
#include <QImageReader>
#include <QMessageBox>
#include <QKeyEvent>

ToolCatalg::ToolCatalg(QWidget *parent) : QMainWindow(parent)
{
    initData();
    loadLoosePhoto();
}


void ToolCatalg::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    switch(event->key())
    {
    case Qt::Key_Delete:
        deleteFilePhoto();
        break;
    case Qt::Key_Escape:
        close();
        break;
    }
}

void ToolCatalg::initData()
{
    listFile = new Gallery();
    listFile->setGridSize(Display::GridSizeOfListAnime());
}

bool ToolCatalg::photoNotUsedByAnime(QString name)
{
    return !DatabaseManager::existPhotoWithName(name);
}

void ToolCatalg::loadLoosePhoto()
{

    int widthPhoto =Display::WidthPhotoOFItemList();
    int heightPhoto=Display::HeightPhotoOFItemList();

    dirAddressFilePhoto = ProcessAddress::getAddress("Photos");

    dirFilePhoto = new QDir(dirAddressFilePhoto);

    int index = 0;
    QPixmap temp;
    for ( QString fileName : dirFilePhoto->entryList( loadFilterByImageType() , QDir::Files)){
        if(photoNotUsedByAnime(fileName))
        {
             temp = QPixmap( dirFilePhoto->absoluteFilePath( fileName));
            listFile->addItem(fileName);
            listFile->item(index) -> setData(Qt::DecorationRole
                                             , Display::scaled(temp ,widthPhoto,heightPhoto));
            index++;
        }
    }

    setCentralWidget(listFile);
}

QStringList ToolCatalg::loadFilterByImageType()
{
    QStringList filterByImageType;
    for (QByteArray format : QImageReader::supportedImageFormats()) {
        filterByImageType += "*." + format;
    }
    return filterByImageType;
}


void ToolCatalg::deleteFilePhoto()
{

    QListWidgetItem * temp = listFile->currentItem();
    if( temp == nullptr)
        return;
    QMessageBox  * msgBox = new QMessageBox(this);
    msgBox->setIcon(QMessageBox::Question);
    msgBox->setWindowTitle("Delete Anime");
    msgBox->setText(QString("The Photo \"%1\" is going to be deleted.").arg(temp->text()));
    msgBox->setInformativeText("Do you want to delete?");
    msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Ok);

    int ret = msgBox->exec();
    if ( ret == QMessageBox::Ok)
    {
        qDebug()<< QFile::remove(dirFilePhoto->absoluteFilePath(listFile->currentItem()->text()));
        listFile->takeItem(listFile->currentRow());
        delete temp;
    }
    delete msgBox;

}
