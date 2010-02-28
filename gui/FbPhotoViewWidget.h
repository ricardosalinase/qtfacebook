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
    FbPhotoViewWidget(const DATA::FbPhoto *photo, const UserInfo *info, QWidget *parent = 0);

signals:

public slots:

private:
    const DATA::FbPhoto *m_photo;
    const UserInfo *m_info;
};

} // namespace GUI

#endif // FBPHOTOVIEWWIDGET_H
