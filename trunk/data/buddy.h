#ifndef Buddy_H
#define Buddy_H

#include <QString>
#include <QUrl>
#include <QStringList>

namespace DATA {

class Buddy
{
    
public:
    Buddy();

    QString getUID();
    void setUID(QString id);
    QString getFirstName();
    void setFirstName(QString fn);
    QString getFullName();
    void setFullName(QString fn);
    QUrl getThumbUrl();
    void setThumbUrl(QString url);
    bool isIdle();
    void isIdle(bool);
    bool isInList(QString listId);
    QList<QString> getListIds();
    void addToList(QString listId);
    void addtoList(QStringList ids);


private:
    QString m_uid;
    QString m_firstName;
    QString m_fullName;
    QUrl m_thumbUrl;
    bool m_idle;
    QList<QString> m_listIds;


};

} // namespace DATA

#endif // Buddy_H
