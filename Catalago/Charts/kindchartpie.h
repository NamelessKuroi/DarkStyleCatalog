#ifndef KINDCHARTPIE_H
#define KINDCHARTPIE_H

#include <QDebug>

#include <QWidget>

#include <Catalago/Controller/databasemanager.h>

#include <QChart>
#include <QPieSlice>
#include <QPieSeries>
#include <QChartView>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QPen>
#include <QBrush>

#include <QFont>

#include <QList>
#include <QVector>
#include <QPair>

#include <QPushButton>


using namespace QtCharts;


class KindChartPie : public QWidget
{
    Q_OBJECT
public:
    explicit KindChartPie(QWidget *parent = 0);
    void setChartTheme(QChart::ChartTheme Theme);
signals:

public slots:
    void handleSlice(bool Exploded);
    void handleLetter();
private:


    char letterFromButton(QPushButton * button);
    void highlightSlice(QPieSlice * pieSlice);
    void savePenAndBrushFromSlice(QPieSlice * pieSlice);
    void restorePenAndBrushToSlice(QPieSlice * pieSlice);

    void fillGrid(char startLetter, char endLetter , int row);
    void createButtons();
    
    void loadValuesForSerie();

    void createPieSerie();
    void createChart();
    void createChartView();

    void connectSignalsSlice();
    void connectSignals();

    QPieSeries * pieSeries;
    QChart *chart;
    QChartView * charView;

    int buttonFlat;
    QGridLayout * letterLayout;

    QPen pen;
    QBrush  brush;

    QVector < QPushButton * > letters;
    QVector < QList < QPair < QString , double > > >  KindByLetter;
};

#endif
