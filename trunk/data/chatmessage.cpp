#include "chatmessage.h"

namespace DATA {

ChatMessage::ChatMessage()
{
}

QString ChatMessage::getMsgId() {
    return m_msgId;
}

void ChatMessage::setMsgId(QString id) {
    m_msgId = id;
}

QString ChatMessage::getFromId() {
    return m_from;
}

void ChatMessage::setFromId(QString id) {
    m_from = id;
}

QString ChatMessage::getToId() {
    return m_to;
}

void ChatMessage::setToId(QString id) {
    m_to = id;
}

QString ChatMessage::getFromName() {
    return m_fromName;
}

void ChatMessage::setFromName(QString from) {
    m_fromName = from;
}

QString ChatMessage::getToName() {
    return m_toName;
}

void ChatMessage::setToName(QString to) {
    m_toName = to;
}

QString ChatMessage::getFromFirstName() {
    return m_fromFirstName;
}

void ChatMessage::setFromFirstName(QString first) {
    m_fromFirstName = first;
}

QString ChatMessage::getToFirstName() {
    return m_toFirstName;
}

void ChatMessage::setToFirstName(QString to) {
    m_toFirstName = to;
}

QString ChatMessage::getText() {
    return m_text;
}

void ChatMessage::setText(QString text) {
    m_text = text;
}



} // namespace DATA
