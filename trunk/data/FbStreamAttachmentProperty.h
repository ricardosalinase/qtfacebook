#ifndef FBSTREAMATTACHMENTPROPERTY_H
#define FBSTREAMATTACHMENTPROPERTY_H

#include <QString>
#include <QUrl>

namespace DATA {

class FbStreamAttachmentProperty
{
public:
    FbStreamAttachmentProperty();
    void setName(QString& name);
    QString& getName();
    void setText(QString& text);
    QString& getText();
    void setHref(QString& href);
    QUrl& getHref();


private:
    QString m_name;
    QString m_text;
    QUrl m_href;


};

} // namespace DATA

#endif // FBSTREAMATTACHMENTPROPERTY_H
