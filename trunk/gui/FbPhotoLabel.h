#ifndef FBPHOTOLABEL_H
#define FBPHOTOLABEL_H

#include <QLabel>

#include "data/FbPhoto.h"

namespace GUI {

class FbPhotoLabel : public QLabel
{
    Q_OBJECT
public:
    explicit FbPhotoLabel(DATA::FbPhoto *photo, QWidget *parent = 0);
    DATA::FbPhoto * getPhoto();

signals:
    void userClickedImage(DATA::FbPhoto *);

private:
    DATA::FbPhoto *m_photo;

protected:
    virtual void mousePressEvent( QMouseEvent *ev);

};

} // namespace GUI

#endif // FBPHOTOLABEL_H
