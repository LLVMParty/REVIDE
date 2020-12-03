#include "BitcodeHighlighter.h"

BitcodeHighlighter::BitcodeHighlighter(const BitcodeDialog* style, QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    refreshColors(style);
}

static const char* keywords[] = {
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
    "exact",
    "any",
    "immarg",
};

static const char* instructions[] = {
    "add",
    "load",
    "store",
    "and",
    "or",
    "xor",
    "zext",
    "call",
    "switch",
    "br",
    "icmp",
    "phi",
    "sub",
    "sext",
    "shl",
    "lshr",
    "ashr",
    "select",
    "trunc",
    "eq",
    "ne",
    "sgt",
    "ret",
    "ult",
    "bitcast",
    "getelementptr",
    "inbounds",
    "alloca",
    "mul",
    "urem",
    "ptrtoint",
    "inttoptr",
};

void BitcodeHighlighter::refreshColors(const BitcodeDialog* style)
{
    highlightingRules.clear();

    struct Format
    {
        Format(QTextCharFormat& format)
            : format(format)
        {
        }

        Format&& self() &
        {
            return std::move(*this);
        }

        Format&& italic() &&
        {
            format.setFontItalic(true);
            return self();
        }

        Format&& bold() &&
        {
            format.setFontWeight(QFont::Bold);
            return self();
        }

        Format&& color(const QColor& color) &&
        {
            format.setForeground(color);
            return self();
        }

        Format&& color(const QColorWrapper& color) &&
        {
            format.setForeground(color());
            return self();
        }

    private:
        QTextCharFormat& format;
    };

    auto addRule = [this](const char* regex) {
        highlightingRules.append({ QRegularExpression(regex), QTextCharFormat() });
        if (!highlightingRules.back().pattern.isValid())
        {
            qFatal("Invalid regular expression");
        }
        return Format(highlightingRules.back().format);
    };

    // Rules added later can override previous rules (since they are applied in order)

    // constants: 12, 0x33
    addRule(R"regex(\b\d+\b)regex")
        .color(style->constantColor);
    addRule(R"regex((true|false|void|none|null))regex")
        .color(style->constantColor);

    // Keywords
    {
        QTextCharFormat keywordFormat;
        keywordFormat.setForeground(style->keywordColor());

        for (const QString& pattern : keywords)
            highlightingRules.append({ QRegularExpression(QString("\\b%1\\b").arg(pattern)), keywordFormat });
    }

    // Instructions
    {
        QTextCharFormat instructionFormat;
        instructionFormat.setForeground(style->instructionColor());
        instructionFormat.setFontWeight(QFont::Bold);

        for (const QString& pattern : instructions)
            highlightingRules.append({ QRegularExpression(QString("\\b%1\\b").arg(pattern)), instructionFormat });
    }

    // global variables
    addRule(R"regex((\s|^)[@$]"[^"]+")regex")
        .bold()
        .color(style->globalVariableColor);

    addRule(R"regex((\s|^)[@$][-a-zA-Z$._0-9]+)regex")
        .bold()
        .color(style->globalVariableColor);

    // local variables
    addRule(R"regex((\s|^)%"[^"]+")regex")
        .bold()
        .color(style->localVariableColor);

    addRule(R"regex((\s|^)%[-a-zA-Z$._0-9]+)regex")
        .bold()
        .color(style->localVariableColor);

    // @function( and @"bla blah"(
    addRule(R"regex(\s@"[^"]+"(?=\())regex")
        .bold()
        .color(style->functionColor);

    addRule(R"regex(\s@[-a-zA-Z$._][-a-zA-Z$._0-9]*(?=\())regex")
        .bold()
        .color(style->functionColor);

    // i64
    addRule(R"regex(i\d+)regex")
        .color(style->integerTypeColor);

    // metadata
    addRule(R"regex(!.+$)regex")
        .color(style->metadataColor);
    addRule(R"regex(, !)regex")
        .color(style->metadataColor);
    addRule(R"regex(^attributes #\d+ = .+$)regex")
        .color(style->metadataColor);
    addRule(R"regex( = "[^"]+"$)regex")
        .color(style->metadataColor);

    // Line comments
    addRule(R"regex(^;.+$)regex")
        .color(style->commentColor);
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
