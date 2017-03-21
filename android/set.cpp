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

    //Сеть
    qnam = new QNetworkAccessManager();

    //База данных
    dbase = QSqlDatabase::addDatabase("QSQLITE"); //Создаем базу данных

    dbase.setDatabaseName("extremist.sqlite");//Подключаемся
    if (!dbase.open()) //Открываем
        qDebug() << "No.";

    QSqlQuery a_query;

    QString s = "CREATE TABLE word_table ("
            "id integer PRIMARY KEY NOT NULL, "
            "enWord VARCHAR(255), "
            "ruWord VARCHAR(255), "
            "rating int(11)"
            ");"; //Создаем таблицу, если её еще не существует
    bool b = a_query.exec(s);
    if (!b) {
        qDebug() << "Таблица со словами уже существует";
    }

    getDataFromDatabases();
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
    QString s_insert = "INSERT INTO word_table (enWord, ruWord, rating) VALUES ('%1', '%2', %3);";
    QSqlQuery a_query;
    while (!file.atEnd())
    {
        ruLine = file.readLine();
        if (ruLine.indexOf("rus:") != -1)
        {
            ruLine = getFormatString(ruLine);
            qDebug() << ruLine;
            //Получаем английское слово
            enLine = file.readLine();
            while (enLine.indexOf("eng") == -1)
                enLine = file.readLine();
            enLine = getFormatString(enLine);
            qDebug() << enLine;
            //Заносим слова в базу данных
            QString s = s_insert.arg(enLine)
                    .arg(ruLine)
                    .arg(0);
            if (a_query.exec(s))
            {
                qDebug() << "Добавлена новая пара слов в базу данных:" + ruLine + "---"+enLine;
            }
            //words->addItem(ruLine + "---" + enLine);
        }
    }
    words->clear();
    getDataFromDatabases();
}

QString set::getFormatString(QString str)
{
    str = str.right(str.length() - 4).trimmed();
    return str;
}

void set::getDataFromDatabases()
{
    QString s = "SELECT * FROM word_table";
    QSqlQuery a_query;
    bool b = a_query.exec(s);
    if (!b)
        qDebug() << "Не пошло считываие. stopCharTable";
    QSqlRecord rec = a_query.record();
    QString res;
    while (a_query.next())
    {
        res = a_query.value(rec.indexOf("enWord")).toString() + "---";
        res += a_query.value(rec.indexOf("ruWord")).toString();
        words->addItem(res);
    }
}
