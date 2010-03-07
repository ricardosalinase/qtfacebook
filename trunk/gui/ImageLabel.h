#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>


namespace GUI {

class ImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageLabel(const QString& urlString, QWidget *parent = 0);
    QString& getUrlString();

signals:
    void userClickedImage(QString urlString);

private:
    QString m_urlString;

protected:
    virtual void mousePressEvent( QMouseEvent *ev);

};

} // namespace GUI

#endif // IMAGELABEL_H
