#ifndef UIMAGEPREVIEW_H
#define UIMAGEPREVIEW_H

#include <QtGui>
#include <QtCore>

#include "USupport.h"
#include "QProgressIndicator/QProgressIndicator.h"

#include "ui_UImagePreview.h"

namespace Ui {
class UImagePreview;
}

class USimpleImageViewer: public QWidget
{
public:
    USimpleImageViewer(QWidget *parent = 0)
        : QWidget(parent),
          d_pixmap(QPixmap()) { }

    void setPixmap(QPixmap & pixmap)
    {
        d_pixmap = pixmap;
        repaint();
    }

private:
    QPixmap d_pixmap;

protected:
    void paintEvent(QPaintEvent *)
    {
        if(d_pixmap.isNull())
            return;

        QPainter painter(this);

        if(d_pixmap.width() < width() // TODO PREVIEW
                || d_pixmap.height() < height())
        {
            int x = (width() - d_pixmap.width()) / 2;
            int y = (height() - d_pixmap.height()) / 2;
            painter.drawPixmap(x, y, d_pixmap);

        }else
        {
            int pixsize;

            if(width() > height())
            {
                pixsize = height();
                painter.translate((width() - pixsize) / 2, 0);
            }
            else
            {
                pixsize = width();
                painter.translate(0, (height() - pixsize) / 2);
            }
            painter.drawPixmap(0, 0, pixsize, pixsize,
                               d_pixmap.scaled(pixsize, pixsize, Qt::KeepAspectRatio));
        }
    }
};

class UImagePreview: public QWidget
{
    Q_OBJECT
    
public:
    explicit UImagePreview(QWidget *parent = 0);
    ~UImagePreview();
    void setImage(QString path);
    void clear();

private:
    Ui::UImagePreview *ui;
    USimpleImageViewer *simpleImageViewer;
    QPixmap originalPixmap;
};

#endif // UIMAGEPREVIEW_H
