#ifndef STREAMPOSTWIDGET_H
#define STREAMPOSTWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScrollArea>
#include <QLabel>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>
#include <QProgressBar>

#include "data/streampost.h"
#include "api/factory.h"
#include "gui/commentwidget.h"



namespace GUI {

class StreamPostWidget : public QWidget
{
Q_OBJECT
public:
    enum RequestType { PosterPixmap, AppIcon };
    explicit StreamPostWidget(DATA::StreamPost *post, QWidget *parent = 0);
    ~StreamPostWidget();

signals:
    void closed(GUI::StreamPostWidget*);
    void contentClicked(QString url);
public slots:

protected:
    void closeEvent ( QCloseEvent * event );

private slots:
    void gotNetworkReply(QNetworkReply *reply);
    void gotContentUpdate();


private:
    void getPosterPixmap();
    void getPhoto(DATA::FbStreamMedia *media);
    QVBoxLayout *m_contentLayout;
    QHBoxLayout *m_photoLayout;
    QHBoxLayout *m_ageLineLayout;
    QHBoxLayout *m_linkLayout;
    QNetworkAccessManager *m_nam;
    DATA::StreamPost *m_post;
    QLabel *m_userPicLabel;
    QLabel *m_postLabel;
    bool m_triedBothIcons;
    QMap<QNetworkReply*, RequestType> m_outstandingNetworkRequests;
};

} // namespace GUI

#endif // STREAMPOSTWIDGET_H
