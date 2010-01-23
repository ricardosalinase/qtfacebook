/********************************************************************************
** Form generated from reading UI file 'testqueryconsole.ui'
**
** Created: Sat Jan 23 09:13:39 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTQUERYCONSOLE_H
#define UI_TESTQUERYCONSOLE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TestQueryConsole
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *apiCallInput;
    QLabel *label_2;
    QLineEdit *postVarKey;
    QLabel *label_3;
    QPushButton *addButton;
    QPlainTextEdit *encodedPostArgs;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTextBrowser *outputFrame;
    QPushButton *sendButton;
    QLineEdit *postVarValue;

    void setupUi(QWidget *TestQueryConsole)
    {
        if (TestQueryConsole->objectName().isEmpty())
            TestQueryConsole->setObjectName(QString::fromUtf8("TestQueryConsole"));
        TestQueryConsole->resize(509, 608);
        gridLayout = new QGridLayout(TestQueryConsole);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(TestQueryConsole);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 17));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        apiCallInput = new QLineEdit(TestQueryConsole);
        apiCallInput->setObjectName(QString::fromUtf8("apiCallInput"));
        apiCallInput->setMinimumSize(QSize(0, 27));

        gridLayout->addWidget(apiCallInput, 0, 1, 1, 4);

        label_2 = new QLabel(TestQueryConsole);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        postVarKey = new QLineEdit(TestQueryConsole);
        postVarKey->setObjectName(QString::fromUtf8("postVarKey"));

        gridLayout->addWidget(postVarKey, 1, 1, 1, 1);

        label_3 = new QLabel(TestQueryConsole);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 2, 1, 1);

        addButton = new QPushButton(TestQueryConsole);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        gridLayout->addWidget(addButton, 1, 4, 1, 1);

        encodedPostArgs = new QPlainTextEdit(TestQueryConsole);
        encodedPostArgs->setObjectName(QString::fromUtf8("encodedPostArgs"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(encodedPostArgs->sizePolicy().hasHeightForWidth());
        encodedPostArgs->setSizePolicy(sizePolicy);

        gridLayout->addWidget(encodedPostArgs, 3, 0, 1, 5);

        scrollArea = new QScrollArea(TestQueryConsole);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy1);
        scrollArea->setMinimumSize(QSize(0, 400));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 489, 398));
        outputFrame = new QTextBrowser(scrollAreaWidgetContents);
        outputFrame->setObjectName(QString::fromUtf8("outputFrame"));
        outputFrame->setGeometry(QRect(-5, -9, 501, 411));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 5, 0, 1, 5);

        sendButton = new QPushButton(TestQueryConsole);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));

        gridLayout->addWidget(sendButton, 4, 0, 1, 5);

        postVarValue = new QLineEdit(TestQueryConsole);
        postVarValue->setObjectName(QString::fromUtf8("postVarValue"));

        gridLayout->addWidget(postVarValue, 1, 3, 1, 1);

        QWidget::setTabOrder(apiCallInput, postVarKey);
        QWidget::setTabOrder(postVarKey, postVarValue);
        QWidget::setTabOrder(postVarValue, addButton);
        QWidget::setTabOrder(addButton, encodedPostArgs);
        QWidget::setTabOrder(encodedPostArgs, sendButton);
        QWidget::setTabOrder(sendButton, scrollArea);
        QWidget::setTabOrder(scrollArea, outputFrame);

        retranslateUi(TestQueryConsole);

        QMetaObject::connectSlotsByName(TestQueryConsole);
    } // setupUi

    void retranslateUi(QWidget *TestQueryConsole)
    {
        TestQueryConsole->setWindowTitle(QApplication::translate("TestQueryConsole", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TestQueryConsole", "API Call:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TestQueryConsole", "POST Args:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TestQueryConsole", "=", 0, QApplication::UnicodeUTF8));
        addButton->setText(QApplication::translate("TestQueryConsole", "Add", 0, QApplication::UnicodeUTF8));
        sendButton->setText(QApplication::translate("TestQueryConsole", "Send", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TestQueryConsole: public Ui_TestQueryConsole {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTQUERYCONSOLE_H
