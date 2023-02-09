#ifndef QTBUILTINIMAGE_H
#define QTBUILTINIMAGE_H

#include "plugin/plugin.h"
#include "QtBuiltinImage_global.h"

class QTBUILTINIMAGE_EXPORT QtBuiltinImage: public ImagePlugin {
    public:
        // ParsePlugin interface
        QStringList supportedExtensions() override;
        QStringList supportedMimeTypes() override;
        CheckResult checkExtension(QFile &) override;
        CheckResult checkMimeType(QFile &) override;
        QLabel *getData(QFile &) override;

    private:
        CheckResult check(QFile &);
};

#endif // QTBUILTINIMAGE_H
