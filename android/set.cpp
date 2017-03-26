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
            "enWord VARCHAR(255), "
            "ruWord VARCHAR(255), "
            "rating int(11)"
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
        numbOfWord = 0;
        //Посчитаем  количество слов в словаре
        s = "SELECT * FROM word_table";
        b = a_query.exec(s);
        if (!b)
            qDebug() << "Не пошло считываие. word_table";
        while (a_query.next())
            numbOfWord++;

        state = true;

        getUserInterface();
    }
    else //Иначе выдаем интерфейс авторизации
        getAuthInterface();
}

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
    userInfo->setText(userNick + " : " + QString::number(numbOfWord) + " слов(а).");
    getSync = new QPushButton(this);
    getSync->setText("Синхронизировать");

    getWord = new QPushButton(this);
    getWord->setText("Начать");

    word = new QLabel(this);
    word->setAlignment(Qt::AlignCenter);
    //words = new QListWidget(this);
    grid->addWidget(userInfo, 0,0,1,2);
    grid->addWidget(getSync, 1,0,1,2);
    grid->addWidget(getWord,2,0,1,2);
    grid->addWidget(word,3,0,1,2);
    QWidget::connect(getSync, SIGNAL (clicked()), this, SLOT (startSync()));
    QWidget::connect(getWord, SIGNAL (clicked()), this, SLOT (startTest()));
}

set::~set()
{

}

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

    //QString url = "http://morlok1.esy.es/getData.php";
    //reply = qnam->get(QNetworkRequest(QUrl(url)));
    //QWidget::connect(reply, SIGNAL (finished()), this, SLOT (writeToFile()));
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
                numbOfWord++;
                qDebug() << "Добавлена новая пара слов в базу данных:" + ruLine + "---"+enLine;
            }
        }
    }
    userInfo->setText(userNick + " : " + QString::number(numbOfWord) + " слов(а).");
}

QString set::getFormatString(QString str)
{
    str = str.right(str.length() - 4).trimmed();
    return str;
}

void set::startTest()
{
    srand(time(NULL));
    int numberOfWord = -1;
    QString s;
    QSqlQuery a_query;
    bool b;
    QVector<int> numbers;
    for (int i = 0; i < (numbOfWord < 10 ? numbOfWord : 10); i++)
    {
        numberOfWord = (rand()%numbOfWord)+1;
        while (numbers.contains(numberOfWord))
            numberOfWord = (rand()%numbOfWord)+1;
        numbers.append(numberOfWord);
        s = "SELECT * FROM word_table WHERE id=" + QString::number(numberOfWord);
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
    }

    //Включить интерфейс прохождения теста
    state = 0;
    getTestInterface();
}

void set::getTestInterface()
{
    nextWord = new QPushButton(this);
    nextWord->setText("Следующее");
    getTranslate = new QPushButton(this);
    getTranslate->setText("Не помню");
    testProgress = new QLabel(this);
    testProgress->setAlignment(Qt::AlignCenter);
    testProgress->setText("1/" + QString::number(numbOfWord < 10 ? numbOfWord : 10));
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
    word->setText(enWord);


}

void set::getWordAction()
{
    srand(time(NULL));
    if (testWord.size() != 0)
    {
        QList<QString> keys = testWord.keys();
        enWord = keys[rand()%testWord.size()];
        ruWord = testWord[enWord];
        testWord.remove(enWord);
        word->setText(enWord);
        state++;
        testProgress->setText(QString::number(state) + "/" + QString::number(numbOfWord < 10 ? numbOfWord : 10));
    }
    else
        removeTestInterface();
}

void set::getTranslateAction()
{
    word->setText(ruWord);
}

void set::removeTestInterface()
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
}

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

        state = true;

        removeAuthInterface();

        str = "INSERT INTO auth_table (nick) VALUES ('" + userNick + "')";
        res = a_query.exec(str);
        if (!res)
            qDebug() << "Авторизация в базе данных не отметилась";
    }
    else
        authInfo->setText("Логин не найден");
}
