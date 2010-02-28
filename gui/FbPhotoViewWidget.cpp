#include "FbPhotoViewWidget.h"

namespace GUI {

FbPhotoViewWidget::FbPhotoViewWidget(const DATA::FbPhoto *photo, const UserInfo *info, QWidget *parent) :
    QWidget(parent),
    m_photo(photo),
    m_info(info)
{
}

} // namespace GUI
