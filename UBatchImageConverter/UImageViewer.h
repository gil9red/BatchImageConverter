#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QtGui>
#include <QtCore>

#include "USupport.h"
#include "USwitchPhotos.h"

class UImageViewer: public QMainWindow
{
    Q_OBJECT

public:
    UImageViewer(QString path, QWidget *parent = 0);
    UImageViewer(QWidget *parent = 0);
    void setImage(QString path);
    void setSourceModel(QStandardItemModel *model);
    void setCurrentIndex(int index);

private:
    QLabel *labelImage;
    QPixmap pixmap;

    QScrollArea *scrollArea;
    double scaleFactor;

    QToolButton *tButtonRotatedClockwise;
    QToolButton *tButtonTurnCounterClockwise;
    QToolButton *tButtonZoomIn;
    QToolButton *tButtonZoomOut;
    QToolButton *tButtonNormalSize;
    QToolButton *tButtonFitToWindow;
    QToolButton *tButtonEnabledChessGrid;    
    QToolButton *tButtonNext;
    QToolButton *tButtonPrevious;
    USwitchPhotos *switchPhotos;

    QAction *actionQuit;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionNormalSize;
    QAction *actionFitToWindow;
    QAction *actionRotatedClockwise;
    QAction *actionTurnCounterClockwise;
    QAction *actionEnabledChessGrid;    
    QAction *actionPrevious;
    QAction *actionNext;

    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuHelp;

    QPoint lastPos;

    QStandardItemModel *sourceModel;

private:
    void createGUI();
    void createToolbarCommand();
    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void restoreDefault();

private slots:
    void updateChessGrid(bool update = true);

public slots:
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow(bool fit);
    void rotatedClockwise();
    void turnCounterClockwise();
    void next();
    void previous();

protected:
    void closeEvent(QCloseEvent *);    
    bool eventFilter(QObject *object, QEvent *event);
};

#endif
