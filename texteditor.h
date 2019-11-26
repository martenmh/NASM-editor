#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

//#include "numberline.h"
#include <QWidget>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextEdit>


class TextEditor : public QPlainTextEdit
{

public:
    TextEditor(QWidget *parent);

    void numberLinePaintEvent(QPaintEvent *event);
    int numberLineAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateNumberLineWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateNumberLineArea(const QRect &, int);
private:
    QWidget *numberLine;
};

#endif // TEXTEDITOR_H
