#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QMainWindow>
#include <QFile>
#include <QXmlStreamReader>
#include <QPluginLoader>

#include "struct.h"
#include "plugin/plugin.h"
#include "plugin/plugin_manager.h"
#include "setting/setting.h"
#include "translator/translator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Launcher; }
QT_END_NAMESPACE

class Launcher : public QMainWindow {
    Q_OBJECT

    public:
        Launcher(QString filename = "launcher.exl", QWidget *parent = nullptr);
        ~Launcher();

        struct ItemInfo {
            QString path;
            QStringList arguments;
            launcher::LaunchType type = launcher::LaunchType::_NULL;
        };

        enum Tag { _INVALID, LAUNCHER, TRANSLATE, COLLECT, EXPLORE, ITEM, NAME, DESCRIPTION, ICON, POSTER, PATH };

        static Tag toTag(QString &);
        static Tag toTag(QStringView);

    private:
        Ui::Launcher *ui;

        QXmlStreamReader::Error ParseLauncher(QFile &);
        QXmlStreamReader::Error ProcessTag_launcher(QXmlStreamReader &);
        QXmlStreamReader::Error ProcessTag_translate(QXmlStreamReader &);
        QXmlStreamReader::Error ProcessTag_collect(QXmlStreamReader &);
        QXmlStreamReader::Error ProcessTag_explore(QXmlStreamReader &);
        QXmlStreamReader::Error ProcessTag_item(QXmlStreamReader &);
        QXmlStreamReader::Error ProcessTag_name(QXmlStreamReader &, ItemData &);
        QXmlStreamReader::Error ProcessTag_description(QXmlStreamReader &, ItemData &);
        QXmlStreamReader::Error ProcessTag_icon(QXmlStreamReader &, ItemData &);
        QXmlStreamReader::Error ProcessTag_poster(QXmlStreamReader &, ItemData &);
        QXmlStreamReader::Error ProcessTag_path(QXmlStreamReader &, ItemInfo &);

        Translator translator;

    protected:
        QVector<ItemData> items_data;
        QVector<ItemInfo> items_info;
        Setting setting;
        QPixmap *backimg;
        QLabel *backimgLabel;
        SceneryPlugin *scenery;
        ImagePluginManager *imageresolver;
        QProcess process;

        void LoadTheme();
        void LoadPlugin();
        void Translate();

        void BuildUI();
        void UpdateTheme();

        // QWidget interface
        void resizeEvent(QResizeEvent *event) override;

    protected slots:
        void Execute(qint32);
};
#endif // LAUNCHER_H
