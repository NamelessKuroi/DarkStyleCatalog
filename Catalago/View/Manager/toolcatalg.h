#ifndef TOOLCATALG_H
#define TOOLCATALG_H

//QT
#include <QDebug>
#include <QMainWindow>
#include <QDir>
class QListWidget;

class ToolCatalg : public QMainWindow
{
    Q_OBJECT
public:
    explicit ToolCatalg(QWidget *parent = 0);

signals:

public slots:
    void deleteFilePhoto();
protected:
    virtual void keyPressEvent(QKeyEvent *event);
private:
    void initData();
    void loadLoosePhoto();

    QStringList loadFilterByImageType();
    bool photoNotUsedByAnime(QString name);

    QString dirAddressFilePhoto;
    QDir * dirFilePhoto;
    QListWidget * listFile;
    QVector < QString > fileNames;

};

#endif // TOOLCATALG_H
