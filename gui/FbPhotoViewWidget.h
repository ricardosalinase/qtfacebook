#ifndef FBPHOTOVIEWWIDGET_H
#define FBPHOTOVIEWWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QCloseEvent>

#include "userinfo.h"
#include "data/FbPhoto.h"
#include "gui/LoadingProgressWidget.h"

namespace GUI {

class FbPhotoViewWidget : public QWidget
{
Q_OBJECT
public:
    FbPhotoViewWidget(DATA::FbPhoto *photo, UserInfo *info, QWidget *parent = 0);

signals:
    void closed(GUI::FbPhotoViewWidget*);
    void userClickedLink(QString link);

public slots:

private slots:
    void gotNetworkReply(QNetworkReply *reply);

private:
    void closeEvent ( QCloseEvent * event );
    DATA::FbPhoto *m_photo;
    UserInfo *m_info;
    bool m_isOwner;
    QNetworkAccessManager *m_nam;
    LoadingProgressWidget *m_progress;
};




} // namespace GUI

#endif // FBPHOTOVIEWWIDGET_H
