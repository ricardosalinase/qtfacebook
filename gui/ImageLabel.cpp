#include "ImageLabel.h"

namespace GUI {

ImageLabel::ImageLabel(const QString& urlString, QWidget *parent) :
        QLabel(parent),
        m_urlString(urlString)
{
}

void ImageLabel::mousePressEvent( QMouseEvent */*ev*/ ) {
    emit userClickedImage(m_urlString);
}

QString& ImageLabel::getUrlString() {
    return m_urlString;
}

} // namespace GUI
