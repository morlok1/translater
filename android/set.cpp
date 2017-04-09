#include "set.h"

set::set(QWidget *parent) : QWidget(parent)
{
    grid = new QGridLayout(this);
    setLayout(grid);

    //Сеть
    qnam = new QNetworkAccessManager();


    //База данных
    dbase = QSqlDatabase::addDatabase("QSQLITE"); //Создаем базу данных

    dbase.setDatabaseName("words.sqlite");//Подключаемся
    if (!dbase.open()) //Открываем
        qDebug() << "No.";

    QSqlQuery a_query;
    //Создаем таблицу, если её еще не существует
    QString s = "CREATE TABLE word_table ("
            "id integer PRIMARY KEY NOT NULL, "
            "enWord VARCHAR(255), "                 //Английское слово
            "ruWord VARCHAR(255), "                 //Русское слово
            "rating int(11), "                      //Рейтинг
            "sync int(11), "                        //Синхронизация с сервером
            "learned int(11)"                       //Выучено ли слово
            ");"; //Создаем таблицу, если её еще не существует
    bool b = a_query.exec(s);
    if (!b)
    {
        qDebug() << "Таблица со словами уже существует";
    }

    //Создаем таблицу авторизации, которая будет хранить ник
    s = "CREATE TABLE auth_table ("
            "id integer PRIMARY KEY NOT NULL, "
            "nick VARCHAR(256)"
            ");"; //Создаем таблицу, если её еще не существует
    b = a_query.exec(s);
    if (!b)
    {
        qDebug() << "Таблица автризации уже существует";
    }

    //Необходимо проверить, авторизован ли на данный момент пользователь
    //Чтобы показать соответствующий интерфейс

    s = "SELECT * FROM auth_table";
    b = a_query.exec(s);
    if (!b)
        qDebug() << "Не пошло считывание. auth_table";
    b = false;
    QSqlRecord rec = a_query.record();
    while (a_query.next())
    {
        userNick = a_query.value(rec.indexOf("nick")).toString();
        b = true;
    }


    if (b) //Пользователь авторизован
    {

        getSumOfWords(); //Считаем слова

        state = -1;

        getUserInterface();
    }
    else //Иначе выдаем интерфейс авторизации
        getAuthInterface();
}


//Работа с интерфейсом

void set::getAuthInterface()
{
    authText = new QLabel(this);
    authText->setText("Авторизация");
    authText->setAlignment(Qt::AlignLeft);

    nickname = new QLineEdit(this);
    nickname->setPlaceholderText("Логин");

    getAuth = new QPushButton(this);
    getAuth->setText("Войти");

    authInfo = new QLabel(this);

    grid->addWidget(authText,0,0);
    grid->addWidget(nickname,1,0);
    grid->addWidget(getAuth,2,0);
    grid->addWidget(authInfo,3,0);

    QWidget::connect(getAuth, SIGNAL (clicked()), this, SLOT (startAuthorization()));
}

void set::removeAuthInterface()
{
    //Удаление интерфейса авторизации
    QWidget::disconnect(getAuth, SIGNAL (clicked()), this, SLOT (startAuthorization()));

    grid->removeWidget(authText);
    grid->removeWidget(nickname);
    grid->removeWidget(getAuth);
    grid->removeWidget(authInfo);

    delete authText;
    delete nickname;
    delete getAuth;
    delete authInfo;

    getUserInterface();
}

