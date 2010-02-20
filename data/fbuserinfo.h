#ifndef FBUSERINFO_H
#define FBUSERINFO_H

#include <QString>
#include <QUrl>

namespace DATA {

class FbUserInfo
{
public:
    FbUserInfo();

    void setUID(QString id);
    QString& getUID();
    void setName(QString name);
    QString& getName();
    void setPic(QString pic);
    QUrl& getPic();
    void setPicSquare(QString ps);
    QUrl& getPicSquare();
    void setPicBig(QString pb);
    QUrl& getPicBig();

private:
    QString m_uid;
    QString m_name;
    QUrl m_pic;
    QUrl m_picSquare;
    QUrl m_picBig;

};

} // namespace DATA

#endif // FBUSERINFO_H
