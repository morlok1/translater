#ifndef SET_H
#define SET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

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
#include <ctime>

class set : public QWidget
{
    Q_OBJECT

    QGridLayout* grid;

    //Интерфейс
    //Тестирование
    QPushButton * getSync;
    //QListWidget * words;
    QPushButton * getWord;
    QLabel * word;

    QPushButton* nextWord;
    QPushButton* getTranslate;
    QLabel* testProgress;

    //Сеть
    QNetworkAccessManager* qnam;
    QNetworkReply *reply;
    
    QSqlDatabase dbase; //База данных


    //Данные
    QString enWord;
    QString ruWord;
    int numbOfWord;
    int state;
    QMap<QString, QString> testWord;
public:
    set(QWidget *parent = 0);
    ~set();

    QString getFormatString(QString);
    void getDataFromDatabases();
    void getTestInterface();
    void removeTestInterface();

public slots:
    void startSync();
    void writeToFile();
    void syncWithServer();
    void startTest();
    void getWordAction();
    void getTranslateAction();
};

#endif // SET_H
