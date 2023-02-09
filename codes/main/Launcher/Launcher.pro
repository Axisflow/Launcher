QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    launcher.cpp

HEADERS += \
    launcher.h \
    plugin/plugin.h \
    plugin/plugin_manager.h \
    plugin/plugin_global.h \
    setting/setting.h \
    setting/setting_global.h \
    struct.h \
    translator/translator.h \
    translator/translator_global.h

LIBS += \
    setting/setting.dll \
    translator/translator.dll \
    plugin/plugin.dll

FORMS += \
    launcher.ui

TRANSLATIONS += \
    Launcher_zh_TW.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
