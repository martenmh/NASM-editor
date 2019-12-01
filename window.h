#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "texteditor.h"
#include "terminal.h"
#include "highlighter.h"

class Window : public QWidget
{
public:
    Window(QWidget *parent);
    void toggleTree();
    void toggleTerminal();
    void toggleTools();
};


#endif // WINDOW_H
