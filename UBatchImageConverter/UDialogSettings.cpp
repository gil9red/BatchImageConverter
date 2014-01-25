#include "UDialogSettings.h"
#include "ui_UDialogSettings.h"

/// PUBLIC
UDialogSettings::UDialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UDialogSettings)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog
                   | Qt::WindowCloseButtonHint);

    connect(ui->pButtonOk, SIGNAL(clicked()), SLOT(ok()));
    connect(ui->pButtonCancel, SIGNAL(clicked()), SLOT(cancel()));
    connect(ui->pButtonResetToDefault, SIGNAL(clicked()), SLOT(resetToDefault()));

    setResult(QDialog::Rejected);

    load();
}

UDialogSettings::~UDialogSettings()
{
    delete ui;
}

/// PRIVATE
void UDialogSettings::load()
{
    bool def;
    bool checked;

    def = UConfig::value("DEFAULT", "IncludeSubFolders").toBool();
    checked = UConfig::value("CURRENT", "IncludeSubFolders", def).toBool();
    ui->cBoxIncludeSubFolders->setChecked(checked);

    def = UConfig::value("DEFAULT", "CloseAtEndFileSearchDialog").toBool();
    checked = UConfig::value("CURRENT", "CloseAtEndFileSearchDialog", def).toBool();
    ui->cBoxCloseAtEndFileSearchDialog->setChecked(checked);

    def = UConfig::value("DEFAULT", "CloseAtEndFileSaveDialog").toBool();
    checked = UConfig::value("CURRENT", "CloseAtEndFileSaveDialog", def).toBool();
    ui->cBoxCloseAtEndFileSaveDialog->setChecked(checked);
}

/// PUBLIC SLOTS
void UDialogSettings::ok()
{
    bool checked;

    checked = ui->cBoxIncludeSubFolders->isChecked();
    UConfig::setValue("CURRENT", "IncludeSubFolders", checked);

    checked = ui->cBoxCloseAtEndFileSearchDialog->isChecked();
    UConfig::setValue("CURRENT", "CloseAtEndFileSearchDialog", checked);

    checked = ui->cBoxCloseAtEndFileSaveDialog->isChecked();
    UConfig::setValue("CURRENT", "CloseAtEndFileSaveDialog", checked);

    setResult(QDialog::Accepted);
    close();
}
void UDialogSettings::cancel()
{
    setResult(QDialog::Rejected);
    close();
}
void UDialogSettings::resetToDefault()
{
    bool checked;

    checked = UConfig::value("DEFAULT", "IncludeSubFolders").toBool();
    ui->cBoxIncludeSubFolders->setChecked(checked);

    checked = UConfig::value("DEFAULT", "CloseAtEndFileSearchDialog").toBool();
    ui->cBoxCloseAtEndFileSearchDialog->setChecked(checked);

    checked = UConfig::value("DEFAULT", "CloseAtEndFileSaveDialog").toBool();
    ui->cBoxCloseAtEndFileSaveDialog->setChecked(checked);
}
void UDialogSettings::setResult(int r)
{
    d_result = r;
}
int UDialogSettings::result()
{
    return d_result;
}
