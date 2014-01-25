#include "UImageViewer.h"

/// PUBLIC
UImageViewer::UImageViewer(QString path, QWidget *parent)
    : QMainWindow(parent)
{
    createGUI();
    setImage(path);

    resize(500, 400);
}
UImageViewer::UImageViewer(QWidget *parent)
    : QMainWindow(parent)
{
    createGUI();

    resize(500, 400);
}
void UImageViewer::setImage(QString path)
{
    QString name = "Окно просмотра";

    QString fileName = path;
    if (!fileName.isEmpty())
    {
        QImage image(fileName);

        if(image.isNull())
        {
            QMessageBox::information(this, name,
                                     QString("Невозможно загрузить %1.")
                                     .arg(fileName));
            return;
        }

        pixmap = QPixmap::fromImage(image);
        labelImage->setPixmap(pixmap);
        updateChessGrid(tButtonEnabledChessGrid->isChecked());

        scaleFactor = 1.0;

        actionFitToWindow->setEnabled(true);
        updateActions();

        if (!actionFitToWindow->isChecked())
            labelImage->adjustSize();
    }

    setWindowTitle(QString("%1 - %2").arg(name).arg(path));
    if(parentWidget())
        parentWidget()->setWindowTitle(windowTitle());
}
void UImageViewer::setSourceModel(QStandardItemModel *model)
{
    sourceModel = model;
    switchPhotos->setRange(1, sourceModel->rowCount());
}
void UImageViewer::setCurrentIndex(int index)
{
    switchPhotos->setValue(index + 1);
}

