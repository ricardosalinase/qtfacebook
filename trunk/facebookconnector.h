#ifndef FACEBOOKCONNECTOR_H
#define FACEBOOKCONNECTOR_H

#include <QWidget>

class FaceBookConnector : public QWidget
{
Q_OBJECT
public:
    explicit FaceBookConnector(QString apiKey);
    FaceBookConnector(QWidget *parent, QString apiKey);


signals:

public slots:

private:
    QString m_apiKey;
    void init();

};

#endif // FACEBOOKCONNECTOR_H
