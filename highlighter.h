#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include<QSyntaxHighlighter>
#include<QRegularExpression>
#include<QVector>
#include<QString>

class Highlighter : QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent);
protected:
    void highlightBlock(const QString &text) override;
private:
    struct HighlightingRule{
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    // Struct for easily changeable colors of QTextCharFormat
    struct CharStyle{
        QColor color;
        QFont::Weight weight;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpresseion;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat labelFormat;
    QTextCharFormat registerFormat;
    QTextCharFormat macroFormat;
    QTextCharFormat macroLabelFormat;
    QTextCharFormat numberFormat;


};

#endif // HIGHLIGHTER_H
