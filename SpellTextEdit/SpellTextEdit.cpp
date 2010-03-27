#include "SpellTextEdit.h"


#include <QDebug>
#include <QTextCursor>
#include <QMenu>

#include "SpellSyntaxHighlighter.h"
#include "ASpellConfig.h"

SpellTextEdit::SpellTextEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
    AspellCanHaveError *possibleError =
            new_aspell_speller(ASpellConfig::instance()->getConfig());

    m_speller = 0;

    if (aspell_error_number(possibleError) != 0)
        qDebug() << "ASpell Error: " << aspell_error_message(possibleError);
    else
        m_speller = to_aspell_speller(possibleError);

    // The document takes ownership and destroys the highlighter
    (new SpellSyntaxHighlighter(m_speller, this->document()));

}

SpellTextEdit::~SpellTextEdit() {

    delete_aspell_speller(m_speller);

}


void SpellTextEdit::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *menu = createStandardContextMenu();
    QMap<QAction *, QString> actionMap;

    // Find the word under the mouse pointer
    QPoint pos = e->pos();
    QTextCursor cursor = this->cursorForPosition(pos);

    cursor.select(QTextCursor::WordUnderCursor);

    QString word = cursor.selectedText();

    if (word != "")
    {
        // User right clicked on a word - see if it's spelled correctly
        if (aspell_speller_check(m_speller, word.toAscii().constData(), -1) == 0)
        {
            menu->addSeparator();

            const AspellWordList * suggestions =
                        aspell_speller_suggest(m_speller, word.toAscii().constData(), -1);

            AspellStringEnumeration * elements = aspell_word_list_elements(suggestions);

            const char *suggestedWord;
            int numWords = 0;
            while ((suggestedWord = aspell_string_enumeration_next(elements)) && numWords < 8)
            {
                numWords++;
                QAction *act = menu->addAction(suggestedWord);
                actionMap.insert(act, suggestedWord);
            }

            delete_aspell_string_enumeration(elements);

        }
    }

    QAction *act = menu->exec(e->globalPos());

    if (act != 0)
    {
        QString suggestion = actionMap.value(act);
        cursor.insertText(suggestion);

    }

    delete menu;

}


