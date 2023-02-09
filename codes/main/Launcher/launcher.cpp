#include <QXmlStreamReader>
#include <QFile>
#include <QStack>
#include <QPushButton>
#include <QMediaPlayer>
#include <QtPlugin>

#include "launcher.h"
#include "ui_launcher.h"

Launcher::~Launcher() {
    delete ui;
}

Launcher::Launcher(QString filename, QWidget *parent): QMainWindow(parent), ui(new Ui::Launcher) {
    ui->setupUi(this);

    QDir imagedll = QDir(Setting::ImagePlugin());
    imageresolver = new ImagePluginManager(imagedll);

    this->LoadTheme();

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning("The Launcher File can't be read or doesn't exists.");
        this->close();
        return;
    }

    if(this->ParseLauncher(file) != QXmlStreamReader::NoError) {
        qWarning("The Launcher File is broken.");
        this->close();
        return;
    }

    this->LoadPlugin();
    this->BuildUI();
}

void Launcher::LoadTheme() {
    this->backimg = nullptr;
    QString backfile = Setting::Theme() + setting.getThemeName() + "/back";
    if(QFile::exists(backfile)) {
        this->backimgLabel = new QLabel(this);
        this->backimgLabel->move(0, 0);
        this->backimgLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        this->backimgLabel->stackUnder(this->centralWidget());
        QFile f = QFile(backfile);
        f.open(QFile::OpenModeFlag::ReadOnly);
        this->backimg = new QPixmap(imageresolver->resolve(f)->pixmap());
        f.close();
    }
}

void Launcher::LoadPlugin() {
    QLibrary loader(this->setting.getSceneryPlugin());
    loader.load();
    if(!loader.isLoaded()) {
        qWarning() << loader.errorString();
    } else {
        SceneryPlugin *(*init)(QMainWindow *, QVector<ItemData> *) = (SceneryPlugin *(*)(QMainWindow *, QVector<ItemData> *)) loader.resolve("init");

        if(init) {
            this->scenery = init(this, &items_data);
        } else {
            qWarning() << "Scenery Plugin Loader Error!";
            this->close();
        }
    }

    connect(scenery, &SceneryPlugin::Execute, this, &Launcher::Execute);
}

void Launcher::BuildUI() {

}

QXmlStreamReader::Error Launcher::ParseLauncher(QFile &file) {
    QXmlStreamReader reader(&file);

    while(!reader.atEnd()) {
        reader.readNext();
        if(reader.hasError()) return reader.error();

        switch(reader.tokenType()) {
            case QXmlStreamReader::StartElement:
                switch(toTag(reader.name())) {
                    case Tag::LAUNCHER:
                        ProcessTag_launcher(reader);
                        break;
                    default:
                        reader.skipCurrentElement();
                        break;
                }
                break;

            default:
                break;
        }
    }

    return reader.error();
}

Launcher::Tag Launcher::toTag(QStringView strV) {
    QString str = strV.toString();
    return toTag(str);
}

Launcher::Tag Launcher::toTag(QString &str) {
    if(str == "Launcher") return LAUNCHER;
    else if(str == "Collect") return COLLECT;
    else if(str == "Translate") return TRANSLATE;
    else if(str == "Explore") return EXPLORE;
    else if(str == "Item") return ITEM;
    else if(str == "Name") return NAME;
    else if(str == "Description") return DESCRIPTION;
    else if(str == "Icon") return ICON;
    else if(str == "Poster") return POSTER;
    else if(str == "Path") return PATH;
    else return _INVALID;
}

