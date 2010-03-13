#include "SpellTextEdit.h"

#include <QDebug>
#include <QTextCursor>

namespace GUI {

SpellTextEdit::SpellTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    connect(this, SIGNAL(textChanged()),
            this, SLOT(userChangedText()));

}


void SpellTextEdit::userChangedText()
{

    qDebug() << this->textCursor().position();

    QTextCursor cursor = this->textCursor();

    cursor.movePosition(QTextCursor::PreviousCharacter,
                        QTextCursor::KeepAnchor);

    qDebug() << "Previous char: " << cursor.selectedText();

    if (cursor.selectedText() == " ")
    {
        if (cursor.movePosition(QTextCursor::PreviousWord))
        {
            cursor.movePosition(QTextCursor::QTextCursor::EndOfWord,
                                QTextCursor::KeepAnchor);
            qDebug() << "Previous Word: " << cursor.selectedText();



        }



    }

}
} // namespace GUI
