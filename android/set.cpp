#include "set.h"

set::set(QWidget *parent) : QWidget(parent)
{
    grid = new QGridLayout(this);

    getSync = new QPushButton(this);
    getSync->setText("Синхронизировать");

    words = new QListWidget(this);

    grid->addWidget(getSync, 0,0);
    grid->addWidget(words,1,0);

    setLayout(grid);

    QWidget::connect(getSync, SIGNAL (clicked()), this, SLOT (startSync()));


    qnam = new QNetworkAccessManager();
}

set::~set()
{

}

void set::startSync()
{
    QString url = "http://morlok1.esy.es/getData.php";
    reply = qnam->get(QNetworkRequest(QUrl(url)));
    QWidget::connect(reply, SIGNAL (finished()), this, SLOT (writeToFile()));
}

void set::writeToFile()
{
    QFile file("words.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QIODevice * content = static_cast<QIODevice*>(QObject::sender());
    file.write(content->readAll());
    file.close();
    syncWithServer();
}

void set::syncWithServer()
{
    QFile file("words.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString ruLine, enLine;
    while (!file.atEnd())
    {
        ruLine = file.readLine();
        if (ruLine.indexOf("rus:") != -1)
        {
            ruLine = getFormatString(ruLine);

            //Получаем английское слово
            enLine = file.readLine();
            while (enLine.indexOf("eng") == -1)
                enLine = file.readLine();
            enLine = getFormatString(enLine);
            words->addItem(ruLine + "---" + enLine);
        }
    }
}

QString set::getFormatString(QString str)
{
    str = str.right(str.length() - 4).trimmed();
    return str;
}
