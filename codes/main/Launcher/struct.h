#ifndef STRUCT_H
#define STRUCT_H

#include <QtCore>
#include <QLabel>
#include <QMovie>

namespace launcher {

    enum LaunchType {
        _NULL, ACTION, DIRECTORY, EXECUTABLE, FILE
    };

}

struct ItemData {
    QString name;
    QString description;
    QDateTime datatime;
    QLabel *icon;
    QLabel *poster;
    QWidget *preview;
    launcher::LaunchType type = launcher::LaunchType::_NULL;
};

#endif // STRUCT_H
