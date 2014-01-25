#ifndef UIMAGESETTINGSBATCH_H
#define UIMAGESETTINGSBATCH_H

#include <QtGui>
#include <QtCore>

#include "ui_UImageSettingsBatch.h"

#include "USupport.h"
#include "UImagePreview.h"

namespace Ui {
class UImageSettingsBatch;
}

class UImageSettingsBatch : public QWidget
{
    Q_OBJECT
    
public:
    explicit UImageSettingsBatch(QWidget *parent = 0);
    ~UImageSettingsBatch();
    
    void load();
    void save();

public:
    Ui::UImageSettingsBatch *ui;
    bool existSavePath;

public slots:
    void selectPathSave();    

private slots:
    void testSavePath(QString path);
    void testSavePath();

signals:
    void existedSavePath();
};

#endif // UIMAGESETTINGSBATCH_H
