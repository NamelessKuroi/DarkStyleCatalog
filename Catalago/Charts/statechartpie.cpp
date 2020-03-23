#include "statechartpie.h"
#include <QString>
StateChartPie::StateChartPie(QWidget *parent) : QWidget(parent)
{
    percentage = true;
    createPieSerie();
    createChart();
    createChartView();
    connectSignals();
}

void StateChartPie::setChartTheme(QChart::ChartTheme Theme)
{
    chart->setTheme(Theme);
}

void StateChartPie::createPieSerie()
{
    QList < QPair < QString , double > > values = DatabaseManager::valuesForStatesChartPie();

    pieSeries = new QPieSeries();
    for(QPair < QString , double > value : values )
        pieSeries->append(value.first,value.second);
    pieSeries->setHoleSize(0.35);
}

void StateChartPie::createChart()
{
    int LETTER_SIZE = 15;
    QString LETTER_TYPE = "Times";


    chart = new QChart();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->addSeries(pieSeries);
    chart->setTitle("Quantity of Anime By State");

    chart->legend()->setAlignment(Qt::AlignRight );
    chart->legend()->setShowToolTips(true);
    chart->legend()->setToolTip( QString("Anime Total: %1").arg(pieSeries->sum()));
    chart->legend()->show();

    chart->setTheme(QChart::ChartThemeDark);
    chart->setTitleFont(QFont(LETTER_TYPE, LETTER_SIZE, QFont::Bold ));

}

void StateChartPie::createChartView()
{
    QVBoxLayout * baseLayout = new  QVBoxLayout();
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    baseLayout->addWidget(chartView);
    setLayout(baseLayout);
}

void StateChartPie::handleSlice(bool Exploded)
{
    QPieSlice * slice = qobject_cast < QPieSlice *>(sender());

    if(Exploded)
    {
        slice->setExploded(true);
        slice->setLabelFont(QFont("Times", 12 , QFont::Bold ));
        slice->setLabelVisible();
        currentlabelSildeExploted = slice->label();

        if(percentage)
            slice->setLabel(QString("%1:%2%").arg(slice->label())
                            .arg(QString::number(slice->percentage() * 100, 'f', 2)));
        else
            slice->setLabel(QString("%1:%2").arg(slice->label()).arg(slice->value()) );

        highlightSlice(slice);
    }
    else
    {
        slice->setExploded(false);
        slice->setLabelVisible(false);
        slice->setLabel(currentlabelSildeExploted);
        restorePenAndBrushToSlice(slice);
    }
}

void StateChartPie::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    switch (event->key()) {
    case Qt::Key_V:
        percentage = false;
        break;

    case Qt::Key_P:
        percentage = true;
        break;
    }
}

void StateChartPie::highlightSlice(QPieSlice *pieSlice)
{
    savePenAndBrushFromSlice(pieSlice);
    pieSlice->setPen(QPen(Qt::darkGreen, 2));
    pieSlice->setBrush(Qt::green);

}

void StateChartPie::savePenAndBrushFromSlice(QPieSlice *pieSlice)
{
    pen = pieSlice->pen();
    brush = pieSlice->brush();
}

void StateChartPie::restorePenAndBrushToSlice(QPieSlice *pieSlice)
{
    pieSlice->setPen(pen);
    pieSlice->setBrush(brush);
}

void StateChartPie::connectSignals()
{
    for(QPieSlice * slice :  pieSeries->slices() )
    {
        connect(slice , SIGNAL(hovered(bool)) , this ,SLOT(handleSlice(bool)));
    }
}


