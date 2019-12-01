#include "numberline.h"
#include <memory>
NumberLine::NumberLine(TextEditor *editor) : QWidget(editor) {
    this->editor = editor;
    Lines = new QHash<int, info>();
}

NumberLine::~NumberLine(){
    delete Lines;
}

QSize NumberLine::sizeHint() const {
       return QSize(editor->numberLineAreaWidth(), 0);
}


void NumberLine::paintEvent(QPaintEvent *event) {
   editor->numberLinePaintEvent(event);
}
#include <QPoint>
#include <QDebug>

void NumberLine::mousePressEvent ( QMouseEvent * event ){
    int pressedNumber = (event->pos().y() + 18) / (height / maxNumber);

    /* Toggle breakpoint (reverse current breakpoint state) */
    Lines->find(pressedNumber).value().breakPointEnabled = !Lines->find(pressedNumber).value().breakPointEnabled;
    /* Repaint */
    this->update();
}

void NumberLine::setTextSize(int n, int h){
    /* Delete the last line if the line count is larger
     * than the calculated line count from the paint event */
    if(Lines->count() > n)
        Lines->remove(Lines->count());

    maxNumber = n;
    height = h;
}



