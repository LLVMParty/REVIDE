#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include "BitcodeDialog.h"

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class BitcodeHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    BitcodeHighlighter(const BitcodeDialog* style, QTextDocument* parent = 0);

protected:
    void highlightBlock(const QString& text) override;

private:
    QColor HEXColorToRGBColor(uint64_t HEXColor)
    {
        uint8_t RR = (HEXColor >> 16) & 0xff;
        uint8_t GG = (HEXColor >> 8) & 0xff;
        uint8_t BB = HEXColor & 0xff;
        return QColor(RR, GG, BB);
    }

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat instructionFormat;
    QTextCharFormat variableFormat;
    QTextCharFormat constantFormat;
    QTextCharFormat integerTypeFormat;
    QTextCharFormat complexPointerTypeFormat;
    QTextCharFormat alignFormat;
};
