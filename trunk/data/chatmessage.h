#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QString>

namespace DATA {

class ChatMessage
{
public:
    explicit ChatMessage();

    QString getMsgId();
    void setMsgId(QString id);
    QString getFromId();
    void setFromId(QString id);
    QString getToId();
    void setToId(QString id);
    QString getFromName();
    void setFromName(QString from);
    QString getToName();
    void setToName(QString to);
    QString getFromFirstName();
    void setFromFirstName(QString first);
    QString getToFirstName();
    void setToFirstName(QString to);
    QString getText();
    void setText(QString text);
    QString getClientTime();
    void setClientTime(QString time);

private:
    QString m_msgId;
    QString m_from;
    QString m_to;
    QString m_fromName;
    QString m_toName;
    QString m_fromFirstName;
    QString m_toFirstName;
    QString m_text;
    QString m_clientTime;




};

} // namespace DATA

#endif // CHATMESSAGE_H
