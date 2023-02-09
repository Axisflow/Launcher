#include "setting.h"

Setting::Setting() {

}

QString Setting::getThemeName() {
    return "default";
}

QString Setting::getSceneryPlugin() {
    return "plugin/scenery/default.dll";
}

QString Setting::Theme() {
    return QCoreApplication::applicationDirPath() + "/theme/";
}

QString Setting::Plugin() {
    return QCoreApplication::applicationDirPath() + "/plugin/";
}

QString Setting::SceneryPlugin() {
    return QCoreApplication::applicationDirPath() + "/plugin/scenery/";
}

QString Setting::ImagePlugin() {
    return QCoreApplication::applicationDirPath() + "/plugin/image/";
}
