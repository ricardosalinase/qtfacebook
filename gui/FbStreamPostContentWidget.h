#ifndef FBSTREAMPOSTCONTENTWIDGET_H
#define FBSTREAMPOSTCONTENTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "data/FbStreamAttachment.h"

namespace GUI {

class FbStreamPostContentWidget : public QWidget
{
Q_OBJECT
public:
    explicit FbStreamPostContentWidget(DATA::FbStreamAttachment *attachment, QWidget *parent = 0);
    virtual QSize minimumSizeHint () const;
signals:
    void contentLoaded();
public slots:

private slots:
    void gotNetworkReply(QNetworkReply *);

private:
    QHBoxLayout *m_linkLayout;
    QNetworkAccessManager *m_nam;

};

} // namespace GUI

#endif // FBSTREAMPOSTCONTENTWIDGET_H