/// PRIVATE
void UImageViewer::createGUI()
{
    sourceModel = 0;

    setContextMenuPolicy(Qt::NoContextMenu);

    labelImage = new QLabel;
    labelImage->setBackgroundRole(QPalette::Base);
    labelImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    labelImage->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(labelImage);
    scrollArea->setAlignment(Qt::AlignCenter);
    setCentralWidget(scrollArea);

    scrollArea->installEventFilter(this);

    createToolbarCommand();
    createActions();
    createMenus();
}
void UImageViewer::createToolbarCommand()
{
    tButtonRotatedClockwise = new QToolButton();
    tButtonRotatedClockwise->setIcon(QIcon(":/IconClockwise"));
    tButtonRotatedClockwise->setToolTip("Повернуть по часовой стрелке");
    connect(tButtonRotatedClockwise, SIGNAL(clicked()), SLOT(rotatedClockwise()));

    tButtonTurnCounterClockwise = new QToolButton();
    tButtonTurnCounterClockwise->setIcon(QIcon(":/IconTurnCounter"));
    tButtonTurnCounterClockwise->setToolTip("Повернуть против часовой стрелки");
    connect(tButtonTurnCounterClockwise, SIGNAL(clicked()), SLOT(turnCounterClockwise()));

    tButtonZoomIn = new QToolButton();
    tButtonZoomIn->setIcon(QIcon(":/IconZoomIn"));
    tButtonZoomIn->setToolTip("Увеличить");
    connect(tButtonZoomIn, SIGNAL(clicked()), SLOT(zoomIn()));

    tButtonZoomOut = new QToolButton();
    tButtonZoomOut->setIcon(QIcon(":/IconZoomOut"));
    tButtonZoomOut->setToolTip("Уменьшить");
    connect(tButtonZoomOut, SIGNAL(clicked()), SLOT(zoomOut()));

    tButtonNormalSize = new QToolButton();
    tButtonNormalSize->setIcon(QIcon(":/IconNormalSize"));
    tButtonNormalSize->setToolTip("Обычный размер");
    connect(tButtonNormalSize, SIGNAL(clicked()), SLOT(normalSize()));

    tButtonFitToWindow = new QToolButton();
    tButtonFitToWindow->setCheckable(true);
    tButtonFitToWindow->setIcon(QIcon(":/IconFitToWindow"));
    tButtonFitToWindow->setToolTip("По размеру окна");
    connect(tButtonFitToWindow, SIGNAL(clicked(bool)), SLOT(fitToWindow(bool)));

    tButtonEnabledChessGrid = new QToolButton();
    tButtonEnabledChessGrid->setCheckable(true);
    tButtonEnabledChessGrid->setChecked(false);
    tButtonEnabledChessGrid->setIcon(QIcon(":/IconChessGrid"));
    tButtonEnabledChessGrid->setToolTip("Показывать шахматную сетку");
    connect(tButtonEnabledChessGrid, SIGNAL(clicked(bool)), SLOT(updateChessGrid(bool)));

    switchPhotos = new USwitchPhotos();
    connect(switchPhotos, SIGNAL(next()), SLOT(next()));
    connect(switchPhotos, SIGNAL(previous()), SLOT(previous()));

    QToolBar *tBarCommand1 = new QToolBar();
    tBarCommand1->setMovable(false);
    tBarCommand1->addWidget(tButtonZoomIn);
    tBarCommand1->addWidget(tButtonZoomOut);

    QToolBar *tBarCommand2 = new QToolBar();
    tBarCommand2->setMovable(false);
    tBarCommand2->addWidget(tButtonNormalSize);
    tBarCommand2->addWidget(tButtonFitToWindow);

    QToolBar *tBarCommand3 = new QToolBar();
    tBarCommand3->setMovable(false);
    tBarCommand3->addWidget(tButtonTurnCounterClockwise);
    tBarCommand3->addWidget(tButtonRotatedClockwise);

    QToolBar *tBarCommand4 = new QToolBar();
    tBarCommand4->setMovable(false);
    tBarCommand4->addWidget(tButtonEnabledChessGrid);

    QToolBar *tBarCommand5 = new QToolBar();
    tBarCommand5->setMovable(false);
    tBarCommand5->addWidget(switchPhotos);

    addToolBar(Qt::BottomToolBarArea, tBarCommand1);
    addToolBar(Qt::BottomToolBarArea, tBarCommand2);
    addToolBar(Qt::BottomToolBarArea, tBarCommand3);
    addToolBar(Qt::BottomToolBarArea, tBarCommand4);
    addToolBar(Qt::BottomToolBarArea, tBarCommand5);
}
void UImageViewer::createActions()
{
    actionQuit = new QAction("Выход", this);
    actionQuit->setIcon(QIcon(":/IconQuit"));
    actionQuit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actionQuit, SIGNAL(triggered()), SLOT(close()));

    actionZoomIn = new QAction("Увеличить (25%)", this);
    actionZoomIn->setIcon(QIcon(":/IconZoomIn"));
    actionZoomIn->setShortcut(QKeySequence("Ctrl++"));
    actionZoomIn->setEnabled(false);
    connect(actionZoomIn, SIGNAL(triggered()), SLOT(zoomIn()));

    actionZoomOut = new QAction("Уменьшить (25%)", this);
    actionZoomOut->setIcon(QIcon(":/IconZoomOut"));
    actionZoomOut->setShortcut(QKeySequence("Ctrl+-"));
    actionZoomOut->setEnabled(false);
    connect(actionZoomOut, SIGNAL(triggered()), SLOT(zoomOut()));

    actionNormalSize = new QAction("Обычный размер", this);
    actionNormalSize->setIcon(QIcon(":/IconNormalSize"));
    actionNormalSize->setShortcut(QKeySequence("Ctrl+S"));
    actionNormalSize->setEnabled(false);
    connect(actionNormalSize, SIGNAL(triggered()), SLOT(normalSize()));

    actionFitToWindow = new QAction("По размеру окна", this);
    actionFitToWindow->setIcon(QIcon(":/IconFitToWindow"));
    actionFitToWindow->setEnabled(false);
    actionFitToWindow->setCheckable(true);
    actionFitToWindow->setShortcut(QKeySequence("Ctrl+F"));
    connect(actionFitToWindow, SIGNAL(triggered(bool)), SLOT(fitToWindow(bool)));

    actionRotatedClockwise = new QAction("Повернуть по часовой стрелке", this);
    actionRotatedClockwise->setIcon(QIcon(":/IconClockwise"));
    actionRotatedClockwise->setShortcut(QKeySequence("Ctrl+K"));
    connect(actionRotatedClockwise, SIGNAL(triggered()), SLOT(rotatedClockwise()));

    actionTurnCounterClockwise = new QAction("Повернуть против часовой стрелки", this);
    actionTurnCounterClockwise->setIcon(QIcon(":/IconTurnCounter"));
    actionTurnCounterClockwise->setShortcut(QKeySequence("Ctrl+L"));
    connect(actionTurnCounterClockwise, SIGNAL(triggered()), SLOT(turnCounterClockwise()));

    actionEnabledChessGrid = new QAction("Показывать шахматную сетку", this);
    actionEnabledChessGrid->setShortcut(QKeySequence("Ctrl+G"));
    actionEnabledChessGrid->setCheckable(true);
    connect(actionEnabledChessGrid, SIGNAL(triggered(bool)), SLOT(updateChessGrid(bool)));

    actionPrevious = new QAction("Предыдущее", this);
    actionPrevious->setEnabled(switchPhotos->isEnabledPrevious());
    actionPrevious->setIcon(QIcon(":/IconPrevious"));
    actionPrevious->setShortcut(QKeySequence("Ctrl+Left"));
    connect(actionPrevious, SIGNAL(triggered()), switchPhotos, SLOT(clickedPrevious()));
    connect(switchPhotos, SIGNAL(changeEnabledPrevious(bool)), actionPrevious, SLOT(setEnabled(bool)));

    actionNext = new QAction("Следующее", this);
    actionNext->setEnabled(switchPhotos->isEnabledNext());
    actionNext->setIcon(QIcon(":/IconNext"));
    actionNext->setShortcut(QKeySequence("Ctrl+Right"));
    connect(actionNext, SIGNAL(triggered()), switchPhotos, SLOT(clickedNext()));
    connect(switchPhotos, SIGNAL(changeEnabledNext(bool)), actionNext, SLOT(setEnabled(bool)));
}

