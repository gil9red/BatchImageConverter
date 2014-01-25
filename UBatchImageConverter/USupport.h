#ifndef USUPPORT_H
#define USUPPORT_H

#include <QtGui>
#include <QtCore>

#include "UItemImage.h"

#define appName "MassImgConvert"
#define appAbbreviationName "M.I.C."

static QString splitNumber(qint64 n)
{
    QString number = QString::number(n);
    int count = number.size();

    while(count > 2)
    {
        count -= 3;
        if(count == 0)
            break;
        number.insert(count, ' ');
    }

    return number;
}

namespace UColumnTable
{
enum ColumnTable { Name, Format, SizeFile, SizeImage, Path, Count };
}

namespace UBytes
{
enum UPrefixByte { B, k, M, G, T, P, E, Z, Y, begin = B, last = Y };

static QStringList listPrefixBytes()
{
    QStringList list;

    list << "байт"
         << "килобайт"
         << "мегабайт"
         << "гигабайт"
         << "терабайт"
         << "петабайт"
         << "эксабайт"
         << "зеттабайт"
         << "йоттабайт";

    return list;
}

static QStringList bytes(qint64 byte, UPrefixByte prefix = B, bool replacementPointToComma = true)
{
    QStringList list;

    double b = byte;

    int c = prefix;

    if(c == B)
        list.append(QString("%1 %2").arg(splitNumber(byte)).arg(listPrefixBytes().at(c)));

    while(b > 1)
    {
        b /= 1024;
        if(b < 1)
            break;

        if(c == last)
            break;

        c++;

        QString string = QString("%1 %2")
                .arg(b, 0, 'f', 2)
                .arg(listPrefixBytes().at(c));

        if(replacementPointToComma)
            string.replace('.', ',');

        list.append(string);
    }

    return list;
}

}

// процент от числа
static double percentNumber(double number, int percent)
{ return percent < 0 ? number : (number / 100) * percent; }

static bool imageFileIsValid(QString path)
{ return QImageReader(path).canRead(); }

static QList <QStandardItem *> infoImage(QString path)
{
    QImageReader reader(path);
    if(!reader.canRead())
        return QList <QStandardItem *> ();    

    QFileInfo fileInfo(path);

    QSize d_imageSize = reader.size();

    QString imageName = fileInfo.baseName();
    QString imageFormat = fileInfo.suffix().toUpper();
    QString imageFileSize = UBytes::bytes(fileInfo.size()).last();
    QString imageSize = QString("%1x%2").arg(d_imageSize.width()).arg(d_imageSize.height());
    QString imagePath = path;

    UItemImage itemImage;
    itemImage.name = imageName;
    itemImage.format = imageFormat;
    itemImage.sizeFile = fileInfo.size();
    itemImage.sizeImage = d_imageSize;
    itemImage.path = path;

    QStandardItem *itemName = new QStandardItem(imageName);
    QStandardItem *itemFormat = new QStandardItem(imageFormat);
    QStandardItem *itemSizeFile = new QStandardItem(imageFileSize);
    QStandardItem *itemSizeImage = new QStandardItem(imageSize);
    QStandardItem *itemFilePath = new QStandardItem(imagePath);

    itemFormat->setIcon(QIcon(QString(":/Icon%1").arg(imageFormat.toUpper())));

    QVariant v;
    v.setValue(itemImage);

    itemName->setData(v, Qt::UserRole);
    itemFormat->setData(imageFormat, Qt::UserRole + 1);
    itemSizeFile->setData(fileInfo.size(), Qt::UserRole + 1);
    itemSizeImage->setData(d_imageSize.width() * d_imageSize.height(), Qt::UserRole + 1);
    itemFilePath->setData(imagePath, Qt::UserRole + 1);


    itemName->setEditable(false);
    itemFormat->setEditable(false);
    itemSizeFile->setEditable(false);
    itemSizeImage->setEditable(false);
    itemFilePath->setEditable(false);

    itemName->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    itemFormat->setTextAlignment(Qt::AlignCenter);
    itemSizeFile->setTextAlignment(Qt::AlignCenter);
    itemSizeImage->setTextAlignment(Qt::AlignCenter);
    itemFilePath->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    itemName->setToolTip(itemName->text());
    itemFormat->setToolTip(itemFormat->text());
    itemSizeFile->setToolTip(UBytes::bytes(fileInfo.size()).join("\n"));
    itemSizeImage->setToolTip(itemSizeImage->text());
    itemFilePath->setToolTip(itemFilePath->text());

    QList <QStandardItem *> items;
    for(int i = 0; i < UColumnTable::Count; i++)
    {
        QStandardItem item;
        items.append(&item);
    }

    items[UColumnTable::Name] = itemName;
    items[UColumnTable::Format] = itemFormat;
    items[UColumnTable::SizeFile] = itemSizeFile;
    items[UColumnTable::SizeImage] = itemSizeImage;
    items[UColumnTable::Path] = itemFilePath;

    return items;
}

class UFoldoutGroupBox: public QGroupBox
{
    Q_OBJECT

public:
    UFoldoutGroupBox(QWidget *parent = 0)
        : QGroupBox(parent)
    {
        setCheckable(true);
        setChecked(true);
        setExpand(true);

        connect(this, SIGNAL(toggled(bool)), SLOT(setExpand(bool)));
    }

public slots:
    void setExpand(bool expand)
    {
        setFlat(!expand);

        foreach(QWidget *child, findChildren <QWidget *> ())
            child->setVisible(expand);
    }
};

const QString nameFileConfiguration = "config.ini";

class UConfig
{
public:
    static void setValue(const QString group, const QString key,
                         const QVariant value)
    {
        QSettings ini(qApp->applicationDirPath()
                      + "/"
                      + nameFileConfiguration, QSettings::IniFormat);

        ini.setIniCodec("utf8");
        ini.beginGroup(group);
        ini.setValue(key, value);
        ini.endGroup();
    }

    static QVariant value(const QString group, const QString key,
                          const QVariant &defaultValue = QVariant())
    {
        QSettings ini(qApp->applicationDirPath()
                      + "/"
                      + nameFileConfiguration, QSettings::IniFormat);

        ini.setIniCodec("utf8");
        ini.beginGroup(group);
        return ini.value(key, defaultValue);
    }
};

static QImage paintingChessGrid(const QSize size, const int sizeCell = 15,
                                const QPair <QColor, QColor> color = QPair <QColor, QColor> (Qt::lightGray, Qt::white))
{
    QImage image(size, QImage::Format_RGB32);

    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    for(int row = 0; row < image.width(); row += sizeCell)
        for(int column = 0; column < image.height(); column += sizeCell)
        {
            if((row % 2) == (column % 2))
                painter.setBrush(color.first);
            else
                painter.setBrush(color.second);

            painter.drawRect(row, column, sizeCell, sizeCell);
        }

    return image;
}

#endif // USUPPORT_H
