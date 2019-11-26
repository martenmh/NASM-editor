#ifndef LINENUMBER_H
#define LINENUMBER_H

#include <QWidget>
#include "texteditor.h"
class NumberLine : public QWidget
{
public:
    NumberLine(TextEditor *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
private:

    TextEditor *editor;

};

#endif // LINENUMBER_H
