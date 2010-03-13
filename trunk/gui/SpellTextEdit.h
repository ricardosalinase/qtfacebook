#ifndef SPELLTEXTEDIT_H
#define SPELLTEXTEDIT_H

#include <QTextEdit>

namespace GUI {

class SpellTextEdit : public QTextEdit
{
Q_OBJECT
public:
    explicit SpellTextEdit(QWidget *parent = 0);

signals:

public slots:

private slots:
    void userChangedText();
};

} // namespace GUI

#endif // SPELLTEXTEDIT_H
