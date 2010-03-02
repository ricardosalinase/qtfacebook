#ifndef FBATTACHMENTWIDGET_H
#define FBATTACHMENTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPair>

#include "data/FbStreamAttachment.h"
#include "gui/FbPhotoLabel.h"

namespace GUI {

class FbAttachmentWidget : public QWidget
{
Q_OBJECT
public:
    FbAttachmentWidget(DATA::FbStreamAttachment *attachment,
                                 QWidget *parent = 0);
    QSize minimumSizeHint() const;
signals:
    void userClickedImage(QString link);
public slots:
    void gotNetworkReply(QNetworkReply *);

private:
    enum RequestType { Photo, Other };
    QNetworkAccessManager *m_nam;
    QMap<QNetworkReply *, QPair<RequestType, GUI::FbPhotoLabel *> > m_outstandingRequests;

};

} // namespace GUI

#endif // FBATTACHMENTWIDGET_H
