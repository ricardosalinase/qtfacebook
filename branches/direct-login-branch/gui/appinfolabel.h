#ifndef APIINFOLABEL_H
#define APIINFOLABEL_H

#include <QLabel>
#include <QNetworkReply>

#include "data/appinfo.h"

namespace GUI {

class AppInfoLabel : public QLabel
{
Q_OBJECT
public:
    AppInfoLabel(DATA::AppInfo *ai, QWidget *parent = 0);
    ~AppInfoLabel();
    DATA::AppInfo * getAppInfo();

signals:

public slots:
    void myIconPixmap(QPixmap *p, bool display);
    void myLogoPixmap(QPixmap *p, bool display);

private:
    DATA::AppInfo *m_appInfo;

};

} // namespace GUI

#endif // APIINFOLABEL_H
