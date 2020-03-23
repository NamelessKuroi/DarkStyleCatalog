#include "reportgenerator.h"

#include <QDir>
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <QDomElement>
#include <QDomText>
#include <QStringList>
#include <QProcess>

#include<Catalago/Utils/processaddress.h>

ReportGenerator::ReportGenerator(QVector<Anime *>  animes, QObject *parent)
    : QObject(parent) , animes(animes)
{
    addressDestiny = qApp->applicationDirPath();
    beforeAnime =  nextAnime =  currentAnime = nullptr;
}
void ReportGenerator::toCreateDirForReport()
{
    QDir App(addressDestiny);
    App.mkdir("Report");
    App.cd("Report");
    App.mkdir("Photos");
    App.mkdir("Animes");
    QString source = ProcessAddress::getAddress("ReportGenerator/style.css");
    QString destiny = QString ("%1/Report/style.css")
            .arg(addressDestiny);

//    qDebug()<<"Source:"<<source;
//    qDebug()<<"Destiny:"<<destiny;

//    qDebug()<<QFile::copy(source,destiny);

    QFile::copy(source,destiny);

}


void ReportGenerator::generateReport()
{
    toCreateDirForReport();
    generateAnimesByInitialLetter();

    createIndex();

    QProgressDialog progress;
    progress.setCancelButtonText(tr("&Abort"));
    progress.setRange( 0 , animes.size() );
    progress.setWindowTitle(tr("Generating Report"));
    progress.setWindowModality(Qt::WindowModal);

    progress.show();
    
    for (int index = 0; index < animes.size(); index++) {
        progress.setValue(index);
        progress.setLabelText(tr("Number Anime Report Generated %1 of %2")
                              .arg(index).arg(animes.size()));
        qApp->processEvents();
        
        if (progress.wasCanceled())
            break;

        if(index - 1 > 0)
            beforeAnime = animes[index - 1];
        else
            beforeAnime = NULL;

        currentAnime = animes[index];

        if(index + 1 < animes.size())
            nextAnime = animes[index + 1];
        else
            nextAnime = NULL;

        AnimeToHtml();
    }

    QDir report(addressDestiny);
    report.cd("Report");
    QString name = report.absoluteFilePath("index.html").replace('/','\\');
    QProcess::execute(QString("explorer.exe %1")
                      .arg(name)
                      );


}


void ReportGenerator::createIndex()
{
    QString nameAnime = QString("%1/Report/%2")
            .arg(addressDestiny)
            .arg("index.html");



    QFile  file(nameAnime);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QDomDocument  indexHtml = createHtmlIndex();

    QTextStream out(&file);

    out<<indexHtml.toString().toUtf8();

    file.close();
}

void ReportGenerator::AnimeToHtml()
{

    QString nameAnime = QString("%1/Report/Animes/%2%3")
            .arg(addressDestiny)
            .arg(currentAnime->getIdAnime())
            .arg(".html");


    QFile  file(nameAnime);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QDomDocument  animeHtml = createHtmlAnime();

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out<<animeHtml.toString().toUtf8();

    file.close();

}



QDomDocument ReportGenerator::createHtmlIndex()
{
    QDomDocument doc;
    QDomElement html = doc.createElement("html");
    createHeadIndex(html , doc);
    createBodyIndex(html , doc);
    doc.appendChild(html);
    return doc;

}

QDomDocument  ReportGenerator::createHtmlAnime()
{
    QDomDocument doc;
    QDomElement html = doc.createElement("html");
    createHeadAnime(html , doc);
    createBodyAnime(html , doc);
    doc.appendChild(html);
    return doc;
}

void ReportGenerator::createHeadIndex(QDomElement &html, QDomDocument &doc)
{
    QDomElement head = doc.createElement("head");

    QList < QPair<QString,QString> > attributes;

    //meta
    attributes.append(QPair<QString,QString>("name","qrichtext"));
    attributes.append(QPair<QString,QString>("content","1"));

    createMeta(head , doc , attributes);

    attributes.clear();

    attributes.append(QPair<QString,QString>("http-equiv","Content-Type"));
    attributes.append(QPair<QString,QString>("content","text/html; charset=UTF-8"));

    createMeta(head , doc,attributes);

    attributes.clear();


    //link

    attributes.append(QPair<QString,QString>("href","style.css"));
    attributes.append(QPair<QString,QString>("rel","stylesheet"));

    createLink(head , doc , attributes);

    attributes.clear();

    createTitleHeadIndex(head,doc);

    html.appendChild(head);

}

