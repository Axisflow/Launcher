#include "plugin_manager.h"

ImagePluginManager::ImagePluginManager(QDir &path) {
    QStringList files = path.entryList(QDir::Filter::Files);
    QLibrary loader;
    for(qsizetype i = 0; i < files.size(); i++) {
        if(files[i].endsWith(".dll")) {
            loader.setFileName(path.absolutePath() + '/' + files[i]);
            if(loader.load()) {
                ImagePlugin *(*init)() = (ImagePlugin *(*)()) loader.resolve("init");
                if(init) {
                    ImagePlugin *tmp = init();
                    traversed[tmp] = false;
                    addSupportedExt(tmp);
                    addSupportedMime(tmp);
                    templateExt.push_back(tmp);
                    templateMime.push_back(tmp);
                }
            } else qWarning() << loader.errorString() << '\n';
        }
    }
}

QLabel *ImagePluginManager::resolve(QFile &f) {
    QHash<ImagePlugin *, bool> traverse = this->traversed;
    QString ext = QFileInfo(f.fileName()).suffix();
    ImagePlugin *maybe = nullptr;
    bool isExt = true;

    if(sortedExt.count(ext)) {
        for(qsizetype i = 0; i < sortedExt[ext].size(); i++) {
            if(!traverse[sortedExt[ext][i]]) {
                f.reset();
                switch (sortedExt[ext][i]->checkExtension(f)) {
                    case ImagePlugin::ParsePlugin::CheckResult::Correct:
                        f.reset();
                        return sortedExt[ext][i]->getData(f);
                    case ImagePlugin::ParsePlugin::CheckResult::ReadMimeType:
                        isExt = false;
                        break;
                    case ImagePlugin::ParsePlugin::CheckResult::Maybe:
                        if(maybe == nullptr) maybe = sortedExt[ext][i];
                        break;
                    default:
                        break;
                }
                traverse[sortedExt[ext][i]] = true;
            }

        }
    }

    QQueue<ImagePlugin *> tempExt = this->templateExt;
    QQueue<ImagePlugin *> tempMime = this->templateMime;
    while(!tempExt.isEmpty() && !tempMime.isEmpty()) {
        if(tempExt.isEmpty()) isExt = false;
        if(tempMime.isEmpty()) isExt = true;

        if(isExt) {
            if(!traverse[tempExt.head()]) {
                f.reset();
                switch (tempExt.head()->checkExtension(f)) {
                    case ImagePlugin::ParsePlugin::CheckResult::Correct:
                        f.reset();
                        return tempExt.head()->getData(f);
                    case ImagePlugin::ParsePlugin::CheckResult::ReadMimeType:
                        isExt = false;
                        break;
                    case ImagePlugin::ParsePlugin::CheckResult::Maybe:
                        if(maybe == nullptr) maybe = tempExt.head();
                        break;
                    default:
                        break;
                }
                traverse[tempExt.head()] = true;
            }
            tempExt.pop_front();
        } else {
            if(!traverse[tempMime.head()]) {
                f.reset();
                switch (tempMime.head()->checkExtension(f)) {
                    case ImagePlugin::ParsePlugin::CheckResult::Correct:
                        f.reset();
                        return tempMime.head()->getData(f);
                    case ImagePlugin::ParsePlugin::CheckResult::ReadExtension:
                        isExt = true;
                        break;
                    case ImagePlugin::ParsePlugin::CheckResult::Maybe:
                        if(maybe == nullptr) maybe = tempMime.head();
                        break;
                    default:
                        break;
                }
                traverse[tempMime.head()] = true;
            }
            tempMime.pop_front();
        }
    }

    f.reset();
    if(maybe) return maybe->getData(f);
    return nullptr;
}

void ImagePluginManager::addSupportedExt(ImagePlugin *ip) {
    QStringList list = ip->supportedExtensions();
    for(qsizetype l = 0; l < list.size(); l++) {
        sortedExt[list[l]].push_back(ip);
    }
}

void ImagePluginManager::addSupportedMime(ImagePlugin *ip){
    QStringList list = ip->supportedMimeTypes();
    for(qsizetype l = 0; l < list.size(); l++) {
        sortedMime[list[l]].push_back(ip);
    }
}
