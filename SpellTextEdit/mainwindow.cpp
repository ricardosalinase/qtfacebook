#include "mainwindow.h"
#include "SpellTextEdit.h"
#include "SpellSyntaxHighlighter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SpellTextEdit *edit = new SpellTextEdit();
    this->setCentralWidget(edit);


}

MainWindow::~MainWindow()
{

}