QXmlStreamReader::Error Launcher::ProcessTag_launcher(QXmlStreamReader &reader) {
    while(!reader.atEnd()) {
        reader.readNext();

        switch(reader.tokenType()) {
            case QXmlStreamReader::StartElement:
                switch(toTag(reader.name())) {
                    case Tag::COLLECT:
                        ProcessTag_collect(reader);
                        break;
                    default:
                        reader.skipCurrentElement();
                        break;
                }
                break;

            case QXmlStreamReader::EndElement:
                switch(toTag(reader.name())) {
                    case Tag::LAUNCHER:
                        return reader.error();
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

    return reader.error();
}

QXmlStreamReader::Error Launcher::ProcessTag_collect(QXmlStreamReader &reader) {
    while(!reader.atEnd()) {
        reader.readNext();

        switch(reader.tokenType()) {
            case QXmlStreamReader::StartElement:
                switch(toTag(reader.name())) {
                    case Tag::ITEM:
                        ProcessTag_item(reader);
                        break;
                    default:
                        reader.skipCurrentElement();
                        break;
                }
                break;

            case QXmlStreamReader::EndElement:
                switch(toTag(reader.name())) {
                    case Tag::COLLECT:
                        return reader.error();
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

    return reader.error();
}

QXmlStreamReader::Error Launcher::ProcessTag_item(QXmlStreamReader &reader) {
    ItemData item_data;
    ItemInfo item_info;
    item_data.poster = nullptr;
    item_data.icon = nullptr;
    while(!reader.atEnd()) {
        reader.readNext();

        switch(reader.tokenType()) {
            case QXmlStreamReader::StartElement:
                switch(toTag(reader.name())) {
                    case Tag::NAME:
                        ProcessTag_name(reader, item_data);
                        break;
                    case Tag::DESCRIPTION:
                        ProcessTag_description(reader, item_data);
                        break;
                    case Tag::ICON:
                        ProcessTag_icon(reader, item_data);
                        break;
                    case Tag::POSTER:
                        ProcessTag_poster(reader, item_data);
                        break;
                    case Tag::PATH:
                        ProcessTag_path(reader, item_info);
                        break;
                    default:
                        reader.skipCurrentElement();
                        break;
                }
                break;

            case QXmlStreamReader::EndElement:
                switch(toTag(reader.name())) {
                    case Tag::ITEM:
                        items_data.push_back(item_data);
                        items_info.push_back(item_info);
                        return reader.error();
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

    return reader.error();
}

QXmlStreamReader::Error Launcher::ProcessTag_name(QXmlStreamReader &reader, ItemData &item) {
    while(!reader.atEnd()) {
        reader.readNext();

        switch(reader.tokenType()) {
            case QXmlStreamReader::Characters:
                item.name += reader.text();
                break;

            case QXmlStreamReader::EndElement:
                switch(toTag(reader.name())) {
                    case Tag::NAME:
                        return reader.error();
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

    return reader.error();
}

QXmlStreamReader::Error Launcher::ProcessTag_description(QXmlStreamReader &reader, ItemData &item) {
    while(!reader.atEnd()) {
        reader.readNext();

        switch(reader.tokenType()) {
            case QXmlStreamReader::Characters:
                item.description += reader.text();
                break;

            case QXmlStreamReader::EndElement:
                switch(toTag(reader.name())) {
                    case Tag::DESCRIPTION:
                        return reader.error();
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

    return reader.error();
}

QXmlStreamReader::Error Launcher::ProcessTag_icon(QXmlStreamReader &reader, ItemData &item) {
    QFile *f;
    while(!reader.atEnd()) {
        reader.readNext();

        switch(reader.tokenType()) {
            case QXmlStreamReader::Characters:
                f = new QFile(QDir::currentPath() + '/' + reader.text().toString());
                f->open(QFile::OpenModeFlag::ReadOnly);
                item.icon = imageresolver->resolve(*f);
                delete f;
                break;

            case QXmlStreamReader::EndElement:
                switch(toTag(reader.name())) {
                    case Tag::ICON:
                        return reader.error();
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

    return reader.error();
}

QXmlStreamReader::Error Launcher::ProcessTag_poster(QXmlStreamReader &reader, ItemData &item) {
    QFile *f;
    while(!reader.atEnd()) {
        reader.readNext();

        switch(reader.tokenType()) {
            case QXmlStreamReader::Characters:
                f = new QFile(QDir::currentPath() + '/' + reader.text().toString());
                f->open(QFile::OpenModeFlag::ReadOnly);
                item.poster = imageresolver->resolve(*f);
                delete f;
                break;

            case QXmlStreamReader::EndElement:
                switch(toTag(reader.name())) {
                    case Tag::POSTER:
                        return reader.error();
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

    return reader.error();
}

QXmlStreamReader::Error Launcher::ProcessTag_path(QXmlStreamReader &reader, ItemInfo &item) {
    while(!reader.atEnd()) {
        reader.readNext();

        switch(reader.tokenType()) {
            case QXmlStreamReader::Characters:
                item.path += reader.text();
                break;

            case QXmlStreamReader::EndElement:
                switch(toTag(reader.name())) {
                    case Tag::PATH:
                        return reader.error();
                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }

    return reader.error();
}

void Launcher::UpdateTheme() {
    if(this->backimg) {
        this->backimgLabel->resize(this->centralWidget()->size());
        this->backimgLabel->setPixmap(this->backimg->scaled(this->centralWidget()->size(), Qt::AspectRatioMode::KeepAspectRatioByExpanding, Qt::TransformationMode::SmoothTransformation));
    }
}

void Launcher::resizeEvent(QResizeEvent *event) {
    this->scenery->resizeEvent(event);
    this->UpdateTheme();
}

slots void Launcher::Execute(qint32 index) {
    process.start(items_info[index].path, items_info[index].arguments);
    process.waitForStarted();
    this->setWindowState(Qt::WindowState::WindowMinimized);
    this->setVisible(false);
    process.waitForFinished(-1);
    this->setVisible(true);
    this->setWindowState(Qt::WindowState::WindowActive);
}
