#ifndef AGESTRING_H
#define AGESTRING_H

#include <QString>
#include <QDateTime>


namespace UTIL {

    static QString ageString(QString cTime) {

        QString postedTime;

        QDateTime createTime = QDateTime::fromTime_t(cTime.toUInt());
        uint time_t_createTime = createTime.toTime_t();
        QDateTime now = QDateTime::currentDateTime();
        QDateTime startOfDay(now);
        startOfDay.setTime(QTime(0,0,0,0));

        if (time_t_createTime < startOfDay.toTime_t())
        {
            // Posted earlier than today
            QDateTime startOfYesterday = startOfDay.addDays(-1);

            if (time_t_createTime > startOfYesterday.toTime_t())
            {
                postedTime.append("Yesterday at " +
                                  createTime.toString("hh:mm:ss ap"));
            }
            else
            {
                QDateTime oneWeekAgo = startOfDay.addDays(-6);

                if (time_t_createTime < oneWeekAgo.toTime_t())
                {
                    postedTime.append(createTime.toString("ddd MMMM d 'at' hh:mm:ss ap"));
                }
                else
                {
                    postedTime.append(createTime.toString("ddd 'at' hh:mm:ss ap"));
                }
            }
        }
        else // posted today
        {
            QString unit;
            uint time_t_create = createTime.toTime_t();
            uint time_t_now = now.toTime_t();
            uint diff = time_t_now - time_t_create;

            if (diff < 60)
                unit = " seconds ago";
            else if (diff < 3600)
            {
                diff = diff / 60;
                diff > 1 ? unit = " minutes ago" : unit = " minute ago";
            }
            else
            {
                diff = diff / 3600;
                diff > 1 ? unit = " hours ago" : unit = " hour ago";
            }

            postedTime.append(QString::number(diff) + unit);

        }

        return postedTime;

    }



} // namespace UTIL

#endif // AGESTRING_H
