#include "numberline.h"

NumberLine::NumberLine(TextEditor *editor) : QWidget(editor) {
       this->editor = editor;
}

QSize NumberLine::sizeHint() const {
       return QSize(editor->numberLineAreaWidth(), 0);
}


void NumberLine::paintEvent(QPaintEvent *event) {
   editor->numberLinePaintEvent(event);
}