void set::getUserInterface()
{
    userInfo = new QLabel(this);
    userInfo->setText(userNick + " : " + QString::number(numbOfWord) + " слов(а) всего. \r\n" + QString::number(numbOfNewWord) + " слов(а) новых.");
    getSync = new QPushButton(this);
    getSync->setText("Синхронизировать");

    getWordList = new QPushButton(this);
    getWordList->setText("Обзор слов");

    getWord = new QPushButton(this);
    getWord->setText("Тестирование");

    createNewWord = new QPushButton(this);
    createNewWord->setText("Перевести");
    createNewWord->setDisabled(true);               //Это пока заглушечка, потом допилим сюда форму перевода


    //words = new QListWidget(this);
    grid->addWidget(userInfo, 0,0,1,2);
    grid->addWidget(getSync, 1,0,1,2);
    grid->addWidget(getWordList,2,0,1,2);
    grid->addWidget(getWord,3,0,1,2);
    grid->addWidget(createNewWord,4,0,1,2);
    QWidget::connect(getSync, SIGNAL (clicked()), this, SLOT (startSync()));
    QWidget::connect(getWord, SIGNAL (clicked()), this, SLOT (showTestPage()));
    QWidget::connect(getWordList, SIGNAL (clicked()), this, SLOT (showWordList()));
}

void set::removeUserInterface()
{
    grid->removeWidget(userInfo);
    grid->removeWidget(getSync);
    grid->removeWidget(getWordList);
    grid->removeWidget(getWord);
    grid->removeWidget(createNewWord);

    QWidget::disconnect(getSync, SIGNAL (clicked()), this, SLOT (startSync()));
    QWidget::disconnect(getWord, SIGNAL (clicked()), this, SLOT (showTestPage()));
    QWidget::disconnect(getWordList, SIGNAL (clicked()), this, SLOT (showWordList()));

    delete userInfo;
    delete getSync;
    delete getWordList;
    delete getWord;
    delete createNewWord;
}

void set::getTestInterface()
{
    setRepeatingMode = new QPushButton(this);
    setRepeatingMode->setText("Повторение");

    setLearningMode = new QPushButton(this);
    setLearningMode->setText("Изучение");
    setLearningMode->setDisabled(true);             //Состояние тестирования по умолчанию
    LearningRepeat = true;

    setFromEngToRusMode = new QPushButton(this);
    setFromEngToRusMode->setText("Eng=>Rus");
    setFromEngToRusMode->setDisabled(true);         //Состояние тестирования по умолчанию
    RusEng = true;

    setFromRusToEngMode = new QPushButton(this);
    setFromRusToEngMode->setText("Rus=>Eng");

    startTest = new QPushButton(this);
    startTest->setText("Начать");

    returnToUser = new QPushButton(this);
    returnToUser->setText("Назад");

    word = new QLabel(this);
    word->setAlignment(Qt::AlignCenter);

    grid->addWidget(setRepeatingMode,0,0);
    grid->addWidget(setLearningMode,0,1);
    grid->addWidget(setFromEngToRusMode,1,0);
    grid->addWidget(setFromRusToEngMode,1,1);
    grid->addWidget(startTest,2,0,1,2);
    grid->addWidget(word,3,0,1,2);
    grid->addWidget(returnToUser,6,0,1,2);

    QWidget::connect(setRepeatingMode, SIGNAL (clicked()), this, SLOT(setRepeatingModeAction()));
    QWidget::connect(setLearningMode, SIGNAL (clicked()), this, SLOT(setLearningModeAction()));
    QWidget::connect(setFromRusToEngMode, SIGNAL (clicked()), this, SLOT (setFromRusToEngModeAction()));
    QWidget::connect(setFromEngToRusMode, SIGNAL (clicked()), this, SLOT (setFromEngToRusModeAction()));
    QWidget::connect(startTest, SIGNAL (clicked()), this, SLOT (startTestAction()));
    QWidget::connect(returnToUser, SIGNAL (clicked()), this, SLOT(returnFromTestPageToUserAction()));
}

void set::removeTestInterface()
{
    grid->removeWidget(setRepeatingMode);
    grid->removeWidget(setLearningMode);
    grid->removeWidget(setFromEngToRusMode);
    grid->removeWidget(setFromRusToEngMode);
    grid->removeWidget(startTest);
    grid->removeWidget(word);
    grid->removeWidget(returnToUser);

    delete setRepeatingMode;
    delete setLearningMode;
    delete setFromEngToRusMode;
    delete setFromRusToEngMode;
    delete startTest;
    delete word;
    delete returnToUser;
}



