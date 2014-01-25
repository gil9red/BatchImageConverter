#ifndef USEEKERFILESANDFOLDERS_H
#define USEEKERFILESANDFOLDERS_H

#include <QtGui>
#include <QtCore>

#include "USupport.h"
#include "UItemImage.h"


class USeekerFilesAndFolders: public QThread
{
    Q_OBJECT

public:
    USeekerFilesAndFolders(QObject *parent = 0)
        : QThread(parent),
          stop(false) { }

public:
    QStringList paths;
    QStringList filters;
    QDir::Filters filtersFile;
    QDirIterator::IteratorFlags iteratorFlags;
    bool stop;

public slots:
    void cancel(){ stop = true; }

protected:
    void run()
    {
        if(paths.isEmpty())
            return;

        foreach(QString path, paths)
        {
            QDirIterator iterator(path, filters, filtersFile, iteratorFlags);

            int countFiles = 0;
            int countFolders = 0;

            while(iterator.hasNext() && !stop)
            {
                msleep(1);
                QString d_path = iterator.next();

                QFileInfo info(d_path);

                if(info.isDir())
                {
                    emit folderWasFound(d_path);
                    emit foundFolders(++countFolders);
                }else
                {
                    emit fileWasFound(d_path);
                    emit foundFiles(++countFiles);
                }
            }
        }
    }

signals:
    void fileWasFound(QString);
    void folderWasFound(QString);
    void foundFiles(int);
    void foundFolders(int);
};

class USaveImage: public QThread
{
    Q_OBJECT

public:
    USaveImage(QObject *parent = 0)
        : QThread(parent),
          stop(false) { }

public:
    QList <UItemImage> listImages;

    QString pathSave;
    QString format;

    bool stop;

public slots:
    void cancel(){ stop = true; }

signals:
    void range(int, int);
    void value(int);
    void currentImage(QString);

protected:
    void run()
    {
        int count = listImages.size();

        emit range(0, count);

        for(int i = 0; i < count && !stop; i++)
        {
            msleep(1);
            QString name = listImages.at(i).name;
            QString path = listImages.at(i).path;

            emit value(i + 1);
            emit currentImage(path);

            QImage *image = new QImage();
            image->load(path);
            QString newPathFile = QString("%1/%2.%3")
                    .arg(pathSave)
                    .arg(name)
                    .arg(format);

            image->save(newPathFile, format.toUtf8());
            delete image;
        }
    }
};

#endif // USEEKERFILESANDFOLDERS_H
