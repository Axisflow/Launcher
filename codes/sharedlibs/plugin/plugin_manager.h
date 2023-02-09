#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <QLibrary>
#include <QMap>
#include <QVector>
#include <QQueue>
#include "plugin.h"

class PLUGIN_EXPORT ImagePluginManager {
    public:
        ImagePluginManager(QDir &);

        QLabel *resolve(QFile &);

    private:
        QHash<ImagePlugin *, bool> traversed;
        QMap<QString, QVector<ImagePlugin *>> sortedExt;
        QMap<QString, QVector<ImagePlugin *>> sortedMime;
        QQueue<ImagePlugin *> templateExt;
        QQueue<ImagePlugin *> templateMime;

        void addSupportedExt(ImagePlugin *);
        void addSupportedMime(ImagePlugin *);
};

#endif // PLUGIN_MANAGER_H
