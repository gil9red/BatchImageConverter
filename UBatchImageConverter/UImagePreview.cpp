#include "UImagePreview.h"

/// PUBLIC
UImagePreview::UImagePreview(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::UImagePreview)
{
    ui->setupUi(this);

    ui->frameView->setLayout(new QVBoxLayout());
    simpleImageViewer = new USimpleImageViewer();
    ui->frameView->layout()->addWidget(simpleImageViewer);

    ui->labelColorDepth->hide();
    ui->labelTextColorDepth->hide();
}

UImagePreview::~UImagePreview()
{
    delete ui;
}

void UImagePreview::setImage(QString path)
{
    clear();

    QProgressIndicator *indicator = new QProgressIndicator();
    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(indicator, 0, Qt::AlignCenter);
    indicator->startAnimation();

    simpleImageViewer->setLayout(layout);

    qApp->processEvents();

    QImageReader reader(path);
    if(!reader.canRead())
    {
        delete indicator;
        delete layout;

        return;
    }

    qApp->processEvents();

    QImage image = reader.read();
    if(image.isNull())
    {
        delete indicator;
        delete layout;

        return;
    }

    qApp->processEvents();

    QFileInfo fileInfo(path);

    ui->labelFormat->setText(fileInfo.suffix().toUpper());
    ui->labelSizeFile->setText(UBytes::bytes(fileInfo.size()).last());
    ui->labelSizeImage->setText(QString("%1x%2 пикселей").arg(image.width()).arg(image.height()));
    ui->labelResolution->setText(QString("%1x%2 пикселей/дюйм")
                                 .arg(image.physicalDpiX())
                                 .arg(image.physicalDpiY()));
    ui->labelColorDepth->setText(QString("%1").arg(image.bitPlaneCount()));

    qApp->processEvents();

    originalPixmap = QPixmap::fromImage(image);

    delete indicator;
    delete layout;

    qApp->processEvents();

    simpleImageViewer->setPixmap(originalPixmap);
}
void UImagePreview::clear()
{
    originalPixmap = QPixmap();
    simpleImageViewer->setPixmap(originalPixmap);

    ui->labelFormat->setText("-");
    ui->labelSizeFile->setText("-");
    ui->labelSizeImage->setText("-");
    ui->labelResolution->setText("-");
    ui->labelColorDepth->setText("-");
}
