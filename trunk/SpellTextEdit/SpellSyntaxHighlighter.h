#ifndef SPELLSYNTAXHIGHLIGHTER_H
#define SPELLSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <aspell.h>


class SpellSyntaxHighlighter : public QSyntaxHighlighter
{
Q_OBJECT
public:
    explicit SpellSyntaxHighlighter(AspellSpeller *speller, QTextDocument *parent = 0);
    ~SpellSyntaxHighlighter();

protected:
    void highlightBlock(const QString &text);

signals:

public slots:

private:
    AspellSpeller *m_speller;

};

#endif // SPELLSYNTAXHIGHLIGHTER_H
