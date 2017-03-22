#ifndef SET_H
#define SET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>

//Работа с сетью
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

//Работа с файлами
#include <QFile>
#include <QTextStream>
#include <QIODevice>

//База данных
#include <QtSql>
#include <QDebug>

class set : public QWidget
{
    Q_OBJECT

    QGridLayout* grid;

    //Интерфейс
    //Тестирование
    QPushButton * getSync;
    QListWidget * words;


    //Сеть
    QNetworkAccessManager* qnam;
    QNetworkReply *reply;
    
    QSqlDatabase dbase; //База данных


public:
    set(QWidget *parent = 0);
    ~set();

    QString getFormatString(QString);
    void getDataFromDatabases();

public slots:
    void startSync();
    void writeToFile();
    void syncWithServer();
};

#endif // SET_H