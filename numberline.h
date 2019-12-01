#ifndef LINENUMBER_H
#define LINENUMBER_H

#include <QWidget>
#include <QVector>
#include <QHash>
#include "texteditor.h"
class NumberLine : public QWidget
{

public:

    struct info{
        int top, bottom, left, right;
        bool breakPointEnabled;
        /* Todo (maybe) collapsable macros & lines */
        bool collapsed;
        /* Collapse at line::number until collapseLineEnd */
        int collapseLineEnd;
    };

    NumberLine(TextEditor *editor);
    ~NumberLine();

    QSize sizeHint() const override;
    void setTextSize(int, int);

    /* Contains all info on each line */
    QHash<int, info> *Lines;
    /* Simple break point array containing all breakpoint locations. */
    QVector<int> *breakPointNumbers;
protected:
    void paintEvent(QPaintEvent *event) override;
private:


    /* Set breakpoint or (Maybe TODO?) collapse depending on the x pos */
    void mousePressEvent ( QMouseEvent * event ) override;

    TextEditor *editor;
    int maxNumber;
    int height;
};

#endif // LINENUMBER_H
