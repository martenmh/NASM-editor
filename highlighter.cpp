#include "highlighter.h"


Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter (parent)
{
    HighlightingRule rule;
    // First set the weight & color for the keywords
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontItalic(QFont::StyleItalic);
    /* Define the keywords of the language
     * As im implementing syntax highlighting for x86_64
     * this includes, the instructions, reserving data keywords, defining data keywords. All sections (section .bss, .text, .data), global.
     * call, syscall, ret, pop, push.
     */
    const QString keywordPatterns[] = {
        // Instruction set:
        QStringLiteral("\\bmov\\b"), QStringLiteral("\\bcmp\\b"), QStringLiteral("\\badd\\b"),
        QStringLiteral("\\bsub\\b"), QStringLiteral("\\bdiv\\b"), QStringLiteral("\\bmul\\b"),
        QStringLiteral("\\bidiv\\b"), QStringLiteral("\\bimul\\b"),
        QStringLiteral("\\bneg\\b"), QStringLiteral("\\binc\\b"), QStringLiteral("\dec\\b"),
        QStringLiteral("\\badc\\b"), QStringLiteral("\\bsbb\\b"), QStringLiteral("\\bequ\\b"),
        // Jumps:
        QStringLiteral("\\bjmp\\b"), QStringLiteral("\\bje\\b"),
        QStringLiteral("\\bjne\\b"), QStringLiteral("\\bjg\\b"), QStringLiteral("\\bjge\\b"),
        QStringLiteral("\\bjl\\b"), QStringLiteral("\\bjle\\b"), QStringLiteral("\\bjz\\b"),
        QStringLiteral("\\bjnz\\b"), QStringLiteral("\\bjo\\b"), QStringLiteral("\\bjno\\b"),
        QStringLiteral("\\bjs\\b"), QStringLiteral("\\bjns\\b"),
        // Reserving data:
        QStringLiteral("\\bresb\\b"), QStringLiteral("\\bresw\\b"),QStringLiteral("\\bresd\\b"),
        QStringLiteral("\\bresq\\b"),QStringLiteral("\\brest\\b"),
        // Defining data:
        QStringLiteral("\\bdb\\b"), QStringLiteral("\\bdw\\b"),QStringLiteral("\\bdd\\b"),
        QStringLiteral("\\bdq\\b"), QStringLiteral("\\bdt\\b"),
        // Sections:
        QStringLiteral("\\bsection\\b"), QStringLiteral(".text\\b"),QStringLiteral(".data\\b"),
        QStringLiteral(".rodata\\b"),QStringLiteral(".bss\\b"),
        // Calls:
        QStringLiteral("\\bsyscall\\b"), QStringLiteral("\\bcall\\b"),QStringLiteral("\\bret\\b"),
        // Stack:
        QStringLiteral("\\bpop\\b"), QStringLiteral("\\bpush\\b")
    };

    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Labels
    labelFormat.setFontWeight(QFont::Bold);
    labelFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("([\\w\\d\\s%]+):"), QRegularExpression::CaseInsensitiveOption);
    rule.format = labelFormat;
    highlightingRules.append(rule);

    // Numbers & Decimals
    numberFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("(?<![A-Za-z0-9])[0-9]+.?"), QRegularExpression::CaseInsensitiveOption);
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Macros: %macro %endmacro %include
    macroFormat.setForeground(Qt::white);
    rule.pattern = QRegularExpression(QStringLiteral("(?<![%])%(\\w+)"), QRegularExpression::CaseInsensitiveOption);
    rule.format = macroFormat;
    highlightingRules.append(rule);

    // Single line comments: ; comment
    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegularExpression(QStringLiteral(";.*"), QRegularExpression::CaseInsensitiveOption);
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Double Quotations: "quotation"
    quotationFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("\"(.*)\""), QRegularExpression::CaseInsensitiveOption);
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Single Quotations: 'quotation' (uses same style as quotationFormat)
    rule.pattern = QRegularExpression(QStringLiteral("\'(.*)\'"), QRegularExpression::CaseInsensitiveOption);
    highlightingRules.append(rule);

    // Registers without case sensitivity: mov rax, RDI
    registerFormat.setForeground(Qt::blue);
    const QString registerPatterns[] = {
        QStringLiteral("\\brax\\b"), QStringLiteral("\\beax\\b"), QStringLiteral("\\bax\\b"), QStringLiteral("\\bal\\b"),
        QStringLiteral("\\brbx\\b"), QStringLiteral("\\bebx\\b"), QStringLiteral("\\bbx\\b"), QStringLiteral("\\bbl\\b"),
        QStringLiteral("\\brcx\\b"), QStringLiteral("\\becx\\b"), QStringLiteral("\\bcx\\b"), QStringLiteral("\\bcl\\b"),
        QStringLiteral("\\brdx\\b"), QStringLiteral("\\bedx\\b"), QStringLiteral("\\bdx\\b"), QStringLiteral("\\bdl\\b"),
        QStringLiteral("\\brsi\\b"), QStringLiteral("\\besi\\b"), QStringLiteral("\\bsi\\b"), QStringLiteral("\\bsil\\b"),
        QStringLiteral("\\brdi\\b"), QStringLiteral("\\bedi\\b"), QStringLiteral("\\bdi\\b"), QStringLiteral("\\bdil\\b"),
        QStringLiteral("\\brbp\\b"), QStringLiteral("\\bebp\\b"), QStringLiteral("\\bbp\\b"), QStringLiteral("\\bbpl\\b"),
        QStringLiteral("\\brsp\\b"), QStringLiteral("\\besp\\b"), QStringLiteral("\\bsp\\b"), QStringLiteral("\\bspl\\b"),
        QStringLiteral("\\br8\\b"),  QStringLiteral("\\br8d\\b"), QStringLiteral("\\br8w\\b"), QStringLiteral("\\br8b\\b"),
        QStringLiteral("\\br9\\b"),  QStringLiteral("\\br9d\\b"), QStringLiteral("\\br9w\\b"), QStringLiteral("\\br9b\\b"),
        QStringLiteral("\\br10\\b"), QStringLiteral("\\br10d\\b"), QStringLiteral("\\br10w\\b"), QStringLiteral("\\br10b\\b"),
        QStringLiteral("\\br11\\b"), QStringLiteral("\\br11d\\b"), QStringLiteral("\\br11w\\b"), QStringLiteral("\\br11b\\b"),
        QStringLiteral("\\br12\\b"), QStringLiteral("\\br12d\\b"), QStringLiteral("\\br12w\\b"), QStringLiteral("\\br12b\\b"),
        QStringLiteral("\\br13\\b"), QStringLiteral("\\br13d\\b"), QStringLiteral("\\br13w\\b"), QStringLiteral("\\br13b\\b"),
        QStringLiteral("\\br14\\b"), QStringLiteral("\\br14d\\b"), QStringLiteral("\\br14w\\b"), QStringLiteral("\\br14b\\b"),
        QStringLiteral("\\br15\\b"), QStringLiteral("\\br15d\\b"), QStringLiteral("\\br15w\\b"), QStringLiteral("\\br15b\\b"),
    };
    for (const QString &pattern : registerPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = registerFormat;
        highlightingRules.append(rule);
    }

    // MacroLabel: %macro print
    macroLabelFormat.setForeground(Qt::yellow);
    rule.pattern = QRegularExpression(QStringLiteral("(?<=\\%macro\\s)(\\w+)(?=\\s\\d)"), QRegularExpression::CaseInsensitiveOption);
    rule.format = macroLabelFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\w+(?=\\s+equ)"), QRegularExpression::CaseInsensitiveOption);
    highlightingRules.append(rule);

}

void Highlighter::highlightBlock(const QString &text){
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);
}
