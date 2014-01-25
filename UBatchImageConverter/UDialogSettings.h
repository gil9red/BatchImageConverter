#ifndef UDIALOGSETTINGSGSETTINGS_H
#define UDIALOGSETTINGSGSETTINGS_H

#include <QtGui>
#include <QtCore>

namespace Ui {
class UDialogSettings;
}

#include "USupport.h"

class UDialogSettings: public QDialog
{
    Q_OBJECT
    
public:
    explicit UDialogSettings(QWidget *parent = 0);
    ~UDialogSettings();
    
private:
    Ui::UDialogSettings *ui;
    int d_result;

private:
    void load();

public slots:
    void ok();
    void cancel();
    void resetToDefault();
    void setResult(int r);
    int result();
};

#endif // UDIALOGSETTINGSGSETTINGS_H
