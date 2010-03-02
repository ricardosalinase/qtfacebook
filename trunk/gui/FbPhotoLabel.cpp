#include "FbPhotoLabel.h"

namespace GUI {

FbPhotoLabel::FbPhotoLabel(const QString& pid, QWidget *parent) :
        QLabel(parent),
        m_pid(pid)
{
}

void FbPhotoLabel::mousePressEvent( QMouseEvent */*ev*/ ) {
    emit userClickedImage(m_pid);
}

QString& FbPhotoLabel::getPid() {
    return m_pid;
}

} // namespace GUI
