#ifndef UPROGRESSFILEDIALOG_H
#define UPROGRESSFILEDIALOG_H

#include <QtGui>
#include <QtCore>

#include "USupport.h"

namespace Ui {
class UProgressFileDialog;
}

class UProgressFileDialog : public QDialog
{
    Q_OBJECT
    
    enum { Path, Other };

public:
    explicit UProgressFileDialog(QWidget *parent = 0);
    ~UProgressFileDialog();

    void setOpenFolderPath(QString path);
    QString openFolderPath();

    void setVisibleLabelMask(bool visible);
    void setTextLabelMask(QString text);

    void setVisibleButtonOpenFolderPathWhenComplete(bool visible);
    void setVisibleProgress(bool visible);

    void setVisibleLabelDescription(bool visible);
    void setTextLabelDescription(QString text);

    void showMessageWithAnEmptyListAnComplete(QString text, bool visible = true);

private:
    Ui::UProgressFileDialog *ui;        
    QStandardItemModel model;

    QString d_openFolderPath;
    QString prefixLabelValue;
    QString suffixLabelValue;

    QString prefixLabelMax;
    QString suffixLabelMax;

    QString textMaskLabel;

    QString messageWithAnEmptyListAnComplete;
    bool visibleMessageWithAnEmptyListAnComplete;

    bool visibleButtonOpenFolderPathWhenComplete;

    int corrupt;
    int value;

public slots:
    void addLog(QString log);
    void setRange(int min, int max);
    void setValue(int value);
    void setLabelText(QString text);
    void openFolder();
    void doCancel();        

private slots:
    void complete();
    void indexDoubleClicked(QModelIndex index);
    void testLabelMask();

signals:
    void canceled();
    void addedLog(QString);
};

#endif // UPROGRESSFILEDIALOG_H
