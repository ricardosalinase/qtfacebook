#ifndef FBSTREAMPOSTPHOTOWIDGET_H
#define FBSTREAMPOSTPHOTOWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "data/FbStreamAttachment.h"

namespace GUI {

class FbStreamPostPhotoWidget : public QWidget
{
Q_OBJECT
public:
    enum RequestType { PosterPixmap, Photo, AppIcon, LinkThumb };
    explicit FbStreamPostPhotoWidget(DATA::FbStreamAttachment *attachment, QWidget *parent = 0);


signals:

public slots:

private slots:
    void gotNetworkReply(QNetworkReply *);

private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_photoLayout;
    QNetworkAccessManager *m_nam;

};

} // namespace GUI

#endif // FBSTREAMPOSTPHOTOWIDGET_H
