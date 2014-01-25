#include "UAboutProgram.h"
#include "ui_UAboutProgram.h"

UAboutProgram::UAboutProgram(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::UAboutProgram)
{
    setWindowFlags(Qt::WindowCloseButtonHint);

    ui->setupUi(this);

    connect(ui->pButtonClose, SIGNAL(clicked()),
            this, SLOT(close()));

    QString name = "<html><head/><body><p><span style=\"font-size:14pt; font-weight:600;\">%1</span></p></body></html>";
    ui->labelName->setText(name.arg(qApp->applicationName()));
    ui->labelVersion->setText(qApp->applicationVersion());
}

UAboutProgram::~UAboutProgram()
{
    delete ui;
}
