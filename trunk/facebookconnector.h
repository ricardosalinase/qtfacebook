#ifndef FACEBOOKCONNECTOR_H
#define FACEBOOKCONNECTOR_H

#include <QWidget>

class FaceBookConnector : public QWidget
{
Q_OBJECT
public:
    FaceBookConnector(QString apiKey, QWidget *parent = 0);



signals:

public slots:

private:
    QString m_apiKey;


};

#endif // FACEBOOKCONNECTOR_H