void UImageViewer::createMenus()
{
    menuFile = new QMenu("&Файл", this);
    menuFile->addAction(actionQuit);

    menuView = new QMenu("&Вид", this);
    menuView->addAction(actionZoomIn);
    menuView->addAction(actionZoomOut);
    menuView->addSeparator();
    menuView->addAction(actionNormalSize);
    menuView->addAction(actionFitToWindow);
    menuView->addSeparator();
    menuView->addAction(actionRotatedClockwise);
    menuView->addAction(actionTurnCounterClockwise);
    menuView->addSeparator();
    menuView->addAction(actionEnabledChessGrid);
    menuView->addSeparator();
    menuView->addAction(actionPrevious);
    menuView->addAction(actionNext);

    menuBar()->addMenu(menuFile);
    menuBar()->addMenu(menuView);
}

void UImageViewer::updateActions()
{
    bool enabled = !actionFitToWindow->isChecked();

    actionZoomIn->setEnabled(enabled);
    actionZoomOut->setEnabled(enabled);
    actionNormalSize->setEnabled(enabled);

    tButtonZoomIn->setEnabled(enabled);
    tButtonZoomOut->setEnabled(enabled);
    tButtonNormalSize->setEnabled(enabled);
}

void UImageViewer::scaleImage(double factor)
{
    Q_ASSERT(labelImage->pixmap());
    scaleFactor *= factor;
    labelImage->resize(scaleFactor * labelImage->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    bool enabledZoomIn = scaleFactor < 3.0;
    bool enabledZoomOut = scaleFactor > 0.333;

    actionZoomIn->setEnabled(enabledZoomIn);
    actionZoomOut->setEnabled(enabledZoomOut);

    tButtonZoomIn->setEnabled(enabledZoomIn);
    tButtonZoomOut->setEnabled(enabledZoomOut);

    updateChessGrid(tButtonEnabledChessGrid->isChecked());
}

void UImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
void UImageViewer::restoreDefault()
{
    fitToWindow(false);
    updateActions();
    updateChessGrid(false);
}

/// PRIVATE SLOTS
void UImageViewer::updateChessGrid(bool update)
{
    tButtonEnabledChessGrid->setChecked(update);
    actionEnabledChessGrid->setChecked(update);

    if(update)
    {
        const QPixmap *pixmap = labelImage->pixmap();
        if(!pixmap)
            return;

        QImage grid = paintingChessGrid(pixmap->size());
        QPainter painter(&grid);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawPixmap(0, 0, *pixmap);

        labelImage->setPixmap(QPixmap::fromImage(grid));
    }/*else
        labelImage->setPixmap(pixmap); TODO */
}

/// PUBLIC SLOTS
void UImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void UImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void UImageViewer::normalSize()
{
    labelImage->adjustSize();
    scaleFactor = 1.0;
}

void UImageViewer::fitToWindow(bool fit)
{
    actionFitToWindow->setChecked(fit);
    tButtonFitToWindow->setChecked(fit);

    bool fitToWindow = actionFitToWindow->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);

    if(!fitToWindow)
        normalSize();

    updateActions();

    updateChessGrid(tButtonEnabledChessGrid->isChecked());
}
void UImageViewer::rotatedClockwise()
{
    if(labelImage->pixmap()->isNull())
        return;

    labelImage->resize(labelImage->height(), labelImage->width());

    QTransform transform;
    transform.rotate(90);

    labelImage->setPixmap(labelImage->pixmap()->transformed(transform));

    updateChessGrid(tButtonEnabledChessGrid->isChecked());
}
void UImageViewer::turnCounterClockwise()
{
    if(labelImage->pixmap()->isNull())
        return;

    labelImage->resize(labelImage->height(), labelImage->width());

    QTransform transform;
    transform.rotate(-90);

    labelImage->setPixmap(labelImage->pixmap()->transformed(transform));

    updateChessGrid(tButtonEnabledChessGrid->isChecked());
}
void UImageViewer::next()
{
    restoreDefault();
    int row = switchPhotos->value() - 1;
    QString path = getItemImage(sourceModel, row, UColumnTable::Name).path;
    setImage(path);
}
void UImageViewer::previous()
{
    restoreDefault();
    int row = switchPhotos->value() - 1;
    QString path = getItemImage(sourceModel, row, UColumnTable::Name).path;
    setImage(path);
}