void set::getTestSUBInterface()
{
    nextWord = new QPushButton(this);
    nextWord->setText("Следующее");

    getTranslate = new QPushButton(this);
    getTranslate->setText("Не помню");

    testProgress = new QLabel(this);
    testProgress->setAlignment(Qt::AlignCenter);
    testProgress->setText("1/" + QString::number(testSize));

    grid->addWidget(getTranslate, 4,0);
    grid->addWidget(nextWord, 4,1);
    grid->addWidget(testProgress,5,0,1,2);

    QWidget::connect(getTranslate, SIGNAL (clicked()), this, SLOT (getTranslateAction()));
    QWidget::connect(nextWord, SIGNAL (clicked()), this, SLOT (getWordAction()));

    //Первый проход теста
    state++;
    srand(time(NULL));
    QList<QString> keys = testWord.keys();
    enWord = keys[rand()%testWord.size()];
    ruWord = testWord[enWord];
    testWord.remove(enWord);
    word->setText(RusEng ? enWord : ruWord);

}

void set::removeTestSUBInterface()
{
    QWidget::disconnect(getTranslate, SIGNAL (clicked()), this, SLOT (getTranslateAction()));
    QWidget::disconnect(nextWord, SIGNAL (clicked()), this, SLOT (getWordAction()));

    grid->removeWidget(getTranslate);
    grid->removeWidget(nextWord);
    grid->removeWidget(testProgress);

    delete getTranslate;
    delete nextWord;
    delete testProgress;

    word->clear();

    state = -1;
    startTest->setDisabled(false);
    if (RusEng)
        setFromRusToEngMode->setDisabled(false);
    else
        setFromEngToRusMode->setDisabled(false);

    if (LearningRepeat)
        setRepeatingMode->setDisabled(false);
    else
        setLearningMode->setDisabled(false);
}

void set::getWordListInterface()
{
    returnToUser = new QPushButton(this);
    returnToUser->setText("Назад");

    deleteWord = new QPushButton(this);
    deleteWord->setText("Удалить");
    deleteWord->setDisabled(true);

    setAsLearned = new QPushButton(this);
    setAsLearned->setText("Выучено");
    setAsLearned->setDisabled(true);

    showAllWords = new QPushButton(this);
    showAllWords->setText("Все");
    //showAllWords->setDisabled(true);

    showNewWords = new QPushButton(this);
    showNewWords->setText("Новые");

    showLearnedWords = new QPushButton(this);
    showLearnedWords->setText("Выученные");

    allWord = new QListWidget(this);

    grid->addWidget(deleteWord,0,0,1,3);
    grid->addWidget(setAsLearned,0,3,1,3);
    grid->addWidget(showNewWords, 1,0,1,2);
    grid->addWidget(showAllWords, 1,2,1,2);
    grid->addWidget(showLearnedWords, 1,4,1,2);
    grid->addWidget(allWord,2,0,1,6);
    grid->addWidget(returnToUser,3,0,1,6);

    setLayout(grid);

    QWidget::connect(deleteWord, SIGNAL (clicked()), this, SLOT(deleteWordAction()));
    QWidget::connect(setAsLearned, SIGNAL (clicked()), this, SLOT(setAsLearnedAction()));
    QWidget::connect(returnToUser, SIGNAL (clicked()), this, SLOT(returnFromWordListToUserAction()));
    QWidget::connect(allWord, SIGNAL (itemSelectionChanged()), this, SLOT (activateDeleteButton()));
    QWidget::connect(showNewWords, SIGNAL(clicked()), this, SLOT (showNewWordsAction()));
    QWidget::connect(showAllWords, SIGNAL(clicked()), this, SLOT (showAllWordsAction()));
    QWidget::connect(showLearnedWords, SIGNAL(clicked()), this, SLOT (showLearnedWordsAction()));

    showAllWordsAction();

}

