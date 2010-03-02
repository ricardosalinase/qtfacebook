#ifndef FBSTREAMPOSTATTACHMENTWIDGET_H
#define FBSTREAMPOSTATTACHMENTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "data/FbStreamAttachment.h"

namespace GUI {

class FbStreamPostAttachmentWidget : public QWidget
{
Q_OBJECT
public:
    FbStreamPostAttachmentWidget(DATA::FbStreamAttachment *attachment,
                                 QWidget *parent = 0);

signals:

public slots:
    //void gotNetworkReply(QNetworkReply *);

private:
    QNetworkAccessManager *m_nam;

};

} // namespace GUI

#endif // FBSTREAMPOSTATTACHMENTWIDGET_H
