#ifndef SPELLTEXTEDIT_H
#define SPELLTEXTEDIT_H

#include <QPlainTextEdit>
#include <QContextMenuEvent>
#include <aspell.h>

class SpellTextEdit : public QPlainTextEdit
{
Q_OBJECT
public:
    explicit SpellTextEdit(QWidget *parent = 0);
    ~SpellTextEdit();


signals:

public slots:

private slots:

private:
    void contextMenuEvent(QContextMenuEvent *e);
    AspellSpeller *m_speller;

};


#endif // SPELLTEXTEDIT_H
