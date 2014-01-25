#include "UImageSettingsBatch.h"

/// PUBLIC
UImageSettingsBatch::UImageSettingsBatch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UImageSettingsBatch)
{
    ui->setupUi(this);

    // TODO SETTINGS Foldout
    //    UFoldoutGroupBox *foldoutGroupBox = new UFoldoutGroupBox();
    //    foldoutGroupBox->setLayout(ui->groupBox->layout());
    //    foldoutGroupBox->setTitle(ui->groupBox->title());
    //    layout()->addWidget(foldoutGroupBox);
    //    delete ui->groupBox;

    ui->lEditPathSave->setText(QDesktopServices::storageLocation(QDesktopServices::PicturesLocation)
                               + "/"
                               + "Converted Images");

    foreach (QByteArray byteArray, QImageWriter::supportedImageFormats())
    {
        if(byteArray == "jpeg" || byteArray == "tiff")
            continue;
        QString format = byteArray;
        ui->comboBoxWriteFormats->addItem(format);
    }

    ui->comboBoxWriteFormats->setCurrentIndex(ui->comboBoxWriteFormats->findText("png"));

    connect(ui->tButtonSelectPathSave, SIGNAL(clicked()), SLOT(selectPathSave()));

    connect(ui->lEditPathSave, SIGNAL(editingFinished()), SLOT(testSavePath()));
    connect(ui->lEditPathSave, SIGNAL(textChanged(QString)), SLOT(testSavePath(QString)));

    load();
}

UImageSettingsBatch::~UImageSettingsBatch()
{
    delete ui;
}

void UImageSettingsBatch::load()
{
    QString defaultDir = UConfig::value("DEFAULT", "Save").toString();    
    ui->lEditPathSave->setText(UConfig::value("CURRENT", "Save", defaultDir).toString());

    testSavePath();
}
void UImageSettingsBatch::save()
{   
    UConfig::setValue("CURRENT", "Save", ui->lEditPathSave->text());
}

/// PUBLIC SLOTS
void UImageSettingsBatch::selectPathSave()
{
    QString dir = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
    QString pathDir = QFileDialog::getExistingDirectory(this, QString(), dir);

    if(pathDir.isEmpty()
            || pathDir.isNull())
        return;

    testSavePath();

    ui->lEditPathSave->setText(QDir::toNativeSeparators(pathDir));

    save();
}

/// PRIVATE SLOTS
void UImageSettingsBatch::testSavePath(QString text)
{
    existSavePath = QDir(text).exists();

    if(!existSavePath)
        ui->labelErrorsDir->setText(QString("<font color=red size=3>%1</font>")
                                    .arg("Папка не существует"));
    else
    {
        save();
        ui->labelErrorsDir->clear();
    }

    emit existedSavePath();
}
void UImageSettingsBatch::testSavePath()
{
    testSavePath(ui->lEditPathSave->text());
}
