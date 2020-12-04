#include "VTILDialog.h"
#include "ui_VTILDialog.h"
#include "FunctionDialog.h"
#include "AbstractFunctionList.h"

#include <vtil/vtil>

#include <sstream>

#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>

VTILDialog::VTILDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::VTILDialog)
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    setWindowFlag(Qt::WindowMinimizeButtonHint, true);
    ui->setupUi(this);

    mFunctionDialog = new FunctionDialog(this);
    //mFunctionDialog->show();
    connect(mFunctionDialog, &FunctionDialog::functionClicked, [this](int index) {
        QMessageBox::information(this, "clicked", QString("%1").arg(index));
    });
}

VTILDialog::~VTILDialog()
{
    delete ui;
}

// https://tuttlem.github.io/2014/08/18/getting-istream-to-work-off-a-byte-array.html
class membuf : public std::basic_streambuf<char>
{
public:
    membuf(const uint8_t* p, size_t l)
    {
        setg((char*)p, (char*)p, (char*)p + l);
    }
};

class memstream : public std::istream
{
public:
    memstream(const uint8_t* p, size_t l)
        : std::istream(&_buffer)
        , _buffer(p, l)
    {
        rdbuf(&_buffer);
    }

private:
    membuf _buffer;
};

using namespace vtil;

struct vtil_highlighter
{
    std::string output;

    enum console_color
    {
        CON_BRG = 15,
        CON_YLW = 14,
        CON_PRP = 13,
        CON_RED = 12,
        CON_CYN = 11,
        CON_GRN = 10,
        CON_BLU = 9,
        CON_DEF = 7,
    };

    template<console_color color = CON_DEF, typename... params>
    void log(const char* fmt, params&&... ps)
    {
        // Later we want to print this message to the console for checking
        auto start_pos = output.size();

        // If string literal with no parameters, use puts instead.
        //
        if (sizeof...(ps) == 0)
            output += fmt;
        else
            output += vtil::format::str(fmt, format::fix_parameter<params>(std::forward<params>(ps))...);

        // For testing also dump to the console
        if (output.back() == '\n')
        {
            // Don't ask
            output.pop_back();
            logger::log(static_cast<logger::console_color>(color), "%s\n", output.data() + start_pos);
            output.push_back('\n');
        }
        else
        {
            logger::log(static_cast<logger::console_color>(color), "%s", output.data() + start_pos);
        }
    }

    void dump(const instruction& ins, const instruction* prev = nullptr)
    {
        // Print stack pointer offset
        //
        if (ins.sp_index)
            log<CON_YLW>("[%d] ", ins.sp_index);
        else
            log("    ");

        if (ins.sp_reset)
            log<CON_PRP>(">%c0x%-4x ", ins.sp_offset >= 0 ? '+' : '-', abs(ins.sp_offset));
        else if ((prev ? prev->sp_offset : 0) == ins.sp_offset)
            log<CON_DEF>("%c0x%-4x  ", ins.sp_offset >= 0 ? '+' : '-', abs(ins.sp_offset));
        else if ((prev ? prev->sp_offset : 0) > ins.sp_offset)
            log<CON_RED>("%c0x%-4x  ", ins.sp_offset >= 0 ? '+' : '-', abs(ins.sp_offset));
        else
            log<CON_BLU>("%c0x%-4x  ", ins.sp_offset >= 0 ? '+' : '-', abs(ins.sp_offset));

        // Print name
        //
        if (ins.is_volatile())
            log<CON_RED>(VTIL_FMT_INS_MNM " ", ins.base->to_string(ins.access_size())); // Volatile instruction
        else
            log<CON_BRG>(VTIL_FMT_INS_MNM " ", ins.base->to_string(ins.access_size())); // Non-volatile instruction

        // Print each operand
        //
        for (auto& op : ins.operands)
        {
            if (op.is_register())
            {
                if (op.reg().is_stack_pointer())
                    log<CON_PRP>(VTIL_FMT_INS_OPR " ", op.reg()); // Stack pointer
                else if (op.reg().is_physical())
                    log<CON_BLU>(VTIL_FMT_INS_OPR " ", op.reg()); // Any hardware/special register
                else
                    log<CON_GRN>(VTIL_FMT_INS_OPR " ", op.reg()); // Virtual register
            }
            else
            {
                fassert(op.is_immediate());

                if (ins.base->memory_operand_index != -1 &&
                    &ins.operands[size_t(ins.base->memory_operand_index) + 1] == &op &&
                    ins.operands[ins.base->memory_operand_index].reg().is_stack_pointer())
                {
                    if (op.imm().i64 >= 0)
                        log<CON_YLW>(VTIL_FMT_INS_OPR " ", format::hex(op.imm().i64)); // External stack
                    else
                        log<CON_BRG>(VTIL_FMT_INS_OPR " ", format::hex(op.imm().i64)); // VM stack
                }
                else
                {
                    log<CON_CYN>(VTIL_FMT_INS_OPR " ", format::hex(op.imm().i64)); // Any immediate
                }
            }
        }

        // End line
        //
        log("\n");
    }

