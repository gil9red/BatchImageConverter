#include <QtGui>
#include <QtCore>

#include "UBatchImageConverter.h"

class UTextCodec
{
public:
    static void initTextCodecs(const char *codecName)
    {
        QTextCodec *codec = QTextCodec::codecForName(codecName);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForTr(codec);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/IconProgram"));
    app.setApplicationName(appName);
    app.setApplicationVersion("0.0.3");

    UTextCodec::initTextCodecs("utf8");

    UConfig::setValue("DEFAULT", "Save", QDesktopServices::storageLocation(QDesktopServices::PicturesLocation));
    UConfig::setValue("DEFAULT", "IncludeSubFolders", true);
    UConfig::setValue("DEFAULT", "CloseAtEndFileSearchDialog", false);
    UConfig::setValue("DEFAULT", "CloseAtEndFileSaveDialog", false);

    UBatchImageConverter batchImageConverter;
    batchImageConverter.setWindowTitle(QString("%1 (%2) - %3 - Версия %4")
                                       .arg(appName)
                                       .arg(appAbbreviationName)
                                       .arg("Пакетный конвертор изображений")
                                       .arg(app.applicationVersion()));
    batchImageConverter.show();
    batchImageConverter.resize(800, 600);
    batchImageConverter.moveOnCenter();
//    batchImageConverter.showMaximized(); // TODO MAXIMIZED

    return app.exec();
}