void ReportGenerator::createHeadAnime(QDomElement & html, QDomDocument & doc)
{
    QDomElement head = doc.createElement("head");

    QList < QPair<QString,QString> > attributes;

    //meta
    attributes.append(QPair<QString,QString>("name","qrichtext"));
    attributes.append(QPair<QString,QString>("content","1"));

    createMeta(head , doc , attributes);

    attributes.clear();

    attributes.append(QPair<QString,QString>("http-equiv","Content-Type"));
    attributes.append(QPair<QString,QString>("content","text/html; charset=UTF-8"));

    createMeta(head, doc, attributes);

    attributes.clear();


    //link

    attributes.append(QPair<QString,QString>("href","../style.css"));
    attributes.append(QPair<QString,QString>("rel","stylesheet"));

    createLink(head , doc , attributes);

    attributes.clear();

    createTitleHeadAnime(head,doc);

    html.appendChild(head);
}



void ReportGenerator::createMeta(QDomElement & head , QDomDocument  & doc ,
                                 QList < QPair < QString, QString> > attributes )
{
    QDomElement meta = doc.createElement("meta");
    for(QPair<QString, QString> attribute : attributes)
        meta.setAttribute(attribute.first,attribute.second);
    head.appendChild(meta);
}

void ReportGenerator::createLink(QDomElement & head ,QDomDocument & doc ,
                                 QList < QPair < QString, QString> > attributes)
{
    QDomElement link = doc.createElement("link");
    for(QPair<QString, QString> attribute : attributes)
        link.setAttribute(attribute.first,attribute.second);
    head.appendChild(link);
}



void ReportGenerator::createTitleHeadIndex(QDomElement & head ,QDomDocument & doc)
{
    QDomElement title = doc.createElement("title");
    QDomText nameAnime = doc.createTextNode("Catalago Anime");
    title.appendChild(nameAnime);
    head.appendChild(title);
}

void ReportGenerator::createTitleHeadAnime(QDomElement & head ,QDomDocument & doc)
{
    QDomElement title = doc.createElement("title");
    QDomText nameAnime = doc.createTextNode(currentAnime->getTitle());
    title.appendChild(nameAnime);
    head.appendChild(title);
}

void ReportGenerator::createBodyIndex(QDomElement &html, QDomDocument &doc)
{
    QDomElement body = doc.createElement("body");
    body.setAttribute("class","dictionary");
    createBlockIndex(body,doc);
    html.appendChild(body);
}

void ReportGenerator::createBodyAnime(QDomElement & html , QDomDocument  & doc)
{
    QDomElement body = doc.createElement("body");
    body.setAttribute("class","content");
    createBlockAnime(body,doc);

    if(beforeAnime == NULL )
        createButtons(body , doc , ONLY_NEXT);
    else
        if(nextAnime == NULL )
            createButtons(body , doc , ONLY_BACK);
        else
            createButtons(body,doc);

    html.appendChild(body);

}

void ReportGenerator::createBlockIndex(QDomElement &body, QDomDocument &doc)
{
    QDomElement block = doc.createElement("div");
    for( QChar letter:AnimesByInitialLetter.keys())
    {
        createIndexByInitialLetter(block ,  doc , letter);
    }
    body.appendChild(block);
}

void ReportGenerator::createBlockAnime(QDomElement &body, QDomDocument &doc)
{
    QDomElement block = doc.createElement("div");
    createTitleAnime(block,doc);
    createPhotoAnime(block,doc);
    createSynopsisAnime(block,doc);
    createKindsAnime(block,doc);
    createExtrasAnime(block,doc);

    body.appendChild(block);
}

void ReportGenerator::createIndexByInitialLetter(QDomElement &block, QDomDocument &doc
                                                 , QChar letter)
{
    QDomElement header = doc.createElement("h1");
    header.setAttribute("class","initialLetter");
    header.appendChild(doc.createTextNode(QString("%1").arg(letter)));
    block.appendChild(header);

    QDomElement list = doc.createElement("ul");
    QString reference;
    QDomElement item;
    QDomElement link;
    QVector<Anime * > temp = AnimesByInitialLetter[letter];
    for(Anime * anime : temp )
    {
        reference = QString("Animes/%1.html")
                .arg(anime->getIdAnime());


        item = doc.createElement("li");
        link = doc.createElement("a");

        link.setAttribute("href",reference);
        link.appendChild(doc.createTextNode(anime->getTitle()));

        item.appendChild(link);
        list.appendChild(item);
    }

    block.appendChild(list);

}



void ReportGenerator::createTitleAnime(QDomElement &block, QDomDocument &doc)
{
    QDomElement header = doc.createElement("h1");
    header.appendChild(doc.createTextNode(currentAnime->getTitle()));
    block.appendChild(header);
}

void ReportGenerator::createPhotoAnime(QDomElement &block, QDomDocument &doc)
{

    QString source = ProcessAddress::getAddress(currentAnime->getAddressPhoto());
    QString destiny = QString ("%1/Report/%2")
            .arg(addressDestiny)
            .arg(currentAnime->getAddressPhoto());

    qDebug()<<"Source0:"<<currentAnime->getAddressPhoto();
    qDebug()<<"Source:"<<source;
    qDebug()<<"Destiny:"<<destiny;
    qDebug()<<QFile::copy(source,destiny);

    QDomElement photo = doc.createElement("img");
    photo.setAttribute("src","../" + currentAnime->getAddressPhoto());
    block.appendChild(photo);
}

