#ifndef GALLERY_H
#define GALLERY_H

#include <QListWidget>

class Gallery : public QListWidget
{
    Q_OBJECT
public:
    explicit Gallery(QWidget *parent = 0);

signals:

public slots:

protected:
    virtual void resizeEvent(QResizeEvent * event);

};

#endif // GALLERY_H
