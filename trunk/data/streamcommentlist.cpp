#include "streamcommentlist.h"

namespace DATA {

StreamComment::StreamComment() {

}

void StreamComment::setCommentId(QString id) {
    m_commentId = id;
}

QString StreamComment::getCommentId() {
    return m_commentId;
}

void StreamComment::setFromId(QString id) {
    m_fromId = id;
}

QString StreamComment::getFromId() {
    return m_fromId;
}

void StreamComment::setTime(QString time) {
    m_time = time;
}

QString StreamComment::getTime() {
    return m_time;
}

void StreamComment::setText(QString text) {
    m_text = text;
}

QString StreamComment::getText() {
    return m_text;
}

StreamCommentList::StreamCommentList() :
        m_canRemove(false),
        m_canPost(false)
{
}

bool StreamCommentList::canRemove() {
    return m_canRemove;
}

void StreamCommentList::canRemove(bool cr) {
    m_canRemove = cr;
}

bool StreamCommentList::canPost() {
    return m_canPost;
}

void StreamCommentList::canPost(bool cp) {
    m_canPost = cp;
}

} // namespace DATA
