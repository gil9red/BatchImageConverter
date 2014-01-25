#include "UBatchImageConverter.h"

/// PUBLIC
UBatchImageConverter::UBatchImageConverter(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::UBatchImageConverter)
{    
    createGUI();
}

UBatchImageConverter::~UBatchImageConverter()
{ delete ui; }

void UBatchImageConverter::moveOnCenter()
{
    QSize sizeDesktop = QDesktopWidget().availableGeometry().size();

    move((sizeDesktop.width() - size().width()) / 2,
         (sizeDesktop.height() - size().height()) / 2);
}

/// PRIVATE
void UBatchImageConverter::createGUI()
{
    ui->setupUi(this);    

    imagePreview = new UImagePreview();

    Qt::DockWidgetAreas areas = Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea;
    QDockWidget::DockWidgetFeatures features = QDockWidget::AllDockWidgetFeatures;

    dockWidgetPreview = new QDockWidget("Предпросмотр");
    dockWidgetPreview->setWidget(imagePreview);
    dockWidgetPreview->setFeatures(features);
    dockWidgetPreview->setAllowedAreas(areas);
    addDockWidget(Qt::RightDockWidgetArea, dockWidgetPreview);


    dockWidgetSettingsBatch = new QDockWidget("Настройки");
    dockWidgetSettingsBatch->setWidget(&imageSettingsBatch);
    dockWidgetSettingsBatch->setFeatures(features);
    dockWidgetSettingsBatch->setAllowedAreas(areas);
    addDockWidget(Qt::RightDockWidgetArea, dockWidgetSettingsBatch);
    

    refreshHorizontalHeaderLabels();

    model.setSortRole(Qt::UserRole + 1);

    ui->tableView->setSortingEnabled(true);
    ui->tableView->setModel(&model);
    ui->tableView->setCurrentIndex(ui->tableView->currentIndex());    

    glass = new SBGlass(ui->tableView); // TODO ADD IMAGE DROP
    glass->enableColor(QColor(Qt::gray));
    glass->enableOpacity(0.9);
    glass->enableInfoTextBlock("Просто перетащите на окно файлы и папки",
                               QFont("Times", 14, QFont::Bold));   

    updateEnabled();

    installConnect();

    Qt::ToolBarAreas tBarAreas = Qt::TopToolBarArea
            | Qt::LeftToolBarArea
            | Qt::RightToolBarArea;
    QSize sizeToolIcons(18, 18);

    QToolBar *tBarCommand1 = new QToolBar();
    tBarCommand1->setIconSize(sizeToolIcons);
    tBarCommand1->setMovable(false);
    tBarCommand1->setAllowedAreas(tBarAreas);
    tBarCommand1->addWidget(ui->tButtonStart);
    tBarCommand1->addWidget(ui->tButtonSettings);

    QToolBar *tBarCommand2 = new QToolBar();
    tBarCommand2->setIconSize(sizeToolIcons);
    tBarCommand2->setMovable(false);
    tBarCommand2->setAllowedAreas(tBarAreas);
    tBarCommand2->addWidget(ui->tButtonAddImage);
    tBarCommand2->addWidget(ui->tButtonAddFolder);

    QToolBar *tBarCommand3 = new QToolBar();
    tBarCommand3->setIconSize(sizeToolIcons);
    tBarCommand3->setMovable(false);
    tBarCommand3->setAllowedAreas(tBarAreas);
    tBarCommand3->addWidget(ui->tButtonRemove);
    tBarCommand3->addWidget(ui->tButtonRemoveAll);

    QToolBar *tBarCommand4 = new QToolBar();
    tBarCommand4->setIconSize(sizeToolIcons);
    tBarCommand4->setMovable(false);
    tBarCommand4->setAllowedAreas(tBarAreas);
    tBarCommand4->addWidget(ui->tButtonShowImageInFolder);
    tBarCommand4->addWidget(ui->tButtonShowInViewer);
    tBarCommand4->addWidget(ui->tButtonSlideshow);

    addToolBar(Qt::TopToolBarArea, tBarCommand1);
    addToolBar(Qt::TopToolBarArea, tBarCommand2);
    addToolBar(Qt::TopToolBarArea, tBarCommand3);
    addToolBar(Qt::TopToolBarArea, tBarCommand4);
}
void UBatchImageConverter::refreshHorizontalHeaderLabels()
{
    QStringList labels;

    for(int i = 0; i < UColumnTable::Count; i++)
        labels.append("");

    labels[UColumnTable::Name] = "Имя";
    labels[UColumnTable::Format] = "Формат";
    labels[UColumnTable::SizeFile] = "Размер файла";
    labels[UColumnTable::SizeImage] = "Размер";
    labels[UColumnTable::Path] = "Путь";

    model.setHorizontalHeaderLabels(labels);
}

