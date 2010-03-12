#include "HyperLink.h"

#include <QRegExp>
#include <QStringList>
#include <QDebug>

namespace UTIL {

QString hyperLink(const QString& plainText, int maxLength)
{
    QString linkedText = plainText;
    QRegExp rx("(\\b(?:(?:https?|ftp|file)://|www\\.|ftp\\.)"
               "(?:\\([-A-Z0-9+&@#/%=~_|\\$\\?!:,\\.]*\\)"
               "|[-A-Z0-9+&@#/%=~_|\\$\\?!:,\\.])*"
               "(?:\\([-A-Z0-9+&@#/%=~_|\\$\\?!:,\\.]*\\)"
               "|[A-Z0-9+&@#/%=~_|\\$]))",Qt::CaseInsensitive);

    QStringList list;
    int pos = 0;

    while ((pos = rx.indexIn(plainText, pos)) != -1) {
        list << rx.cap(1);
        qDebug() << "Matched: " << rx.cap(1);
        pos += rx.matchedLength();
    }

    while(!list.empty())
    {
        QString url = list.takeFirst();
        QString urlText = url;
        if (maxLength && url.length() > maxLength)
        {
            int half = maxLength / 2;
            urlText = urlText.left(half) + "..." + urlText.right(half);
        }
        linkedText.replace(url, " <a href=\"" + url + "\">" + urlText + "</a>");
    }
    linkedText.replace("\n","<BR>");
    return linkedText;

}

} // namespace UTIL
