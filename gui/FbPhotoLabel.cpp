#include "FbPhotoLabel.h"

namespace GUI {

FbPhotoLabel::FbPhotoLabel(DATA::FbPhoto *photo, QWidget *parent) :
    QLabel(parent),
    m_photo(photo)
{
}

void FbPhotoLabel::mousePressEvent( QMouseEvent */*ev*/ ) {
    emit userClickedImage(m_photo);
}

DATA::FbPhoto * FbPhotoLabel::getPhoto() {
    return m_photo;
}

} // namespace GUI