void UBatchImageConverter::installGlass()
{ glass->install(ui->tableView); }
void UBatchImageConverter::removeGlass()
{ glass->remove(); }

void UBatchImageConverter::installConnect()
{
    connect(ui->actionShowWindowPreview, SIGNAL(triggered(bool)),
            dockWidgetPreview, SLOT(setVisible(bool)));
    connect(ui->actionShowWindowSettings, SIGNAL(triggered(bool)),
            dockWidgetSettingsBatch, SLOT(setVisible(bool)));


    connect(dockWidgetPreview, SIGNAL(visibilityChanged(bool)),
            ui->actionShowWindowPreview, SLOT(setChecked(bool)));
    connect(dockWidgetSettingsBatch, SIGNAL(visibilityChanged(bool)),
            ui->actionShowWindowSettings, SLOT(setChecked(bool)));


    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(indexDoubleClicked(QModelIndex)));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)),
            SLOT(indexClicked(QModelIndex)));
    connect(ui->tableView, SIGNAL(activated(QModelIndex)),
            SLOT(indexClicked(QModelIndex)));
    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(selectionChange(QItemSelection,QItemSelection)));


    connect(ui->tButtonAddFolder, SIGNAL(clicked()), SLOT(addFolder()));
    connect(ui->tButtonAddImage, SIGNAL(clicked()), SLOT(addImage()));
    connect(ui->tButtonRemove, SIGNAL(clicked()), SLOT(remove()));
    connect(ui->tButtonRemoveAll, SIGNAL(clicked()), SLOT(removeAll()));
    connect(ui->tButtonStart, SIGNAL(clicked()), SLOT(doBatchConvert()));
    connect(ui->tButtonShowImageInFolder, SIGNAL(clicked()), SLOT(showImageInFolder()));
    connect(ui->tButtonShowInViewer, SIGNAL(clicked()), SLOT(showCurrentImageInTheViewer()));
    connect(ui->tButtonSettings, SIGNAL(clicked()), SLOT(settings()));
    connect(ui->tButtonSlideshow, SIGNAL(clicked()), SLOT(slideshow()));

    connect(ui->actionStart, SIGNAL(triggered()), SLOT(doBatchConvert()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    connect(ui->actionAddFolder, SIGNAL(triggered()), SLOT(addFolder()));
    connect(ui->actionAddImage, SIGNAL(triggered()), SLOT(addImage()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionRemove, SIGNAL(triggered()), SLOT(remove()));
    connect(ui->actionRemoveAll, SIGNAL(triggered()), SLOT(removeAll()));
    connect(ui->actionShowImageInFolder, SIGNAL(triggered()), SLOT(showImageInFolder()));
    connect(ui->actionShowInViewer, SIGNAL(triggered()), SLOT(showCurrentImageInTheViewer()));
    connect(ui->actionSettings, SIGNAL(triggered()), SLOT(settings()));
    connect(ui->actionRemoveDuplicateItems, SIGNAL(triggered()), SLOT(removeDuplicateItems()));
    connect(ui->actionSlideshow, SIGNAL(triggered()), SLOT(slideshow()));

    connect(&imageSettingsBatch, SIGNAL(existedSavePath()), SLOT(updateEnabled()));
}

/// PUBLIC SLOTS
void UBatchImageConverter::addFolder()
{
    QString dir = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
    QString pathDir = QFileDialog::getExistingDirectory(this, QString(), dir);

    if(pathDir.isEmpty()
            || pathDir.isNull())
        return;

    addFolders(QStringList() << pathDir);
}
void UBatchImageConverter::addFolders(QStringList paths)
{
    if(paths.isEmpty())
        return;

    QStringList filters;
    foreach (QByteArray byteArray, QImageReader::supportedImageFormats())
    {
        QString filter = byteArray;
        filters << QString("*.%1").arg(filter);
    }

    QDir::Filters filtersFile = QDir::AllEntries | QDir::NoDotAndDotDot;
    QDirIterator::IteratorFlags iteratorFlags = QDirIterator::NoIteratorFlags;

    bool def = UConfig::value("DEFAULT", "IncludeSubFolders").toBool();
    bool includeSubFolders = UConfig::value("CURRENT", "IncludeSubFolders", def).toBool();
    if(includeSubFolders)
        iteratorFlags = QDirIterator::Subdirectories;

    if(UseMultithreading)
    {
        USeekerFilesAndFolders *seeker = new USeekerFilesAndFolders();
        seeker->paths = paths;
        seeker->filters = filters;
        seeker->filtersFile = filtersFile;
        seeker->iteratorFlags = iteratorFlags;

        UProgressFileDialog *dialog = new UProgressFileDialog(this);
        dialog->setWindowTitle("Поиск файлов изображений в папках");
        dialog->setVisibleLabelDescription(true);
        dialog->setTextLabelDescription("Найденные файлы изображений:");
        dialog->setVisibleLabelMask(true);
        dialog->setTextLabelMask("Найдено %v файлов");
        dialog->setVisibleButtonOpenFolderPathWhenComplete(false);
        dialog->setVisibleProgress(false);
        dialog->showMessageWithAnEmptyListAnComplete("Ничего не найдено...");

        connect(seeker, SIGNAL(foundFiles(int)), dialog, SLOT(setValue(int)));
        connect(seeker, SIGNAL(fileWasFound(QString)), dialog, SLOT(addLog(QString)));
        connect(seeker, SIGNAL(fileWasFound(QString)), this, SLOT(addImageToModel(QString)));
        connect(seeker, SIGNAL(finished()), seeker, SLOT(deleteLater()));
        connect(seeker, SIGNAL(finished()), dialog, SLOT(complete()));
        connect(dialog, SIGNAL(canceled()), seeker, SLOT(cancel()));
        connect(seeker, SIGNAL(started()), dialog, SLOT(exec()));

        connect(seeker, SIGNAL(started()), this, SLOT(disableUpdateTable()));
        connect(seeker, SIGNAL(finished()), this, SLOT(enableUpdatesTable()));

        bool def = UConfig::value("DEFAULT", "CloseAtEndFileSearchDialog").toBool();
        bool closed = UConfig::value("CURRENT", "CloseAtEndFileSearchDialog", def).toBool();
        if(closed)
            connect(seeker, SIGNAL(finished()), dialog, SLOT(deleteLater()));

        seeker->start();
    }else
    {
        foreach(QString path, paths)
        {
            QDirIterator iterator(path, filters, filtersFile, iteratorFlags);

            while(iterator.hasNext())
                addImageToModel(iterator.next());
        }
    }

    updateEnabled();
}
void UBatchImageConverter::addImage()
{
    QString filters = "Все поддерживаемые форматы (";
    foreach (QByteArray byteArray, QImageReader::supportedImageFormats())
    {
        QString filter = byteArray;
        filters += QString("*.%1 ").arg(filter);
    }
    filters += ")\n";

    foreach (QByteArray byteArray, QImageReader::supportedImageFormats())
    {
        QString filter = byteArray;
        filters += QString("%1 (*.%2)\n").arg(filter.toUpper()).arg(filter);
    }

    QString dir = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
    QStringList listFiles = QFileDialog::getOpenFileNames(this, QString(), dir, filters);

    if(listFiles.isEmpty())
        return;

    foreach (QString path, listFiles)
        addImageToModel(path);
}
void UBatchImageConverter::remove()
{
    while(ui->tableView->selectionModel()->hasSelection())
        model.removeRow(ui->tableView->selectionModel()->selectedRows().takeFirst().row());

    imagePreview->clear();
    updateEnabled();
}
void UBatchImageConverter::removeAll()
{
    model.clear();
    refreshHorizontalHeaderLabels();

    imagePreview->clear();

    updateEnabled();
}
void UBatchImageConverter::doBatchConvert()
{
    // перед конвертацией обязательно удалим повторы
    removeDuplicateItems();

    QString pathSave = imageSettingsBatch.ui->lEditPathSave->text();
    QDir().mkdir(pathSave);

    if(UseMultithreading)
    {
        QList <UItemImage> list;

        for(int i = 0; i < model.rowCount(); i++)
            list << getItemImage(&model, i, UColumnTable::Name);

        USaveImage *saveThread = new USaveImage();
        saveThread->listImages = list;
        saveThread->pathSave = pathSave;
        saveThread->format = imageSettingsBatch.ui->comboBoxWriteFormats->currentText();

        connect(saveThread, SIGNAL(finished()), saveThread, SLOT(deleteLater()));

        UProgressFileDialog *dialog = new UProgressFileDialog(this);
        dialog->setWindowTitle("Сохранение файлов изображений");
        dialog->setVisibleLabelDescription(true);
        dialog->setTextLabelDescription("Сохраненные файлы изображений:");
        dialog->setLabelText("Сохраненные файлы:");
        dialog->setOpenFolderPath(pathSave);
        dialog->setVisibleLabelMask(true);
        dialog->setTextLabelMask("Сохранено %v/%m файлов");

        connect(saveThread, SIGNAL(range(int,int)), dialog, SLOT(setRange(int,int)));
        connect(saveThread, SIGNAL(value(int)), dialog, SLOT(setValue(int)));
        connect(saveThread, SIGNAL(currentImage(QString)), dialog, SLOT(addLog(QString)));
        connect(dialog, SIGNAL(canceled()), saveThread, SLOT(cancel()));
        connect(saveThread, SIGNAL(started()), dialog, SLOT(exec()));
        connect(saveThread, SIGNAL(finished()), dialog, SLOT(complete()));

        bool def = UConfig::value("DEFAULT", "CloseAtEndFileSaveDialog").toBool();
        bool closed = UConfig::value("CURRENT", "CloseAtEndFileSaveDialog", def).toBool();
        if(closed)
            connect(saveThread, SIGNAL(finished()), dialog, SLOT(deleteLater()));

        saveThread->start();
    }else
    {
        for(int row = 0; row < model.rowCount(); row++)
        {
            UItemImage image = getItemImage(&model, row, UColumnTable::Name);
            QString name = image.name;
            QString path = image.path;
            QString format = imageSettingsBatch.ui->comboBoxWriteFormats->currentText();
            QImage(path).save(QString("%1/%2.%3").arg(pathSave).arg(name).arg(format));
        }
    }
}
void UBatchImageConverter::addImageToModel(QString path)
{
    if(!imageFileIsValid(path))
        return;

    QList <QStandardItem *> listItems = infoImage(path);

    if(!listItems.isEmpty())
        model.appendRow(listItems);

    updateEnabled();
}
void UBatchImageConverter::showImageInFolder()
{
    QString path = getItemImage(&model, ui->tableView->currentIndex().row(), UColumnTable::Name).path;

    QProcess::startDetached(QString("explorer.exe /select,%1")
                            .arg(QDir::toNativeSeparators(path)));
}
void UBatchImageConverter::showImageInTheViewer(QString path)
{
    UImageViewer *viewer = new UImageViewer();
    viewer->setImage(path);
    viewer->setSourceModel(&model);
    viewer->setCurrentIndex(ui->tableView->currentIndex().row());    

    connect(this, SIGNAL(destroyed()), viewer, SLOT(close()));

    // нужно чтобы просмотрщик был только один, и пока он открыт,
    // нельзя было изменить содержимое таблицы фотографий
    // а т.к. просмотрщик не может быть создан как диалог,
    // придется его поместить в виджет-диалог
    QDialog *dialog = new QDialog();
    dialog->setWindowFlags(viewer->windowFlags());
    dialog->setWindowTitle(viewer->windowTitle());

    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    vBoxLayout->setMargin(0);
    vBoxLayout->addWidget(viewer);

    dialog->setLayout(vBoxLayout);
    dialog->resize(500, 400);

    connect(viewer, SIGNAL(destroyed()), dialog, SLOT(close()));

    dialog->exec();
    dialog->deleteLater();
}
void UBatchImageConverter::showCurrentImageInTheViewer()
{
    QString path = getItemImage(&model, ui->tableView->currentIndex().row(), UColumnTable::Name).path;
    showImageInTheViewer(path);
}
void UBatchImageConverter::disableUpdateTable()
{ ui->tableView->setUpdatesEnabled(false); }
void UBatchImageConverter::enableUpdatesTable()
{ ui->tableView->setUpdatesEnabled(true); }

void UBatchImageConverter::about()
{
    UAboutProgram *about = new UAboutProgram();
    about->exec();
    about->deleteLater();
}
void UBatchImageConverter::settings()
{
    UDialogSettings *settings = new UDialogSettings(this);
    settings->setWindowTitle(QString("%1 - %2")
                             .arg(appAbbreviationName)
                             .arg("Настройки"));
    settings->exec();
    settings->deleteLater();
}
void UBatchImageConverter::removeDuplicateItems()
{
    // запомним
    Qt::SortOrder sort = ui->tableView->horizontalHeader()->sortIndicatorOrder();
    int sortColumn = ui->tableView->horizontalHeader()->sortIndicatorSection();

    ui->tableView->sortByColumn(UColumnTable::Path, sort);

    QList <int> deletedRows;

    for(int row = 0; row < model.rowCount() - 1; row++)
        if(model.item(row, UColumnTable::Path)->text() == model.item(row + 1, UColumnTable::Path)->text())
            deletedRows << row;

    while(!deletedRows.isEmpty())
    {
        model.removeRow(deletedRows.takeFirst());

        for(int i = 0; i < deletedRows.size(); i++)
            deletedRows[i] -= 1;
    }

    // восстановим "как было"
    ui->tableView->horizontalHeader()->setSortIndicator(sortColumn, sort);
}
void UBatchImageConverter::slideshow()
{
    USlideshowWidget *slideshowWidget = new USlideshowWidget(this);
    slideshowWidget->setWindowTitle("Слайд показ");

    QString path = getItemImage(&model, 0, UColumnTable::Name).path;
    slideshowWidget->setImage(path);
    slideshowWidget->setSourceModel(&model);
    slideshowWidget->setCurrentIndex(0);

    slideshowWidget->start();

    slideshowWidget->showFullScreen();
    slideshowWidget->exec();
    slideshowWidget->deleteLater();
}

/// PRIVATE SLOTS
void UBatchImageConverter::selectionChange(QItemSelection item1, QItemSelection item2)
{
    Q_UNUSED(item1)
    Q_UNUSED(item2)

    updateEnabled();
}
void UBatchImageConverter::updateEnabled()
{
    bool hasSelection = ui->tableView->selectionModel()->hasSelection();
    bool itemIsSelected = ui->tableView->currentIndex().row() != -1;
    bool modelNotEmpty = model.rowCount() != 0;

    if(!modelNotEmpty)
        installGlass();
    else
        removeGlass();

    bool start = modelNotEmpty & imageSettingsBatch.existSavePath;

    ui->tButtonRemove->setEnabled(hasSelection);
    ui->tButtonRemoveAll->setEnabled(modelNotEmpty);
    ui->tButtonStart->setEnabled(start);
    ui->tButtonShowImageInFolder->setEnabled(itemIsSelected);
    ui->tButtonShowInViewer->setEnabled(itemIsSelected);
    ui->tButtonSlideshow->setEnabled(modelNotEmpty);

    ui->actionRemove->setEnabled(hasSelection);
    ui->actionRemoveAll->setEnabled(modelNotEmpty);
    ui->actionStart->setEnabled(start);
    ui->actionShowImageInFolder->setEnabled(itemIsSelected);
    ui->actionShowInViewer->setEnabled(itemIsSelected);
    ui->actionRemoveDuplicateItems->setEnabled(modelNotEmpty);
    ui->actionSlideshow->setEnabled(modelNotEmpty);
}
void UBatchImageConverter::indexDoubleClicked(QModelIndex index)
{
    QString path = getItemImage(&model, index.row(), UColumnTable::Name).path;
    showImageInTheViewer(path);
}
void UBatchImageConverter::indexClicked(QModelIndex index)
{
    updateEnabled();

    QString path = getItemImage(&model, index.row(), UColumnTable::Name).path;

    imagePreview->setImage(path);
}

/// PROTECTED
void UBatchImageConverter::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        QList <QUrl> urls = event->mimeData()->urls();

        foreach(QUrl url, urls)
        {
            QFileInfo info(url.toLocalFile());

            if(info.isFile())
            {
                if(QImageReader::supportedImageFormats().contains(info.suffix().toUtf8()))
                {
                    event->acceptProposedAction();
                    return;
                }

            }else if(info.isDir())
            {
                // пустую папку нам не нужно
                if(QDir(info.filePath()).entryList(QDir::AllEntries
                                                   | QDir::NoDotAndDotDot).isEmpty())
                    return;

                event->acceptProposedAction();
                return;
            }
        }
    }
}
void UBatchImageConverter::dropEvent(QDropEvent *event)
{
    QList <QUrl> urls = event->mimeData()->urls();

    if(urls.isEmpty())
        return;

    QStringList pathDirs;

    foreach(QUrl url, urls)
    {
        QFileInfo info(url.toLocalFile());
        QString path = info.absoluteFilePath();

        if(info.isFile())
        {
            if(QImageReader::supportedImageFormats().contains(info.suffix().toUtf8()))
                addImageToModel(path);

        }else if(info.isDir())
            pathDirs << info.filePath();
    }

    addFolders(pathDirs);
}
