#ifndef USWITCHPHOTOS_H
#define USWITCHPHOTOS_H

#include <QtGui>
#include <QtCore>

// TODO USwitchPhotos
// Зацикливание: начало и конец связаны
// создать срр файл, т.к. этот класс становится слишком большим для одного h
class USwitchPhotos: public QWidget
{
    Q_OBJECT

public:
    USwitchPhotos(QWidget *parent = 0):
        QWidget(parent),
        d_max(0),
        d_min(0),
        d_value(0)
    {
        tButtonPrevious = new QToolButton();
        tButtonPrevious->setEnabled(false);
        tButtonPrevious->setIcon(QIcon(":/IconPrevious"));
        tButtonPrevious->setToolTip("Предыдущее");
        tButtonPrevious->setAutoRaise(true);
        tButtonPrevious->setAutoRepeat(true);
        connect(tButtonPrevious, SIGNAL(clicked(bool)), SLOT(clickedPrevious()));

        labelDisplay = new QLabel();

        tButtonNext = new QToolButton();
        tButtonNext->setEnabled(false);
        tButtonNext->setIcon(QIcon(":/IconNext"));
        tButtonNext->setToolTip("Следующее");
        tButtonNext->setAutoRaise(true);
        tButtonNext->setAutoRepeat(true);
        connect(tButtonNext, SIGNAL(clicked(bool)), SLOT(clickedNext()));

        QHBoxLayout *hBoxLayout = new QHBoxLayout();
        hBoxLayout->setMargin(0);
        hBoxLayout->addWidget(tButtonPrevious);
        hBoxLayout->addWidget(labelDisplay);
        hBoxLayout->addWidget(tButtonNext);

        setLayout(hBoxLayout);

        updateEnabled();
        updateDisplay();
    }
    int max() { return d_max; }
    int min() { return d_min; }
    int value() { return d_value; }
    bool isEnabledPrevious() { return tButtonPrevious->isEnabled(); }
    bool isEnabledNext() { return tButtonNext->isEnabled(); }

private:
    QToolButton *tButtonPrevious;
    QLabel *labelDisplay;
    QToolButton *tButtonNext;

    int d_max;
    int d_min;
    int d_value;

private:
    void updateDisplay()
    {
        QString mask = "%1 из %2";
        labelDisplay->setText(mask.arg(d_value).arg(d_max));
    }
    void updateEnabled()
    {
        tButtonNext->setEnabled(d_value < d_max);
        tButtonPrevious->setEnabled(d_value > d_min);

        emit changeEnabledNext(tButtonNext->isEnabled());
        emit changeEnabledPrevious(tButtonPrevious->isEnabled());
    }

public slots:
    void setMax(const int max)
    {
        d_max = max;
        updateDisplay();
        updateEnabled();
    }
    void setMin(const int min)
    {
        d_min = min;
        if(d_min < 0)
            d_min = 0;

        updateDisplay();
        updateEnabled();
    }
    void setRange(const int min, const int max)
    {        
        d_min = min;
        if(d_min < 0)
            d_min = 0;

        d_max = max;
        updateDisplay();
        updateEnabled();
    }
    void setValue(const int value)
    {
        d_value = value;
        if(d_value > d_max)
            d_value = d_max;

        updateDisplay();
        updateEnabled();
    }
    void clickedNext()
    {
        d_value++;
        updateDisplay();
        updateEnabled();
        emit next();
    }
    void clickedPrevious()
    {
        d_value--;
        updateDisplay();
        updateEnabled();
        emit previous();
    }

signals:
    void next();
    void previous();
    void changeEnabledPrevious(bool);
    void changeEnabledNext(bool);
};

#endif // USWITCHPHOTOS_H
