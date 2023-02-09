#include "qtbuiltinimage.h"
#include <QLabel>
#include <QImageReader>

extern "C" QTBUILTINIMAGE_EXPORT ImagePlugin *init() {
    return new QtBuiltinImage();
}

QStringList QtBuiltinImage::supportedExtensions() {
    QStringList *result = nullptr;
    result = new QStringList();
    QList<QByteArray> tmp = QImageReader::supportedImageFormats();
    for(QList<QByteArray>::iterator it = tmp.begin(); it != tmp.end(); it++) {
        *result << *it;
    }

    return *result;
}

QStringList QtBuiltinImage::supportedMimeTypes() {
    QStringList *result;
    result = new QStringList();
    QList<QByteArray> tmp = QImageReader::supportedMimeTypes();
    for(QList<QByteArray>::iterator it = tmp.begin(); it != tmp.end(); it++) {
        *result << *it;
    }

    return *result;
}

ImagePlugin::ParsePlugin::CheckResult QtBuiltinImage::checkExtension(QFile &f) {
    return this->check(f);
}

ImagePlugin::ParsePlugin::CheckResult QtBuiltinImage::checkMimeType(QFile &f) {
    return this->check(f);
}

QLabel *QtBuiltinImage::getData(QFile &f) {
    QLabel *tmp = new QLabel;
    QImage img = QImage(f.fileName());
    tmp->setPixmap(QPixmap::fromImage(img));
    tmp->setAlignment(Qt::AlignmentFlag::AlignCenter);
    return tmp;
}

ImagePlugin::ParsePlugin::CheckResult QtBuiltinImage::check(QFile &f) {
    QImage img = QImage(f.fileName());
    if(img.isNull()) return ImagePlugin::ParsePlugin::CheckResult::Wrong;
    else return ImagePlugin::ParsePlugin::CheckResult::Correct;
}
