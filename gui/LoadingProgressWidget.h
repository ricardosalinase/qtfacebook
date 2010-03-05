#ifndef LOADINGPROGRESSWIDGET_H
#define LOADINGPROGRESSWIDGET_H

#include <QWidget>
#include <QProgressBar>

namespace GUI {

class LoadingProgressWidget : public QWidget
{
Q_OBJECT
public:
    explicit LoadingProgressWidget(const QString& label, int height = 50,
                                   int width = 100, QWidget *parent = 0);

signals:

public slots:

private:
    QProgressBar *m_progressBar;

};

} // namespace GUI

#endif // LOADINGPROGRESSWIDGET_H
