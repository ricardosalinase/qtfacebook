#ifndef FBPAGEINFO_H
#define FBPAGEINFO_H

#include <QString>
#include <QUrl>

namespace DATA {

class FbPageInfo
{
public:
    FbPageInfo();

    void setPageId(const QString& id);
    QString& getPageId();
    void setName(const QString& name);
    QString& getName();
    void setPic(const QString& pic);
    QUrl& getPic();
    void setPicSquare(const QString& ps);
    QUrl& getPicSquare();
    void setPicBig(const QString& pb);
    QUrl& getPicBig();

private:
    QString m_pageId;
    QString m_name;
    QUrl m_pic;
    QUrl m_picSquare;
    QUrl m_picBig;


};

} // namespace DATA

#endif // FBPAGEINFO_H
