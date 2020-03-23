#ifndef STATECHARTPIE_H
#define STATECHARTPIE_H

#include <QWidget>

#include <Catalago/Controller/databasemanager.h>


#include <QChart>
#include <QPieSlice>
#include <QPieSeries>
#include <QChartView>

#include <QVBoxLayout>

#include <QPen>
#include <QBrush>

#include <QFont>
#include <QList>
#include <QPair>

#include<QDebug>

using namespace QtCharts;

class StateChartPie : public QWidget
{
    Q_OBJECT
public:
    explicit StateChartPie(QWidget *parent = 0);
    void setChartTheme(QChart::ChartTheme Theme);
signals:

public slots:
    void handleSlice(bool Exploded);    

protected:
    virtual void keyPressEvent(QKeyEvent *event);
private:
    void highlightSlice(QPieSlice * pieSlice);
    void savePenAndBrushFromSlice(QPieSlice * pieSlice);
    void restorePenAndBrushToSlice(QPieSlice * pieSlice);


    void createPieSerie();
    void createChart();
    void createChartView();
    void connectSignals();

    QPieSeries * pieSeries;
    QChart *chart;
    QChartView * charView;


    bool percentage;

    QString currentlabelSildeExploted;
    QPen pen;
    QBrush  brush;
};

#endif // STATECHARTPIE_H
