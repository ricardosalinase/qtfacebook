#include "facebookconnector.h"

FaceBookConnector::FaceBookConnector(QWidget *parent, QString apiKey) :
    QWidget(parent),
    m_apiKey(apiKey)
{
    init();
}

FaceBookConnector::FaceBookConnector(QString apiKey) :
    QWidget(0),
    m_apiKey(apiKey)
{
    init();
}

void FaceBookConnector::init() {





}

/*
void FaceBookConnector::setApiKey(QString apiKey)  {
    m_apiKey = apiKey;
} */
