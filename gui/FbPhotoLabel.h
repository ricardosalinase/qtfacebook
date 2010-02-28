#ifndef FBPHOTOLABEL_H
#define FBPHOTOLABEL_H

#include <QLabel>

#include "data/FbPhoto.h"

namespace GUI {

class FbPhotoLabel : public QLabel
{
    Q_OBJECT
public:
    explicit FbPhotoLabel(const QString& pid, QWidget *parent = 0);

signals:
    void userClickedImage(QString pid);

private:
    QString m_pid;

protected:
    virtual void mousePressEvent( QMouseEvent *ev);

};

} // namespace GUI

#endif // FBPHOTOLABEL_H