void set::removeWordListInterface()
{
    grid->removeWidget(deleteWord);
    grid->removeWidget(setAsLearned);
    grid->removeWidget(showNewWords);
    grid->removeWidget(showAllWords);
    grid->removeWidget(showLearnedWords);
    grid->removeWidget(allWord);
    grid->removeWidget(returnToUser);

    QWidget::disconnect(deleteWord, SIGNAL (clicked()), this, SLOT(deleteWordAction()));
    QWidget::disconnect(setAsLearned, SIGNAL (clicked()), this, SLOT(setAsLearnedAction()));
    QWidget::disconnect(returnToUser, SIGNAL (clicked()), this, SLOT(returnFromWordListToUserAction()));
    QWidget::disconnect(allWord, SIGNAL (itemSelectionChanged()), this, SLOT (activateDeleteButton()));
    QWidget::disconnect(showNewWords, SIGNAL(clicked()), this, SLOT (showNewWordsAction()));
    QWidget::disconnect(showAllWords, SIGNAL(clicked()), this, SLOT (showAllWordsAction()));
    QWidget::disconnect(showLearnedWords, SIGNAL(clicked()), this, SLOT (showLearnedWordsAction()));

    delete deleteWord;
    delete setAsLearned;
    delete showNewWords;
    delete showAllWords;
    delete showLearnedWords;
    delete returnToUser;
    delete allWord;
}



set::~set()
{

}

//Синхронизация
void set::startSync()
{
    multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QString url = "http://morlok1.esy.es/getData.php";

    //authInfo->setText("Авторизуемся");
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"nick\"")));
    QByteArray postDataByteArray;
    postDataByteArray.append(userNick);
    textPart.setBody(postDataByteArray);
    multiPart->append(textPart);

    reply = qnam->post(QNetworkRequest(QUrl(url)), &(*multiPart));

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
    QString s_insert = "INSERT INTO word_table (enWord, ruWord, rating, sync, learned) VALUES ('%1', '%2', %3, %4, %5);";
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
                    .arg(0)         //Рейтинг - 0
                    .arg(1)         //Синхронизированное слово
                    .arg(0);        //Слово не выучено
            if (a_query.exec(s))
            {
                //numbOfWord++;
                qDebug() << "Добавлена новая пара слов в базу данных:" + ruLine + "---"+enLine;
            }
        }
    }
    getSumOfWords();
    userInfo->setText(userNick + " : " + QString::number(numbOfWord) + " слов(а) всего. \r\n" + QString::number(numbOfNewWord) + " слов(а) новых.");
    //userInfo->setText(userNick + " : " + QString::number(numbOfWord) + " слов(а).");
}

QString set::getFormatString(QString str)
{
    str = str.right(str.length() - 4).trimmed();
    return str;
}

//Тестирование
void set::startTestAction()
{
    testWord.clear();
    //getWord->setDisabled(true);
    startTest->setDisabled(true);
    setRepeatingMode->setDisabled(true);
    setLearningMode->setDisabled(true);
    setFromRusToEngMode->setDisabled(true);
    setFromEngToRusMode->setDisabled(true);
    //if (numbOfWord != 0)
    //{
        srand(time(NULL));
        QString s;
        QSqlQuery a_query;
        bool b;

        s = "SELECT * FROM word_table WHERE learned=" + QString::number(LearningRepeat ? 0 : 1);
        b = a_query.exec(s);
        if (!b)
            qDebug() << "Не пошло считываие.";
        QSqlRecord rec = a_query.record();
        while (a_query.next())
        {
            enWord = a_query.value(rec.indexOf("enWord")).toString();
            ruWord = a_query.value(rec.indexOf("ruWord")).toString();
            testWord.insert(enWord,ruWord);
        }
        //Включить интерфейс прохождения теста
        state = 0;
        testSize = testWord.size() < 10 ? testWord.size() : 10;
        if (testSize != 0)
            getTestSUBInterface();
    //}
    //else
        else
            word->setText("База пуста");
}

