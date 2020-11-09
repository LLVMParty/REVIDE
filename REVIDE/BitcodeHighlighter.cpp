#include "BitcodeHighlighter.h"

BitcodeHighlighter::BitcodeHighlighter(const BitcodeDialog* style, QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(style->keywordColor());
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywords[] = {
        // https://github.com/compiler-explorer/compiler-explorer/blob/6eab83562af4c81269222fc0e7b12092884e0912/static/modes/llvm-ir-mode.js#L56
        "acq_rel",
        "acquire",
        "addrspace",
        "alias",
        "align",
        "alignstack",
        "alwaysinline",
        "appending",
        "argmemonly",
        "arm_aapcscc",
        "arm_aapcs_vfpcc",
        "arm_apcscc",
        "asm",
        "atomic",
        "available_externally",
        "blockaddress",
        "builtin",
        "byval",
        "c",
        "catch",
        "caller",
        "cc",
        "ccc",
        "cleanup",
        "coldcc",
        "comdat",
        "common",
        "constant",
        "datalayout",
        "declare",
        "default",
        "define",
        "deplibs",
        "dereferenceable",
        "distinct",
        "dllexport",
        "dllimport",
        "dso_local",
        "dso_preemptable",
        "except",
        "external",
        "externally_initialized",
        "extern_weak",
        "fastcc",
        "filter",
        "from",
        "gc",
        "global",
        "hhvmcc",
        "hhvm_ccc",
        "hidden",
        "initialexec",
        "inlinehint",
        "inreg",
        "inteldialect",
        "intel_ocl_bicc",
        "internal",
        "linkonce",
        "linkonce_odr",
        "localdynamic",
        "localexec",
        "local_unnamed_addr",
        "minsize",
        "module",
        "monotonic",
        "msp430_intrcc",
        "musttail",
        "naked",
        "nest",
        "noalias",
        "nobuiltin",
        "nocapture",
        "noimplicitfloat",
        "noinline",
        "nonlazybind",
        "nonnull",
        "norecurse",
        "noredzone",
        "noreturn",
        "nounwind",
        "optnone",
        "optsize",
        "personality",
        "private",
        "protected",
        "ptx_device",
        "ptx_kernel",
        "readnone",
        "readonly",
        "release",
        "returned",
        "returns_twice",
        "sanitize_address",
        "sanitize_memory",
        "sanitize_thread",
        "section",
        "seq_cst",
        "sideeffect",
        "signext",
        "syncscope",
        "source_filename",
        "speculatable",
        "spir_func",
        "spir_kernel",
        "sret",
        "ssp",
        "sspreq",
        "sspstrong",
        "strictfp",
        "swiftcc",
        "tail",
        "target",
        "thread_local",
        "to",
        "triple",
        "unnamed_addr",
        "unordered",
        "uselistorder",
        "uselistorder_bb",
        "uwtable",
        "volatile",
        "weak",
        "weak_odr",
        "within",
        "writeonly",
        "x86_64_sysvcc",
        "win64cc",
        "x86_fastcallcc",
        "x86_stdcallcc",
        "x86_thiscallcc",
        "zeroext",
        "label",

        // Additional keywords
        "source_filename",
        "nofree",
        "willreturn",
        "nsw",
        "nuw",
        "exact"
    };
    for (const QString& pattern : keywords)
    {
        rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(pattern));
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    instructionFormat.setForeground(style->instructionColor());
    instructionFormat.setFontWeight(QFont::Bold);
    const QString instructions[] = {
        "add", "load", "store", "and", "or", "xor", "zext", "call", "switch", "br", "icmp", "phi", "sub", "sext", "shl", "lshr", "ashr", "select", "trunc", "eq", "ne", "sgt", "ret", "ult", "bitcast", "getelementptr"
    };
    for (const QString& pattern : instructions)
    {
        rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(pattern));
        rule.format = instructionFormat;
        highlightingRules.append(rule);
    }

    variableFormat.setForeground(style->variableColor());
    variableFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("%[0-9a-zA-Z\\._]+"));
    rule.format = variableFormat;
    highlightingRules.append(rule);

    constantFormat.setForeground(style->constantColor());
    constantFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral(" [0-9-]+"));
    rule.format = constantFormat;
    highlightingRules.append(rule);

    integerTypeFormat.setForeground(style->integerTypeColor());
    integerTypeFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("i[1-9][0-9]?"));
    rule.format = integerTypeFormat;
    highlightingRules.append(rule);

    complexPointerTypeFormat.setForeground(style->complexPointerTypeColor());
    complexPointerTypeFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("[%i]{1}[0-9a-zA-Z\\._]+\\*"));
    rule.format = complexPointerTypeFormat;
    highlightingRules.append(rule);

    classFormat.setForeground(style->classColor());
    classFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(style->singleLineCommentColor());
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(style->multiLineCommentColor());

    quotationFormat.setForeground(style->quotationColor());
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setFontWeight(QFont::Bold);
    functionFormat.setForeground(style->functionColor());
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("@"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    alignFormat.setForeground(style->alignColor());
    rule.pattern = QRegularExpression(QStringLiteral("align [0-9-]+"));
    rule.format = alignFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("#[0-9-]+"));
    rule.format = alignFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void BitcodeHighlighter::highlightBlock(const QString& text)
{
    for (const HighlightingRule& rule : qAsConst(highlightingRules))
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
