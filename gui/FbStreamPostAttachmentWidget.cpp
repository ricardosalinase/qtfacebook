#include "FbStreamPostAttachmentWidget.h"

namespace GUI {

FbStreamPostAttachmentWidget::FbStreamPostAttachmentWidget(DATA::FbStreamAttachment *attachment, QWidget *parent) :
    QWidget(parent)
{
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam,SIGNAL(finished(QNetworkReply*)),
            this, SLOT(gotNetworkReply(QNetworkReply*)));


    int numMedia = attachment->getMedia().size();

    if (numMedia > 1)
    {

    }
    else
    {

    }


}

} // namespace GUI
