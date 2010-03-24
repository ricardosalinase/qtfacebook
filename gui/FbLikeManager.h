#ifndef FBLIKEMANAGER_H
#define FBLIKEMANAGER_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

#include "data/fbuserinfo.h"
#include "api/factory.h"

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
    void apiFqlGetLikes(API::FQL::GetLikes *);
    void apiFqlGetLikesFailed(API::FQL::GetLikes *);
    void getLikes();


private:
    QString m_objectId;
    bool m_userLikes;
    QLabel *m_label;
    void updateLikes();
    QList<DATA::FbUserInfo *> m_likers;
    API::Factory *m_factory;
    QTimer *m_timer;
};

} // namespace GUI

#endif // FBLIKEMANAGER_H
