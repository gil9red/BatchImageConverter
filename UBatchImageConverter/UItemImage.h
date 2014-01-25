#ifndef UITEMIMAGE_H
#define UITEMIMAGE_H

#include <QtGui>
#include <QtCore>

class UItemImage
{
public:
    QString name;
    QString format;
    qint64 sizeFile;
    QSize sizeImage;
    QString path;
};

static UItemImage getItemImage(QStandardItemModel *model, int row, int column)
{ return model->item(row, column)->data(Qt::UserRole).value <UItemImage> (); }

Q_DECLARE_METATYPE(UItemImage)

#endif // UITEMIMAGE_H
