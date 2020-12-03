#pragma once

#include <QObject>
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
    void refreshColors(const BitcodeDialog* style);

protected:
    void highlightBlock(const QString& text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat multiLineCommentFormat;
    QRegularExpression commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    QRegularExpression commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
};
