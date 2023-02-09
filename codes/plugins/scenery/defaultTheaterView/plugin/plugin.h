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

#endif // PLUGIN_H
