#pragma once

#include <QDialog>
#include <memory>

class BitcodeHighlighter;

namespace Ui {
class BitcodeDialog;
}

struct LLVMGlobalContext;

class BitcodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BitcodeDialog(QWidget* parent = nullptr);
    ~BitcodeDialog();
    bool load(const QString& type, const QByteArray& data);

private slots:
    void on_buttonGodbolt_clicked();

private:
    Ui::BitcodeDialog* ui = nullptr;
    std::unique_ptr<LLVMGlobalContext> mContext;
    std::unique_ptr<BitcodeHighlighter> mHighlighter;
};

