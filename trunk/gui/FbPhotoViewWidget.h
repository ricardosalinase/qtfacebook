#ifndef FBPHOTOVIEWWIDGET_H
#define FBPHOTOVIEWWIDGET_H

#include <QWidget>

#include "userinfo.h"
#include "data/FbPhoto.h"

namespace GUI {

class FbPhotoViewWidget : public QWidget
{
Q_OBJECT
public:
    FbPhotoViewWidget(DATA::FbPhoto *photo, UserInfo *info, QWidget *parent = 0);

signals:

public slots:

private:
    DATA::FbPhoto *m_photo;
    UserInfo *m_info;
    bool m_isOwner;
};

} // namespace GUI

#endif // FBPHOTOVIEWWIDGET_H
