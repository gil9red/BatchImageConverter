#ifndef UBATCHIMAGECONVERTER_H
#define UBATCHIMAGECONVERTER_H

#include <QtGui>
#include <QtCore>

#include "ui_UBatchImageConverter.h"

#include "UImageSettingsBatch.h"
#include "USupport.h"
#include "UImageViewer.h"
#include "USeekerFilesAndFolders.h"
#include "UProgressFileDialog.h"
#include "UItemImage.h"
#include "UAboutProgram.h"
#include "UDialogSettings.h"
#include "USlideshowWidget.h"

#include "sbglass.h"

#define UseMultithreading 1

namespace Ui {
class UBatchImageConverter;
}

class UBatchImageConverter: public QMainWindow
{
    Q_OBJECT
    
public:
    explicit UBatchImageConverter(QWidget *parent = 0);
    ~UBatchImageConverter();
    void moveOnCenter();

private:
    void createGUI();
    void refreshHorizontalHeaderLabels();    

    void installGlass();
    void removeGlass();

    void installConnect();

public slots:
    void addFolder();    
    void addFolders(QStringList paths);
    void addImage();
    void remove();
    void removeAll();    
    void doBatchConvert();
    void addImageToModel(QString path);
    void showImageInFolder();
    void showImageInTheViewer(QString path);
    void showCurrentImageInTheViewer();
    void disableUpdateTable();
    void enableUpdatesTable();
    void about();
    void settings();
    void removeDuplicateItems();
    void slideshow();

private slots:
    void selectionChange(QItemSelection item1, QItemSelection item2);
    void updateEnabled();
    void indexDoubleClicked(QModelIndex index);
    void indexClicked(QModelIndex index);

private:
    Ui::UBatchImageConverter *ui;
    UImageSettingsBatch imageSettingsBatch;
    UImagePreview *imagePreview;

    QStandardItemModel model;

    SBGlass *glass;

    QDockWidget *dockWidgetPreview;
    QDockWidget *dockWidgetSettingsBatch;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);    
};

#endif // UBATCHIMAGECONVERTER_H