/// PROTECTED
void UImageViewer::closeEvent(QCloseEvent *)
{
    deleteLater();
}
bool UImageViewer::eventFilter(QObject *object, QEvent *event)
{
    if(object == scrollArea)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast <QMouseEvent *> (event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                lastPos = mouseEvent->pos();

                if(scrollArea->horizontalScrollBar()->isVisible()
                        || scrollArea->verticalScrollBar()->isVisible())
                    scrollArea->setCursor(Qt::ClosedHandCursor);
                else
                    scrollArea->setCursor(Qt::ArrowCursor);
            }

        }else if(event->type() == QEvent::MouseMove)
        {
            QMouseEvent *mouseEvent = static_cast <QMouseEvent *> (event);

            if(mouseEvent->buttons() == Qt::LeftButton)
            {
                lastPos -= mouseEvent->pos();

                int hValue = scrollArea->horizontalScrollBar()->value();
                int vValue = scrollArea->verticalScrollBar()->value();

                scrollArea->horizontalScrollBar()->setValue(lastPos.x() + hValue);
                scrollArea->verticalScrollBar()->setValue(lastPos.y() + vValue);

                lastPos = mouseEvent->pos();
            }

        }else if(event->type() == QEvent::MouseButtonRelease)
            scrollArea->setCursor(Qt::ArrowCursor);
    }

    return QMainWindow::eventFilter(object, event);
}
