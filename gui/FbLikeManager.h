#ifndef FBLIKEMANAGER_H
#define FBLIKEMANAGER_H

#include <QWidget>
#include <QLabel>

namespace GUI {

class FbLikeManager : public QWidget
{
Q_OBJECT
public:
    FbLikeManager(const QString& fbObjectId, bool userLikes = false, QWidget *parent = 0);

signals:
    void userChangedLike(bool likes);

public slots:
    void userLikes();
    void userStoppedLiking();
    bool toggleUserLikes();

private:
    QString m_objectId;
    bool m_userLikes;
    QLabel *m_label;
    void updateLikes();

};

} // namespace GUI

#endif // FBLIKEMANAGER_H