    void dump(const basic_block* blk, std::set<const basic_block*>* visited = nullptr)
    {
        logger::scope_padding _p(4);

        bool blk_visited = visited ? visited->contains(blk) : false;

        auto end_with_bool = [this](bool b) {
            if (b)
                log<CON_GRN>("Y\n");
            else
                log<CON_RED>("N\n");
        };

        log<CON_DEF>("Entry point VIP:       ");
        log<CON_CYN>("0x%llx\n", blk->entry_vip);
        log<CON_DEF>("Stack pointer:         ");
        if (blk->sp_offset < 0)
            log<CON_RED>("%s\n", format::hex(blk->sp_offset));
        else
            log<CON_GRN>("%s\n", format::hex(blk->sp_offset));
        log<CON_DEF>("Already visited?:      ");
        end_with_bool(blk_visited);
        log<CON_DEF>("------------------------\n");

        if (blk_visited)
            return;

        // Print each instruction
        //
        int ins_idx = 0;
        bool no_disasm = false;
        for (auto it = blk->begin(); !it.is_end(); ++it, ins_idx++)
        {
            // If vemit, try to disassmble if not done already.
            //
            if (it->base->name == "vemit")
            {
                if (!no_disasm)
                {
                    std::vector<uint8_t> bytes;
                    for (auto it2 = it; !it2.is_end(); it2++)
                    {
                        if (it2->base->name != "vemit")
                            break;
                        uint8_t* bs = (uint8_t*)&it2->operands[0].imm().u64;
                        bytes.insert(bytes.end(), bs, bs + it2->operands[0].size());
                    }

                    if (bytes.size())
                    {
                        if (it.block->owner->arch_id == architecture_amd64)
                        {
                            auto dasm = amd64::disasm(bytes.data(), it->vip == invalid_vip ? 0 : it->vip, bytes.size());
                            for (auto& ins : dasm)
                                log<CON_YLW>("; %s\n", ins);
                        }
                        else
                        {
                            auto dasm = arm64::disasm(bytes.data(), it->vip == invalid_vip ? 0 : it->vip, bytes.size());
                            for (auto& ins : dasm)
                                log<CON_YLW>("; %s\n", ins);
                        }
                    }
                    no_disasm = true;
                }
            }
            else
            {
                no_disasm = false;
            }

            // Print string context if any.
            //
            if (it->context.has<std::string>())
            {
                const std::string& cmt = it->context;
                log<CON_GRN>("// %s\n", cmt);

                // Skip if nop.
                //
                if (it->base == &ins::nop)
                    continue;
            }

            log<CON_BLU>("%04d: ", ins_idx);
            if (it->vip == invalid_vip)
                log<CON_DEF>("[ PSEUDO ] ");
            else
                log<CON_DEF>("[%08x] ", (uint32_t)it->vip);
            dump(*it, it.is_begin() ? nullptr : &*std::prev(it));
        }

        // Dump each branch as well
        //
        if (visited)
        {
            visited->insert(blk);
            for (auto& child : blk->next)
                dump(child, visited);
        }
    }

    void dump(const routine* routine)
    {
        std::set<const basic_block*> vs;
        dump(routine->entry_point, &vs);
        while (output.back() == '\n')
            output.pop_back();
    }
};

bool VTILDialog::load(const QByteArray& data, QString& errorMessage)
{
    memstream dataStream((const uint8_t*)data.constData(), data.size());
    vtil::routine* rtn;
    vtil::deserialize(dataStream, rtn);
    vtil_highlighter highlighter;
    highlighter.dump(rtn);
    ui->plainTextBitcode->setPlainText(QString::fromStdString(highlighter.output));
    return true;
}

void VTILDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::StyleChange)
    {
// TODO
#if 0
        if (mHighlighter)
        {
            ensurePolished();
            mHighlighter->setDocument(nullptr);
            mHighlighter->setDocument(ui->plainTextBitcode->document());
        }
#endif
    }
    QDialog::changeEvent(event);
}

void VTILDialog::closeEvent(QCloseEvent* event)
{
    mFunctionDialog->close();
    return QDialog::closeEvent(event);
}
