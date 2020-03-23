#include "kindchartpie.h"

KindChartPie::KindChartPie(QWidget *parent) : QWidget(parent)
{
    buttonFlat = -1;
    loadValuesForSerie();
    createPieSerie();
    createButtons();
    createChart();
    createChartView();
    connectSignals();
}

void KindChartPie::setChartTheme(QChart::ChartTheme Theme)
{
    chart->setTheme(Theme);
}

void KindChartPie::createPieSerie()
{
    QList < QPair < QString , double > > values;

    for(int index = 0 ; index < KindByLetter.size();index++)
    {
        if(KindByLetter[index].empty())
            continue;

        values = KindByLetter[index];
        buttonFlat = index;
        break;
    }


    pieSeries = new QPieSeries();
    pieSeries->setHoleSize(0.35);
    for(QPair < QString , double > value : values )
        pieSeries->append(value.first,value.second);

    connectSignalsSlice();
}

void KindChartPie::createChart()
{
    int LETTER_SIZE = 20;
    QString LETTER_TYPE = "Times";
    
    chart = new QChart();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->addSeries(pieSeries);

    chart->legend()->setAlignment(Qt::AlignRight );
    chart->setTitle("Quantity of Anime By Kinds");
    chart->setTheme(QChart::ChartThemeDark);
    chart->setTitleFont(QFont(LETTER_TYPE, LETTER_SIZE, QFont::Bold ));
    
}

void KindChartPie::createChartView()
{
    QVBoxLayout * baseLayout = new  QVBoxLayout();
    
    QChartView *chartView = new QChartView(chart);
    chartView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    chartView->setRenderHint(  QPainter::Antialiasing);
    baseLayout->addLayout(letterLayout);
    baseLayout->addWidget(chartView);
    
    setLayout(baseLayout);
}

void KindChartPie::connectSignalsSlice()
{
    for(QPieSlice * slice :  pieSeries->slices() )
    {
        connect(slice , SIGNAL(hovered(bool)) , this ,SLOT(handleSlice(bool)));
    }
}

void KindChartPie::handleSlice(bool Exploded)
{
    QPieSlice * slice = qobject_cast < QPieSlice *>(sender());
    
    if(Exploded)
    {
        if( pieSeries->slices().size() != 1)
        slice->setExploded(true);
        slice->setLabelFont(QFont("Times", 8 , QFont::Bold ));
        slice->setLabelVisible();
        slice->setLabel(QString("%1:%2").arg(slice->label()).arg(slice->value()) );
        highlightSlice(slice);

    }
    else
    {
        slice->setExploded(false);
        slice->setLabelVisible(false);
        slice->setLabel((QString)slice->label().split(":").first());
        
        restorePenAndBrushToSlice(slice);
    }
}

void KindChartPie::handleLetter()
{
    QPushButton * button = qobject_cast<QPushButton *>(sender());
    button->setFlat(true);

    char Letter = letterFromButton(button);

    if(Letter - 'A' != buttonFlat )
    letters[buttonFlat]->setFlat(false);

    int posittion = Letter - 'A';
    buttonFlat = posittion;
    
    QList < QPair < QString , double > > values = KindByLetter[posittion];
    

    pieSeries->clear();

    for( QPair < QString , double > value : values )
        pieSeries->append(value.first,value.second);

    connectSignalsSlice();
}

char KindChartPie::letterFromButton(QPushButton * button)
{
    return button->text().at(0).toLatin1();
}

void KindChartPie::highlightSlice(QPieSlice *pieSlice)
{
    savePenAndBrushFromSlice(pieSlice);
    pieSlice->setPen(QPen(Qt::darkGreen, 2));
    pieSlice->setBrush(Qt::green);
    
}

void KindChartPie::savePenAndBrushFromSlice(QPieSlice *pieSlice)
{
    pen = pieSlice->pen();
    brush = pieSlice->brush();
}

void KindChartPie::restorePenAndBrushToSlice(QPieSlice *pieSlice)
{
    pieSlice->setPen(pen);
    pieSlice->setBrush(brush);
}

void KindChartPie::fillGrid(char startLetter, char endLetter, int row )
{
     QPushButton * button;

     for( char index = startLetter, col = 0 ;  index <= endLetter; index++ , col++)
     {
         button = new QPushButton( QString("%1").arg(index) );
         button->setFont(QFont("Times", 12 , QFont::Bold ));
         letters.append(button);

         if(KindByLetter[index - 'A'].empty())
             button->setEnabled(false);

         letterLayout->addWidget(button,row,col);
     }
}

void KindChartPie::createButtons()
{
   letterLayout = new QGridLayout();
   fillGrid('A','I',0);
   fillGrid('J','R',1);
   fillGrid('S','Z',2);
   if( buttonFlat != -1)
   letters[buttonFlat]->setFlat(true);
}

void KindChartPie::loadValuesForSerie()
{

    KindByLetter = DatabaseManager::valuesForKindsChartPie();
}

void KindChartPie::connectSignals()
{

    for(QPushButton * button :  letters )
    {
        connect(button , SIGNAL( pressed() ) , this , SLOT(handleLetter()));
    }

}


