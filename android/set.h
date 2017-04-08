#ifndef SET_H
#define SET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

//Работа с сетью
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QHttpMultiPart>

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
    //Главный экран
    QLabel* userInfo;

    QPushButton * getSync;
    //QListWidget * words;
    QPushButton * getWordList;
    QPushButton * getWord;
    QLabel * word;

    //Интерфейс тестирования
    QPushButton* nextWord;
    QPushButton* getTranslate;
    QLabel* testProgress;

    //Авторизация
    QLabel* authText;
    QLineEdit* nickname;
    QPushButton* getAuth;
    QLabel* authInfo;

    //Обзор слов
    QPushButton* returnToUser;
    QPushButton* deleteWord;
    QPushButton* setAsLearned;
    QPushButton* showAllWords;
    QPushButton* showNewWords;
    QPushButton* showLearnedWords;
    QListWidget* allWord;

    //Сеть
    QNetworkAccessManager* qnam;
    QNetworkReply *reply;
    QHttpMultiPart* multiPart;
    
    //База данных
    QSqlDatabase dbase;


    //Данные
    QString enWord;
    QString ruWord;
    int numbOfWord;
    int state;
    QMap<QString, QString> testWord;


    QString userNick;
public:
    set(QWidget *parent = 0);
    ~set();

    QString getFormatString(QString);
    void getTestInterface();
    void removeTestInterface();

    //void fillTheWordList();


public slots:
    void startSync();
    void writeToFile();
    void syncWithServer();
    void startTest();
    void getWordAction();
    void getTranslateAction();

    //Работа с интерфейсом
    void getAuthInterface();
    void removeAuthInterface();
    void getUserInterface();
    void removeUserInterface();
    void getWordListInterface();
    void removeWordListInterface();

    //Авторизация
    void startAuthorization();
    void getAuthRequest();

    void showWordList();
    void returnFromWordListToUserAction();

    void deleteWordAction();
    void setAsLearnedAction();

    void activateDeleteButton();

    void showNewWordsAction();
    void showAllWordsAction();
    void showLearnedWordsAction();
};

#endif // SET_H
