#include "texteditor.h"
#include <numberline.h>
TextEditor::TextEditor(QWidget *parent) : QPlainTextEdit (parent)
{
    numberLine = new NumberLine(this);

    connect(this, &TextEditor::blockCountChanged, this, &TextEditor::updateNumberLineWidth);
    connect(this, &TextEditor::updateRequest, this, &TextEditor::updateNumberLineArea);
    connect(this, &TextEditor::cursorPositionChanged, this, &TextEditor::highlightCurrentLine);

    updateNumberLineWidth(0);
    highlightCurrentLine();
}

int TextEditor::numberLineAreaWidth(){
    int digits = 1;
    int max = qMax(1, blockCount());
    while(max >= 10){
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void TextEditor::updateNumberLineWidth(int /* newBlockCount */){
    setViewportMargins(numberLineAreaWidth(), 0,0,0);
}

void TextEditor::updateNumberLineArea(const QRect &rect, int dy){
    if(dy)
        numberLine->scroll(0, dy);
    else
        numberLine->update(0, rect.y(), numberLine->width(), rect.height());

    if(rect.contains(viewport()->rect()))
        updateNumberLineWidth(0);
}

void TextEditor::resizeEvent(QResizeEvent *event){
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    numberLine->setGeometry(QRect(cr.left(), cr.top(), numberLineAreaWidth(), cr.height()));
}

void TextEditor::highlightCurrentLine(){
    QList<QTextEdit::ExtraSelection> extraSelections;

    if(!isReadOnly()){
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::gray).darker(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

#include <QPainter>
#include <QTextBlock>
void TextEditor::numberLinePaintEvent(QPaintEvent *event){
    QPainter painter(numberLine);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, numberLine->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }

}
