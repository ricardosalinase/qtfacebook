#include "SpellSyntaxHighlighter.h"

#include <QDebug>


SpellSyntaxHighlighter::SpellSyntaxHighlighter(AspellSpeller *speller, QTextDocument *parent) :
    QSyntaxHighlighter(parent),
    m_speller(speller)
{
}

SpellSyntaxHighlighter::~SpellSyntaxHighlighter() {

}

void SpellSyntaxHighlighter::highlightBlock(const QString &text)
{
    QRegExp rx("\\b(\\w+)\\b");

    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1)
    {
        // Check spelling of word, apply style
        if (aspell_speller_check(m_speller, rx.cap(1).toAscii().constData(), -1) == 0)
        {
            QTextCharFormat format;
            format.setFontUnderline(true);
            format.setUnderlineColor(Qt::red);
            format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
            setFormat(pos, rx.matchedLength(), format);
        }

        pos += rx.matchedLength();
    }




}
