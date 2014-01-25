#include "UProgressFileDialog.h"
#include "ui_UProgressFileDialog.h"

/// PUBLIC
UProgressFileDialog::UProgressFileDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::UProgressFileDialog),
      visibleButtonOpenFolderPathWhenComplete(true),
      corrupt(0),
      value(0),
      messageWithAnEmptyListAnComplete(""),
      visibleMessageWithAnEmptyListAnComplete(false)
{
    setWindowFlags(Qt::Dialog
                   | Qt::WindowSystemMenuHint
                   | Qt::WindowMinMaxButtonsHint);

    ui->setupUi(this);

    ui->lViewLog->setModel(&model);    

    ui->labelCorruptFile->setVisible(false);
    ui->labelMask->setVisible(false);
    ui->labelDescription->setVisible(false);

    ui->pButtonCancel->show();
    ui->pButtonClose->hide();
    ui->pButtonOpenFolder->hide();

    connect(ui->pButtonCancel, SIGNAL(clicked()), SLOT(doCancel()));
    connect(ui->pButtonClose, SIGNAL(clicked()), SLOT(close()));
    connect(ui->pButtonOpenFolder, SIGNAL(clicked()), SLOT(openFolder()));

    connect(ui->lViewLog, SIGNAL(doubleClicked(QModelIndex)), SLOT(indexDoubleClicked(QModelIndex)));
}

UProgressFileDialog::~UProgressFileDialog()
{
    delete ui;
}

void UProgressFileDialog::setOpenFolderPath(QString path)
{ d_openFolderPath = path; }
QString UProgressFileDialog::openFolderPath()
{ return d_openFolderPath; }

void UProgressFileDialog::setVisibleLabelMask(bool visible)
{ ui->labelMask->setVisible(visible); }
void UProgressFileDialog::setTextLabelMask(QString text)
{
    textMaskLabel = text;
    ui->labelMask->setText(text);
    testLabelMask();
}

void UProgressFileDialog::setVisibleButtonOpenFolderPathWhenComplete(bool visible)
{ visibleButtonOpenFolderPathWhenComplete = visible; }
void UProgressFileDialog::setVisibleProgress(bool visible)
{ ui->progressBar->setVisible(visible); }

void UProgressFileDialog::setVisibleLabelDescription(bool visible)
{ ui->labelDescription->setVisible(visible); }
void UProgressFileDialog::setTextLabelDescription(QString text)
{ ui->labelDescription->setText(text); }

void UProgressFileDialog::showMessageWithAnEmptyListAnComplete(QString text, bool visible)
{
    messageWithAnEmptyListAnComplete = text;
    visibleMessageWithAnEmptyListAnComplete = visible;
}

/// PRIVATE
void UProgressFileDialog::testLabelMask()
{
    QString text = textMaskLabel;
    text.replace("%m", QString::number(ui->progressBar->maximum()))
            .replace("%v", QString::number(value));
    ui->labelMask->setText(text);
}

/// PUBLIC SLOTS
void UProgressFileDialog::addLog(QString log)
{   
    QStandardItem *item = new QStandardItem();
    item->setText(log);
    item->setData(UProgressFileDialog::Path);

    if(!imageFileIsValid(log))
    {
        corrupt++;
        ui->labelCorruptFile->show();
        ui->labelCorruptFile->setText(QString("Повреждено %1 файлов").arg(corrupt));
        item->setForeground(Qt::red);
        item->setText("Файл поврежден: " + item->text());
        item->setData(UProgressFileDialog::Other);
    }    

    model.appendRow(item);

    emit addedLog(log);
}
void UProgressFileDialog::setRange(int min, int max)
{        
    ui->progressBar->setRange(min, max);
    testLabelMask();
}
void UProgressFileDialog::setValue(int value)
{
    this->value = value;
    ui->progressBar->setValue(value);
    testLabelMask();
}
void UProgressFileDialog::setLabelText(QString text)
{
    ui->labelDescription->setText(text);
}
void UProgressFileDialog::openFolder()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(d_openFolderPath));
}
void UProgressFileDialog::doCancel()
{
    complete();
    emit canceled();
}

/// PRIVATE SLOTS
void UProgressFileDialog::complete()
{
    ui->pButtonCancel->hide();
    ui->pButtonClose->show();
    ui->pButtonOpenFolder->setVisible(visibleButtonOpenFolderPathWhenComplete);

    if(!model.rowCount()
            && visibleMessageWithAnEmptyListAnComplete)
    {
        QStandardItem *item = new QStandardItem(messageWithAnEmptyListAnComplete);
        item->setData(UProgressFileDialog::Other);
        model.appendRow(item);
    }
}
void UProgressFileDialog::indexDoubleClicked(QModelIndex index)
{
    if(model.data(index, Qt::UserRole + 1).toInt() == UProgressFileDialog::Other)
        return;

    QString path = model.data(index).toString();

    QProcess::startDetached(QString("explorer.exe /select,%1")
                            .arg(QDir::toNativeSeparators(path)));
}
