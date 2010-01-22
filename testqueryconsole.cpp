#include "testqueryconsole.h"
#include "ui_testqueryconsole.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QStringList>
#include <QList>
#include <QPair>

#include <QDebug>

TestQueryConsole::TestQueryConsole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestQueryConsole)
{
    ui->setupUi(this);

    connect(ui->addButton, SIGNAL(clicked()),
            this, SLOT(addPostArgs()));

    connect(ui->sendButton, SIGNAL(clicked()),
            this, SLOT(sendQuery()));


}

TestQueryConsole::~TestQueryConsole()
{
    delete ui;
}

void TestQueryConsole::addPostArgs() {

if ((ui->postVarKey->text().compare("") != 0) && (ui->postVarValue->text().compare("") != 0))
    ui->encodedPostArgs->setPlainText(QString(ui->encodedPostArgs->toPlainText() +
                                              ui->postVarKey->text() + "=" +
                                              ui->postVarValue->text() + "&"));
}

void TestQueryConsole::sendQuery() {

    // Build the post args from the input box
    QString origArgs = ui->encodedPostArgs->toPlainText();
    if (origArgs.endsWith("&"))
        origArgs.chop(1);


    QByteArray args(origArgs.toAscii());
    QByteArray exclude("&=");
    QByteArray include;
    QByteArray encodedArgs = args.toPercentEncoding(exclude,include,'%');

    qDebug() << encodedArgs;



    QUrl url("http://api.facebook.com/restserver.php");

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest nr;
    nr.setUrl(url);
    nr.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = 0;







}


void TestQueryConsole::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
