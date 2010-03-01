#include "FbPhotoViewWidget.h"

namespace GUI {

FbPhotoViewWidget::FbPhotoViewWidget(DATA::FbPhoto *photo, UserInfo *info, QWidget *parent) :
    QWidget(parent),
    m_photo(photo),
    m_info(info),
    m_isOwner(false)
{

    if (m_info->getUID() == photo->getOwnerId())
        m_isOwner = true;




}

} // namespace GUI
