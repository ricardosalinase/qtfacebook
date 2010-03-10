#ifndef FBATTACHMENTWIDGET_H
#define FBATTACHMENTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPair>
#include <QWebView>

#include "data/FbStreamAttachment.h"
#include "gui/ImageLabel.h"

namespace GUI {

class FbAttachmentWidget : public QWidget
{
Q_OBJECT
public:
    FbAttachmentWidget(DATA::FbStreamAttachment *attachment,
                                 QWidget *parent = 0);
    QSize minimumSizeHint() const;
signals:
    void userClickedUrl(QString link);

public slots:
    void gotNetworkReply(QNetworkReply *);

private slots:
    void userClickedVideo(QString);

private:
    enum RequestType { Photo, Other };
    QNetworkAccessManager *m_nam;
    QMap<QNetworkReply *, QPair<RequestType, GUI::ImageLabel *> > m_outstandingRequests;
    QWebView *m_webView;
    GUI::ImageLabel *m_videoLabel;
};

} // namespace GUI

#endif // FBATTACHMENTWIDGET_H
