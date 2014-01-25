#ifndef USLIDESHOWWIDGET_H
#define USLIDESHOWWIDGET_H

#include <QtGui>
#include <QtCore>

#include "USupport.h"
#include "USwitchPhotos.h"
#include "UImagePreview.h"

class USlideshowWidget: public QDialog
{
    Q_OBJECT

public:
    explicit USlideshowWidget(QWidget *parent = 0);    
    void setImage(QString path);
    void setSourceModel(QStandardItemModel *model);
    void setCurrentIndex(int index);

private:
    void createControlPanel();

private:
    QFrame *panel;
    QToolButton *tButtonStart;
    QToolButton *tButtonPause;
    USwitchPhotos *switchPhotos;
    QToolButton *tButtonQuit;    

    USimpleImageViewer *simpleImageViewer;

    QTimer *timerSlideshow;

    QStandardItemModel *sourceModel;

public slots:
    void start();
    void pause();
    void next();
    void previous();

protected:
    void keyReleaseEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *);
};

#endif // USLIDESHOWWIDGET_H
