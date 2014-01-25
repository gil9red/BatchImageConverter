#include "USlideshowWidget.h"

/// PUBLIC
USlideshowWidget::USlideshowWidget(QWidget *parent) :
    QDialog(parent)
{
    sourceModel = 0;

    createControlPanel();

    simpleImageViewer = new USimpleImageViewer();
    simpleImageViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    Qt::Alignment alignment = Qt::AlignTop | Qt::AlignLeft;

    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    vBoxLayout->setMargin(5);
    vBoxLayout->setSpacing(0);

    vBoxLayout->addWidget(panel, 0, alignment);
    vBoxLayout->addWidget(simpleImageViewer);

    setLayout(vBoxLayout);

    timerSlideshow = new QTimer();
    timerSlideshow->setInterval(1000); // TODO CHANGE INTERVAL SLIDESHOW

    connect(timerSlideshow, SIGNAL(timeout()),
            switchPhotos, SLOT(clickedNext())); // TODO reverse SLIDESHOW
}
void USlideshowWidget::setImage(QString path)
{
    QPixmap pixmapSlide(path);
    simpleImageViewer->setPixmap(pixmapSlide);
}
void USlideshowWidget::setSourceModel(QStandardItemModel *model)
{
    sourceModel = model;
    switchPhotos->setRange(1, sourceModel->rowCount());
}
void USlideshowWidget::setCurrentIndex(int index)
{
    switchPhotos->setValue(index + 1);
}

/// PRIVATE
void USlideshowWidget::createControlPanel()
{
    tButtonStart = new QToolButton();
    tButtonStart->setCheckable(true);
    tButtonStart->setChecked(true);
    tButtonStart->setToolTip("Продолжить показ слайдов");
    tButtonStart->setAutoRaise(true);
    tButtonStart->setIcon(QIcon(":/IconStart"));
    connect(tButtonStart, SIGNAL(clicked()), SLOT(start()));

    tButtonPause = new QToolButton();
    tButtonPause->setCheckable(true);
    tButtonPause->setToolTip("Приостановить показ слайдов");
    tButtonPause->setAutoRaise(true);
    tButtonPause->setIcon(QIcon(":/IconPause"));
    connect(tButtonPause, SIGNAL(clicked()), SLOT(pause()));

    QButtonGroup *bGroup = new QButtonGroup(this);
    bGroup->addButton(tButtonStart);
    bGroup->addButton(tButtonPause);

    switchPhotos = new USwitchPhotos();
    connect(switchPhotos, SIGNAL(previous()), SLOT(previous()));
    connect(switchPhotos, SIGNAL(next()), SLOT(next()));

    tButtonQuit = new QToolButton();
    tButtonQuit->setToolTip("Закрыть показ слайдов");
    tButtonQuit->setAutoRaise(true);
    tButtonQuit->setIcon(QIcon(":/IconQuit"));
    connect(tButtonQuit, SIGNAL(clicked()), SLOT(close()));

    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->setMargin(0);
    hBoxLayout->setSpacing(0);

    hBoxLayout->addWidget(tButtonStart);
    hBoxLayout->addWidget(tButtonPause);
    hBoxLayout->addSpacing(10);
    hBoxLayout->addWidget(switchPhotos);
    hBoxLayout->addSpacing(10);
    hBoxLayout->addWidget(tButtonQuit);

    panel = new QFrame();
    panel->setFrameShape(QFrame::Panel);
    panel->setStyleSheet("QFrame {background: white}");

    panel->setLayout(hBoxLayout);
}

/// PUBLIC SLOTS
void USlideshowWidget::start()
{
    if(!sourceModel)
        return;

    timerSlideshow->start();
}
void USlideshowWidget::pause()
{
    if(!sourceModel)
        return;

    timerSlideshow->stop();
}
void USlideshowWidget::next()
{
    if(!sourceModel)
        return;

    int row = switchPhotos->value() - 1;
    QString path = getItemImage(sourceModel, row, UColumnTable::Name).path;
    setImage(path);
}
void USlideshowWidget::previous()
{
    if(!sourceModel)
        return;

    int row = switchPhotos->value() - 1;
    QString path = getItemImage(sourceModel, row, UColumnTable::Name).path;
    setImage(path);
}

/// PROTECTED
void USlideshowWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
        close();
}
void USlideshowWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(rect());
}