void set::getWordAction()
{
    srand(time(NULL));
    if (state != testSize)
    {
        QList<QString> keys = testWord.keys();
        enWord = keys[rand()%testWord.size()];
        ruWord = testWord[enWord];
        testWord.remove(enWord);
        word->setText(RusEng ? enWord : ruWord);
        state++;
        testProgress->setText(QString::number(state) + "/" + QString::number(testSize));
    }
    else
        removeTestSUBInterface();
}

void set::getTranslateAction()
{
    word->setText(RusEng ? ruWord : enWord);
}


//Авторизация
void set::startAuthorization()
{
    multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QString url = "http://morlok1.esy.es/checkNickName.php";
    QString nick = nickname->text().trimmed();
    if (nick != "") //Если ник не оказывается пустым
    {
        //authInfo->setText("Авторизуемся");
        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"nick\"")));
        QByteArray postDataByteArray;
        postDataByteArray.append(nick);
        textPart.setBody(postDataByteArray);
        multiPart->append(textPart);

        reply = qnam->post(QNetworkRequest(QUrl(url)), &(*multiPart));

        QWidget::connect(reply, SIGNAL (finished()), this, SLOT (getAuthRequest()));

    }
    else //Иначе - жалуемся
        authInfo->setText("Логин пуст");
}