void ReportGenerator::createSynopsisAnime(QDomElement &block, QDomDocument &doc)
{


    QStringList pieces = currentAnime->getSynopsis().split('\n');
    for(QString piece : pieces)
    {
        QDomElement synopsis = doc.createElement("p");
        synopsis.setAttribute("class","synopsis");
        synopsis.appendChild(doc.createTextNode(piece));
        block.appendChild(synopsis);
    }
}

void ReportGenerator::createKindsAnime(QDomElement &block, QDomDocument &doc)
{
    QDomElement kinds = doc.createElement("p");
    kinds.setAttribute("class","kinds");
    QString listKinds = "";

    QVector <Kind * > Kinds = currentAnime->getKinds();

    if(!Kinds.isEmpty())
        listKinds += Kinds[0]->getName();

    for( int index = 1 ; index < Kinds.size();index++ )
    {
        listKinds += ", ";
        listKinds += Kinds[index]->getName();
    }

    kinds.appendChild(doc.createTextNode(listKinds));

    block.appendChild(kinds);
}

void ReportGenerator::createExtrasAnime(QDomElement &block, QDomDocument &doc)
{


    QDomElement extras = doc.createElement("table");
    extras.setAttribute("class","extras");

    QDomElement row = doc.createElement("tr");

    QDomElement state = doc.createElement("td");
    state.appendChild(doc.createTextNode(currentAnime->getState()));

    QDomElement chapter = doc.createElement("td");
    chapter.setAttribute("class","chapter");

    if(currentAnime->getChapter() == 0 )
        chapter.appendChild(doc.createTextNode(""));
    else
        chapter.appendChild(doc.createTextNode(QString::number(currentAnime->getChapter()) ));

    QDomElement year = doc.createElement("td");
    if(currentAnime->getYear() == 0 )
        year.appendChild(doc.createTextNode(""));
    else
        year.appendChild(doc.createTextNode(QString::number(currentAnime->getYear())));


    row.appendChild(state);
    row.appendChild(chapter);
    row.appendChild(year);

    extras.appendChild(row);

    block.appendChild(extras);
}

void ReportGenerator::createButtons(QDomElement &body, QDomDocument &doc,
                                    ReportGenerator::button state)
{
    QDomElement buttons = doc.createElement("table");

    buttons.setAttribute("class","buttons");

    QDomElement row = doc.createElement("tr");



    QDomElement back = doc.createElement("a");

    QDomElement backCol = doc.createElement("td");
    backCol.appendChild(back);

    QDomElement catalog = doc.createElement("a");
    catalog.setAttribute("class","catalog");
    catalog.setAttribute("href","../index.html");
    catalog.appendChild(doc.createTextNode("Catalog"));

    QDomElement catalogCol = doc.createElement("td");
    catalogCol.appendChild(catalog);


    QDomElement next = doc.createElement("a");
    QDomElement nextCol = doc.createElement("td");
    nextCol.appendChild(next);


    switch (state) {
    case ONLY_BACK:
    {
        back.setAttribute("class","back");
        next.setAttribute("class","blankNext");

        back.setAttribute("href",QString("%1.html")
                          .arg(beforeAnime->getIdAnime()));
        back.appendChild(doc.createTextNode("Back"));

    }

        break;
    case ONLY_NEXT:
    {
        back.setAttribute("class","blankBack");
        next.setAttribute("class","next");

        next.setAttribute("href",QString("%1.html")
                          .arg(nextAnime->getIdAnime()));
        next.appendChild(doc.createTextNode("Next"));

    }
        break;
    default:
    {
        back.appendChild(doc.createTextNode("Back"));
        next.appendChild(doc.createTextNode("Next"));


        back.setAttribute("class","back");
        next.setAttribute("class","next");

        back.setAttribute("href",QString("%1.html")
                          .arg(beforeAnime->getIdAnime()));

        next.setAttribute("href",QString("%1.html")
                          .arg(nextAnime->getIdAnime()));
    }
        break;
    }

    row.appendChild(backCol);
    row.appendChild(catalogCol);
    row.appendChild(nextCol);

    buttons.appendChild(row);



    body.appendChild(buttons);

}

QString ReportGenerator::getAddressDestiny() const
{
    return addressDestiny;
}

void ReportGenerator::setAddressDestiny(const QString &value)
{
    addressDestiny = value;
}

void ReportGenerator::generateAnimesByInitialLetter()
{

    for(Anime * anime : animes)
    {
        AnimesByInitialLetter[initialLetter(anime->getTitle())].append(anime);
    }
}

QChar ReportGenerator::initialLetter(QString word)
{
    return word.at(0);
}
