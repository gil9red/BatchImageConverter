QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = M.I.C.
TEMPLATE = app

DESTDIR = ../bin

SOURCES += main.cpp\
        UBatchImageConverter.cpp \
    UProgressFileDialog.cpp \
    UImageSettingsBatch.cpp \
    sbglass.cpp \
    UImagePreview.cpp \
    UAboutProgram.cpp \
    UImageViewer.cpp \
    UDialogSettings.cpp \
    QProgressIndicator/QProgressIndicator.cpp \
    USlideshowWidget.cpp

HEADERS  += UBatchImageConverter.h \
    USupport.h \
    USeekerFilesAndFolders.h \
    UProgressFileDialog.h \
    UItemImage.h \
    UImageSettingsBatch.h \
    sbglass.h \
    UImagePreview.h \
    UAboutProgram.h \
    UImageViewer.h \
    UDialogSettings.h \
    QProgressIndicator/QProgressIndicator.h \
    USwitchPhotos.h \
    USlideshowWidget.h

FORMS    += UBatchImageConverter.ui \
    UProgressFileDialog.ui \
    UImageSettingsBatch.ui \
    UImagePreview.ui \
    UAboutProgram.ui \
    UDialogSettings.ui

RC_FILE = app.rc

OTHER_FILES += \
    app.rc \   
    ChangeLogs.txt

RESOURCES += \
    resource.qrc
