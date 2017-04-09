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
    QPushButton * createNewWord;

    //Интерфейс тестирования
    QPushButton* setLearningMode;
    QPushButton* setRepeatingMode;
    QPushButton* setFromEngToRusMode;
    QPushButton* setFromRusToEngMode;
    QPushButton* startTest;
    QLabel * word;
    //Подинтерфейс тестирования
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

    //Текущая пара тестирования
    QString enWord;
    QString ruWord;
    //Количество слов
    int numbOfWord;
    //Количество новых слов
    int numbOfNewWord;
    //Какое-то состояние, которое я где-то постоянно дергаю, но не помню, зачем
    int state;
    //А здесь хранятся 10 слов для перевода во время тестирования
    QMap<QString, QString> testWord;
    //Направление перевода при тестировании
    bool RusEng;            //true - eng=>rus
                            //false - rus=>eng
    //Режим тестирования
    bool LearningRepeat;    //true - изучение
                            //false - повторение

    //Ну а это ник пользователя, под которым он авторизован
    QString userNick;


public:

    set(QWidget *parent = 0);
    ~set();

    QString getFormatString(QString);
    void getSumOfWords();

    //void fillTheWordList();


public slots:
    void startSync();
    void writeToFile();
    void syncWithServer();
    void startTestAction();
    void getWordAction();
    void getTranslateAction();

    //Работа с интерфейсом
    void getAuthInterface();
    void removeAuthInterface();
    void getUserInterface();
    void removeUserInterface();
    void getWordListInterface();
    void removeWordListInterface();
    void getTestInterface();
    void removeTestInterface();
    void getTestSUBInterface();
    void removeTestSUBInterface();

    //Авторизация
    void startAuthorization();
    void getAuthRequest();



    void showWordList();
    void returnFromWordListToUserAction();

    void showTestPage();
    void returnFromTestPageToUserAction();


    void deleteWordAction();
    void setAsLearnedAction();

    void activateDeleteButton();

    void showNewWordsAction();
    void showAllWordsAction();
    void showLearnedWordsAction();


    void setRepeatingModeAction();
    void setLearningModeAction();
    void setFromEngToRusModeAction();
    void setFromRusToEngModeAction();
};

#endif // SET_H
