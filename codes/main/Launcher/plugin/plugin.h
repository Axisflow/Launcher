#ifndef PLUGIN_H
#define PLUGIN_H

#include <QtCore>
#include <QLabel>
#include "plugin_global.h"

class PLUGIN_EXPORT SceneryPlugin : public QObject {
    Q_OBJECT
    public:
        virtual ~SceneryPlugin() = default;
    public slots:
        virtual void resizeEvent(QResizeEvent *&) = 0;

    signals:
        void Execute(qint32 index);
};

template<typename GetType, typename ParseType>
class PLUGIN_EXPORT ParsePlugin {
    public:
        enum CheckResult { None, Wrong, Maybe, Correct, ReadExtension, ReadMimeType };

        virtual ~ParsePlugin() = default;

        virtual QStringList supportedExtensions() = 0;
        virtual QStringList supportedMimeTypes() = 0;
        virtual CheckResult checkExtension(ParseType &) = 0;
        virtual CheckResult checkMimeType(ParseType &) = 0;
        virtual GetType *getData(ParseType &) = 0;
};

class PLUGIN_EXPORT ImagePlugin : public ParsePlugin<QLabel, QFile> {
    public:
        virtual ~ImagePlugin() = default;
};

#endif // PLUGIN_H