void set::getAuthRequest()
{
    QFile file("words.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QIODevice * content = static_cast<QIODevice*>(QObject::sender());
    file.write(content->readAll());
    file.close();

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString str;
    bool res = false;
    while (!file.atEnd()) //До самого конца файла
    {
        str = file.readLine();
        if (str.indexOf("answer:1") != -1)
            res = true;
    }

    if (res) //Если севрер ответил нам хорошими новостями
    {
        //Переходим к интерфейсу тестирования

        userNick = nickname->text(); //Запоминаем ник на время этой сессии

        //Считаем количество слов
        numbOfWord = 0;
        //Посчитаем  количество слов в словаре
        str = "SELECT * FROM word_table";
        QSqlQuery a_query;
        res = a_query.exec(str);
        if (!res)
            qDebug() << "Не пошло считываие. word_table";
        while (a_query.next())
            numbOfWord++;

        state = -1;

        removeAuthInterface();

        str = "INSERT INTO auth_table (nick) VALUES ('" + userNick + "')";
        res = a_query.exec(str);
        if (!res)
            qDebug() << "Авторизация в базе данных не отметилась";
    }
    else
        authInfo->setText("Логин не найден");
}


//Переход между окнами

//Главный экран => обзор слов
void set::showWordList()
{
    removeUserInterface();
    getWordListInterface();
}
//Обзор слов => главный экран
void set::returnFromWordListToUserAction()
{
    removeWordListInterface();
    getUserInterface();
}


void set::showTestPage()
{
    removeUserInterface();
    getTestInterface();
}

void set::returnFromTestPageToUserAction()
{
    if (word->text() != "" && word->text() != "База пуста")
        removeTestSUBInterface();
    removeTestInterface();
    getUserInterface();
}


//Работа с кнопками удаления\изменения группы
void set::activateDeleteButton()
{
    deleteWord->setDisabled(false);
    setAsLearned->setDisabled(false);
}

void set::deleteWordAction()
{
    deleteWord->setDisabled(true);
    setAsLearned->setDisabled(true);
    if (allWord->currentItem())
    {
        QString engWord = allWord->currentItem()->text();
        engWord = engWord.left(engWord.indexOf(" --- ") + 1).trimmed();

        QSqlQuery a_query;
        QString s, s_insert;
        s_insert = "DELETE FROM word_table WHERE enWord='%1'";
        s = s_insert.arg(engWord);
        a_query.exec(s);
        //Перезаписываем
        //fillTheWordList();
        getSumOfWords();
        showAllWordsAction();
    }
}

void set::setAsLearnedAction()
{
    deleteWord->setDisabled(true);
    setAsLearned->setDisabled(true);
    if (allWord->currentItem())
    {
        QString engWord = allWord->currentItem()->text();
        engWord = engWord.left(engWord.indexOf(" --- ") + 1).trimmed();

        QSqlQuery a_query;
        QString s, s_insert;
        s_insert = "UPDATE word_table SET learned=1 WHERE enWord='%1'";
        s = s_insert.arg(engWord);
        a_query.exec(s);
        //Перезаписываем
        //fillTheWordList();
        getSumOfWords();
        showAllWordsAction();
    }
}


//Показываем слова
void set::showAllWordsAction()
{
    deleteWord->setDisabled(true);
    setAsLearned->setDisabled(true);

    showAllWords->setDisabled(true);
    showNewWords->setDisabled(false);
    showLearnedWords->setDisabled(false);
    allWord->clear();
    //А теперь заполняем листы
    QSqlQuery a_query;
    a_query.exec("SELECT * FROM word_table");
    QSqlRecord rec = a_query.record();

    while(a_query.next())
    {
        enWord = a_query.value(rec.indexOf("enWord")).toString();
        ruWord = a_query.value(rec.indexOf("ruWord")).toString();
        allWord->addItem(enWord + " --- " + ruWord);
    }
}

void set::showNewWordsAction()
{
    deleteWord->setDisabled(true);
    setAsLearned->setDisabled(true);

    showAllWords->setDisabled(false);
    showNewWords->setDisabled(true);
    showLearnedWords->setDisabled(false);
    allWord->clear();
    //А теперь заполняем листы
    QSqlQuery a_query;
    a_query.exec("SELECT * FROM word_table WHERE learned=0");
    QSqlRecord rec = a_query.record();

    while(a_query.next())
    {
        enWord = a_query.value(rec.indexOf("enWord")).toString();
        ruWord = a_query.value(rec.indexOf("ruWord")).toString();
        allWord->addItem(enWord + " --- " + ruWord);
    }
}

void set::showLearnedWordsAction()
{
    deleteWord->setDisabled(true);
    setAsLearned->setDisabled(true);

    showAllWords->setDisabled(false);
    showNewWords->setDisabled(false);
    showLearnedWords->setDisabled(true);
    allWord->clear();
    //А теперь заполняем листы
    QSqlQuery a_query;
    a_query.exec("SELECT * FROM word_table WHERE learned=1");
    QSqlRecord rec = a_query.record();

    while(a_query.next())
    {
        enWord = a_query.value(rec.indexOf("enWord")).toString();
        ruWord = a_query.value(rec.indexOf("ruWord")).toString();
        allWord->addItem(enWord + " --- " + ruWord);
    }
}


//Настраиваем тестирование
void set::setRepeatingModeAction()
{
    LearningRepeat = false;
    setRepeatingMode->setDisabled(true);
    setLearningMode->setDisabled(false);
}

void set::setLearningModeAction()
{
    LearningRepeat = true;
    setRepeatingMode->setDisabled(false);
    setLearningMode->setDisabled(true);
}

void set::setFromEngToRusModeAction()
{
    RusEng = true;
    setFromEngToRusMode->setDisabled(true);
    setFromRusToEngMode->setDisabled(false);
}

void set::setFromRusToEngModeAction()
{
    RusEng = false;
    setFromEngToRusMode->setDisabled(false);
    setFromRusToEngMode->setDisabled(true);
}




void set::getSumOfWords()
{
    numbOfWord = 0;
    numbOfNewWord = 0;
    //Посчитаем  количество слов в словаре

    QSqlQuery a_query;
    QString s = "SELECT * FROM word_table";
    bool b = a_query.exec(s);
    QSqlRecord rec = a_query.record();
    if (!b)
        qDebug() << "Не пошло считываие. word_table";
    while (a_query.next())
    {
        numbOfWord++;
        numbOfNewWord += a_query.value(rec.indexOf("learned")).toInt();
    }
    numbOfNewWord = numbOfWord - numbOfNewWord;
}
