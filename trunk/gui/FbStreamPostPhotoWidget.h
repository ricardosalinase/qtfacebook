#ifndef FBSTREAMPOSTPHOTOWIDGET_H
#define FBSTREAMPOSTPHOTOWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPair>

#include "data/FbStreamAttachment.h"
#include "data/FbPhoto.h"

namespace GUI {

class FbStreamPostPhotoWidget : public QWidget
{
Q_OBJECT
public:
    enum RequestType { PosterPixmap, Photo, AppIcon, LinkThumb };
    explicit FbStreamPostPhotoWidget(DATA::FbStreamAttachment *attachment, QWidget *parent = 0);


signals:
    void photoSelected(QString photoId, QString albumId, QString ownerId);
public slots:

private slots:
    void gotNetworkReply(QNetworkReply *);
    void userClickedImage(QString pid);

private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_photoLayout;
    QNetworkAccessManager *m_nam;
    QMap<QNetworkReply *, DATA::FbPhoto> m_outstandingNetworkRequests;
    QMap<QString, DATA::FbPhoto> m_photoMap;


};

} // namespace GUI

#endif // FBSTREAMPOSTPHOTOWIDGET_H
