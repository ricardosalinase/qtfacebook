#ifndef FBALBUMVIEWWIDGET_H
#define FBALBUMVIEWWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCloseEvent>

#include "data/FbAlbum.h"
#include "data/FbPhoto.h"
#include "api/factory.h"
#include "gui/LoadingProgressWidget.h"
#include "gui/FbPhotoLabel.h"

namespace GUI {

class FbAlbumViewWidget : public QWidget
{
Q_OBJECT
public:
    explicit FbAlbumViewWidget(const QString& albumId, QWidget *parent = 0);
    explicit FbAlbumViewWidget(DATA::FbAlbum *album, QWidget *parent = 0);
    ~FbAlbumViewWidget();

signals:
    void closed(GUI::FbAlbumViewWidget*);

public slots:

private slots:
    void gotNetworkReply(QNetworkReply *reply);
    void apiFqlGetAlbums(API::FQL::GetAlbums *method);
    void getAlbumsFailed(API::FQL::GetAlbums *method);
    void apiFqlGetPhotos(API::FQL::GetPhotos *method);
    void getPhotosFailed(API::FQL::GetPhotos *method);
    void userClickedPhoto(DATA::FbPhoto *photo);

private:
    void closeEvent ( QCloseEvent * event );
    void getPhotos();
    void buildDisplay(QList<DATA::FbPhoto *> *photoList);
    DATA::FbAlbum *m_album;
    bool m_isOwner;
    bool m_destroyAlbum;
    API::Factory *m_factory;
    QHBoxLayout *m_mainLayout;
    QGridLayout *m_thumbLayout;
    QNetworkAccessManager *m_nam;
    LoadingProgressWidget *m_progress;
    QMap<QNetworkReply *, GUI::FbPhotoLabel *> m_outstandingRequests;
    bool m_useSmallThumbs;



};

} // namespace GUI

#endif // FBALBUMVIEWWIDGET_H
