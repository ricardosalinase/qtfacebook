#include "FbCommentManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QDebug>

#include "util/OurUserInfo.h"

namespace GUI {

FbCommentManager::FbCommentManager(const QString& id, FbType type, bool isOwner, QWidget *parent) :
    QWidget(parent),
    m_idType(type),
    m_id(id),
    m_isOwner(isOwner)
{

    m_factory = new API::Factory(this);
    connect(m_factory, SIGNAL(apiStreamAddComment(API::Stream::AddComment*)),
           this, SLOT(apiStreamAddComment(API::Stream::AddComment*)));
    connect(m_factory, SIGNAL(apiStreamAddCommentFailed(API::Stream::AddComment*)),
            this, SLOT(apiStreamAddCommentFailed(API::Stream::AddComment*)));
    connect(m_factory, SIGNAL(apiFqlGetComments(API::FQL::GetComments*)),
            this, SLOT(gotComments(API::FQL::GetComments*)));
    connect(m_factory, SIGNAL(apiFqlGetCommentsFailed(API::FQL::GetComments*)),
            this, SLOT(getCommentsFailed(API::FQL::GetComments*)));
    connect(m_factory, SIGNAL(apiStreamRemoveComment(API::Stream::RemoveComment*)),
            this, SLOT(apiStreamRemoveComment(API::Stream::RemoveComment*)));
    connect(m_factory, SIGNAL(apiStreamRemoveCommentFailed(API::Stream::RemoveComment*)),
            this, SLOT(apiStreamRemoveCommentFailed(API::Stream::RemoveComment*)));
    connect(m_factory, SIGNAL(apiCommentsAdd(API::Comments::Add*)),
            this, SLOT(apiCommentsAdd(API::Comments::Add*)));
    connect(m_factory, SIGNAL(apiCommentsAddFailed(API::Comments::Add*)),
            this, SLOT(apiCommentsAddFailed(API::Comments::Add*)));
    connect(m_factory, SIGNAL(apiCommentsRemove(API::Comments::Remove*)),
            this, SLOT(apiCommentsRemove(API::Comments::Remove*)));
    connect(m_factory, SIGNAL(apiCommentsRemoveFailed(API::Comments::Remove*)),
            this, SLOT(apiCommentsRemoveFailed(API::Comments::Remove*)));

    m_commentTimer = new QTimer(this);
    connect(m_commentTimer, SIGNAL(timeout()),
            this, SLOT(getComments()));

    //setSizePolicy(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);

    m_commentScrollArea = new QScrollArea();
    m_commentContainer = new QWidget();
    QVBoxLayout *commentLayout = new QVBoxLayout();
    commentLayout->addStretch();
    m_commentContainer->setLayout(commentLayout);
    m_commentScrollArea->setWidget(m_commentContainer);
    m_commentScrollArea->setWidgetResizable(true);
    m_commentScrollArea->setVisible(false);
    m_commentScrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
    mainLayout->addWidget(m_commentScrollArea,2);
    mainLayout->addStretch();
    m_progressWidget = new QWidget();
    QHBoxLayout *progressWidgetLayout = new QHBoxLayout();
    progressWidgetLayout->setMargin(0);

    QProgressBar *bar = new QProgressBar();
    bar->setMinimum(0);
    bar->setMaximum(0);
    bar->setMaximumHeight(10);
    QLabel *l = new QLabel("Updating Comments");
    l->setStyleSheet("font-size: 8pt;");

    progressWidgetLayout->addWidget(l);
    progressWidgetLayout->addWidget(bar);
    m_progressWidget->setLayout(progressWidgetLayout);

    QHBoxLayout *hbl2 = new QHBoxLayout();
    l = new QLabel();
    l->setMinimumHeight(15);
    hbl2->addWidget(l);
    hbl2->addWidget(m_progressWidget);
    mainLayout->addLayout(hbl2);


    m_commentEdit = new QTextEdit();
    m_commentEdit->setVisible(false);
    m_commentEdit->setAutoFillBackground(true);
    QPalette palette = m_commentEdit->palette();
    palette.setColor(QPalette::Base,QColor(228, 232, 248));
    m_commentEdit->setPalette(palette);
    //m_commentEdit->setStyleSheet("background-color : #e4e8f8");
    m_commentEdit->setAcceptRichText(false);
    m_commentEdit->setFrameStyle(QFrame::Panel);
    m_commentEdit->setFrameShadow(QFrame::Sunken);
    m_commentEdit->setMaximumHeight(75);
    m_commentEdit->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    mainLayout->addWidget(m_commentEdit,0);

    m_addCommentButton = new QPushButton("Add Comment");
    m_addCommentButton->setVisible(false);
    mainLayout->addWidget(m_addCommentButton, 0);
    connect(m_addCommentButton, SIGNAL(clicked()),
            this, SLOT(commentButtonClicked()));

    setLayout(mainLayout);
    getComments();


}

FbCommentManager::~FbCommentManager() {

    while (!m_commentList.empty())
    {
        DATA::StreamComment *c = m_commentList.takeFirst();
        delete c;
    }



}

void FbCommentManager::getComments()
{
    m_progressWidget->setVisible(true);
    API::Method *method = m_factory->createMethod("fql.multiquery.getComments");

    if (m_idType == PostId)
        method->setArgument("post_id", m_id);
    else
        method->setArgument("object_id", m_id);

    bool rc = method->execute();
    if (!rc)
    {
        qDebug() << "FbCommentManager::getComments(); fql.multiquery.getComments failed to execute: " << method->errorString();
        delete method;
    }


}
void FbCommentManager::gotComments(API::FQL::GetComments *method) {

    QList<DATA::StreamComment *> cList = method->getCommentList();
    int num = cList.size();

    // TODO: Should be only get the comments we don't have?

    // This is the *only* way I've found to make this work
    // otherwise the viewport of the QScrollArea doesn't repaint
    // and you end up with artifacts of old comments
    m_commentScrollArea->takeWidget();
    delete m_commentContainer;
    while (!m_commentList.empty())
    {
        DATA::StreamComment *c = m_commentList.takeFirst();
        delete c;
    }

    m_commentContainer = new QWidget();
    m_commentLayout = new QVBoxLayout();

    CommentWidget *last = 0;
    while(!cList.empty())
    {
        DATA::StreamComment *c = cList.takeFirst();
        m_commentList.append(c);

        bool canDelete = false;
        if(m_isOwner || c->getFromId() == UTIL::OurUserInfo::getInstance()->getUID())
        {
            canDelete = true;
        }

        CommentWidget *cw = new CommentWidget(c, canDelete);
        //cw->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_commentLayout->addWidget(cw);
        connect(cw, SIGNAL(userClickedDelete(GUI::CommentWidget*)),
                this, SLOT(userDeletedComment(GUI::CommentWidget*)));
        last = cw;
    }
    m_commentLayout->addStretch();
    m_commentContainer->setLayout(m_commentLayout);
    m_commentScrollArea->setWidget(m_commentContainer);


    if (num)
    {
        m_commentScrollArea->setVisible(true);
        m_commentScrollArea->ensureWidgetVisible(last);
        emit updatedComments();
    }
    else
    {
        m_commentScrollArea->setVisible(false);
    }

    delete method;
    m_progressWidget->setVisible(false);
    emit updatedComments();
    m_commentTimer->start(60000);
}

void FbCommentManager::getCommentsFailed(API::FQL::GetComments *method) {

    qDebug() << "API::FQL::GetComments Failed. Retrying in 5 seconds";
    m_commentTimer->start(5000);
    //m_progressWidget->setVisible(false);
    delete method;
}

void FbCommentManager::showAddComment()
{
    m_commentEdit->setVisible(true);
    m_addCommentButton->setVisible(true);
}


void FbCommentManager::commentButtonClicked() {

    m_addCommentButton->setDisabled(true);

    // See if there's actually a comment
    QString comment = m_commentEdit->toPlainText();
    if (comment != "")
    {
        if (m_idType == PostId)
        {

            API::Method *method = m_factory->createMethod("stream.addComment");
            method->setArgument("post_id", m_id);
            method->setArgument("comment", comment);
            if (!method->execute())
            {
                qDebug() << "stream.addComment failed to execute: " << method->errorString();
                delete method;
            }
        }
        else
        {
            // have to use a different API method. Annoying.
            API::Method *method = m_factory->createMethod("comments.add");
            method->setArgument("object_id", m_id);
            method->setArgument("text", comment);
            if (!method->execute())
            {
                qDebug() << "comments.add failed to execute: " << method->errorString();
                delete method;
            }
        }
    }
    else
    {
        m_addCommentButton->setEnabled(true);
    }
}

void FbCommentManager::userDeletedComment(GUI::CommentWidget *commentWidget) {

    qDebug() << "Delete comment: " << commentWidget->getComment()->getCommentId();

    if (m_idType == PostId)
    {
        API::Method *method = m_factory->createMethod("stream.removeComment");

        method->setArgument("comment_id",commentWidget->getComment()->getCommentId());
        if(!method->execute())
        {
            qDebug() << "API::Stream::removeComment failed to execute: " << method->getErrorStr();
            delete method;
        }
    }
    else
    {
        // Have to use a different API method. Annoying.
        API::Method *method = m_factory->createMethod("comments.remove");

        method->setArgument("comment_id",commentWidget->getComment()->getCommentId());
        method->setArgument("object_id", m_id);
        if(!method->execute())
        {
            qDebug() << "API::Comments::Remove failed to execute: " << method->getErrorStr();
            delete method;
        }

    }

}

void FbCommentManager::apiStreamAddComment(API::Stream::AddComment *method) {
    qDebug() << "Added Comment";
    m_commentEdit->clear();
    m_addCommentButton->setEnabled(true);
    delete method;
    getComments();

}

void FbCommentManager::apiStreamAddCommentFailed(API::Stream::AddComment *method) {
    qDebug() << "Failed to add Comment";
    m_addCommentButton->setEnabled(true);
    delete method;
}

void FbCommentManager::apiCommentsAdd(API::Comments::Add *method) {
    qDebug() << "Added Comment";
    m_commentEdit->clear();
    m_addCommentButton->setEnabled(true);
    delete method;
    getComments();

}

void FbCommentManager::apiCommentsAddFailed(API::Comments::Add *method) {
    qDebug() << "Failed to add Comment";
    m_addCommentButton->setEnabled(true);
    delete method;
}

void FbCommentManager::apiStreamRemoveComment(API::Stream::RemoveComment *method) {

    qDebug() << "Removed Comment";
    delete method;
    getComments();

}

void FbCommentManager::apiStreamRemoveCommentFailed(API::Stream::RemoveComment *method) {
    qDebug() << "Failed to remove comment";
    delete method;
}

void FbCommentManager::apiCommentsRemove(API::Comments::Remove *method) {

    qDebug() << "Removed Comment";
    delete method;
    getComments();

}

void FbCommentManager::apiCommentsRemoveFailed(API::Comments::Remove *method) {
    qDebug() << "Failed to remove comment";
    delete method;
}
} // namespace GUI
