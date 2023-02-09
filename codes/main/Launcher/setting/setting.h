#ifndef SETTING_H
#define SETTING_H

#include "setting_global.h"
#include <QtCore>

class SETTING_EXPORT Setting {
    public:
        typedef QFile::OpenModeFlag OpenModeFlag;
        Setting();
        Setting(QString, OpenModeFlag);

        QFile::FileError setFile(QString, OpenModeFlag);
        QString getFilePath();

        QString getThemeName();
        QString getSceneryPlugin();


        static QString Theme();
        static QString Plugin();
        static QString SceneryPlugin();
        static QString ImagePlugin();

    private:
        QString file;
};

#endif // SETTING_H
