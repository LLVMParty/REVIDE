#include "BitcodeHighlighter.h"

BitcodeHighlighter::BitcodeHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Styling:
    // - Light theme base: https://coolors.co/333f47-1ba7b3-8799c4-fefff7-dfdbbe-f2edd7-e90b55-80b700-a34784
    // - Dark theme base: https://coolors.co/ceff1a-bbb83c-82816d-706eb0-141515-161b1f-1c262e-52f2ed-473198-945aaf

    // keywordFormat.setForeground(QBrush(HEXColorToRGBColor(0x333F47))); // light theme foreground
    keywordFormat.setForeground(QBrush(HEXColorToRGBColor(0x82816D))); // dark theme foreground
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

    // instructionFormat.setForeground(QBrush(HEXColorToRGBColor(0x80B700))); // light theme foreground
    instructionFormat.setForeground(QBrush(HEXColorToRGBColor(0x52F2ED))); // dark theme foreground
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

    // variableFormat.setForeground(QBrush(HEXColorToRGBColor(0xA34784))); // light theme foreground
    variableFormat.setForeground(QBrush(HEXColorToRGBColor(0xBBB83C))); // dark theme foreground
    variableFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("%[0-9a-zA-Z\\._]+"));
    rule.format = variableFormat;
    highlightingRules.append(rule);

    // constantFormat.setForeground(QBrush(HEXColorToRGBColor(0xE90B55))); // light theme foreground
    constantFormat.setForeground(QBrush(HEXColorToRGBColor(0xCEFF1A))); // dark theme foreground
    constantFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral(" [0-9-]+"));
    rule.format = constantFormat;
    highlightingRules.append(rule);

    // integerTypeFormat.setForeground(QBrush(HEXColorToRGBColor(0x8799C4))); // light theme foreground
    integerTypeFormat.setForeground(QBrush(HEXColorToRGBColor(0x706EB0))); // dark theme foreground
    integerTypeFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("i[1-9][0-9]?"));
    rule.format = integerTypeFormat;
    highlightingRules.append(rule);

    // complexPointerTypeFormat.setForeground(QBrush(HEXColorToRGBColor(0x8799C4))); // light theme foreground
    complexPointerTypeFormat.setForeground(QBrush(HEXColorToRGBColor(0x706EB0))); // dark theme foreground
    complexPointerTypeFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("[%i]{1}[0-9a-zA-Z\\._]+\\*"));
    rule.format = complexPointerTypeFormat;
    highlightingRules.append(rule);

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    // quotationFormat.setForeground(QBrush(QColor(70, 93, 104))); // light theme foreground
    quotationFormat.setForeground(QBrush(HEXColorToRGBColor(0x4D6774))); // dark theme foreground
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setFontWeight(QFont::Bold);
    functionFormat.setForeground(QBrush(HEXColorToRGBColor(0x1BA7B3)));
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("@"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // alignFormat.setForeground(QBrush(HEXColorToRGBColor(0x333F47))); // light theme foreground
    alignFormat.setForeground(QBrush(HEXColorToRGBColor(0x98579F))); // dark theme foreground
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
