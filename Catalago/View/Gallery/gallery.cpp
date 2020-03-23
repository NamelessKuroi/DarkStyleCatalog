#include "gallery.h"
#include <QResizeEvent>
#include <QDebug>
#include <QListWidget>
#include <QFrame>
#include <QLabel>
Gallery::Gallery(QWidget *parent)
    : QListWidget(parent)
{
    setMovement(QListView::Static);
    setResizeMode(QListView::Adjust);
    setViewMode(QListView::IconMode);
    setWrapping(true);
}

void Gallery::resizeEvent(QResizeEvent *event)
{
    QListWidget::resizeEvent(event);
    if(currentItem() != 0 )
        scrollToItem(currentItem());

}



