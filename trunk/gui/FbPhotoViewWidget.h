#ifndef FBPHOTOVIEWWIDGET_H
#define FBPHOTOVIEWWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>


#include "data/FbPhoto.h"
#include "gui/LoadingProgressWidget.h"
#include "api/factory.h"

namespace GUI {

class FbPhotoViewWidget : public QWidget
{
Q_OBJECT
public:

    FbPhotoViewWidget(DATA::FbPhoto *photo, QWidget *parent = 0);
    FbPhotoViewWidget(const QString& photoId, QWidget *parent = 0);
    ~FbPhotoViewWidget();
signals:
    void closed(GUI::FbPhotoViewWidget*);
    void userClickedLink(QString link);

public slots:

private slots:
    void gotNetworkReply(QNetworkReply *reply);
    void apiFqlGetPhotos(API::FQL::GetPhotos *method);
    void getPhotosFailed(API::FQL::GetPhotos *method);

private:
    void closeEvent ( QCloseEvent * event );
    void buildDisplay();
    DATA::FbPhoto *m_photo;
    bool m_isOwner;
    QNetworkAccessManager *m_nam;
    LoadingProgressWidget *m_progress;
    bool m_destroyPhoto;
    API::Factory *m_factory;
    QHBoxLayout *m_mainLayout;
    QVBoxLayout *m_rightSideLayout;
};




} // namespace GUI

#endif // FBPHOTOVIEWWIDGET_H
