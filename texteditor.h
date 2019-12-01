#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

//#include "numberline.h"
#include <QWidget>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QTextBlock>


class NumberLine;
class TextEditor : public QPlainTextEdit
{

public:
    TextEditor(QWidget *parent);

    void numberLinePaintEvent(QPaintEvent *event);
    int numberLineAreaWidth();

    QTextBlock block;
protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateNumberLineWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateNumberLineArea(const QRect &, int);
private:
    NumberLine *numberLine;
};

#endif // TEXTEDITOR_H
